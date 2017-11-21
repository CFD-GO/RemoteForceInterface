#include "RemoteForceInterface.h"

#ifndef error
 #define error printf
#endif
#ifndef debug1
 #define debug1 printf
#endif
#ifndef ERROR
 #define ERROR printf
#endif
#ifndef output
 #define output printf
#endif

namespace rfi {

template < typename real_t, rfi_type_t rfi_type >
RemoteForceInterface < real_t, rfi_type >::RemoteForceInterface(MPI_Comm intercomm_) : workers(0), masters(0), intercomm(intercomm_), totsize(0) {
   int *universe_sizep, flag;
   MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
   MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &universe_sizep, &flag);  
   if (!flag) { 
     /* This will not happen with most common MPI implementations */
     universe_size = 0; // LCOV_EXCL_LINE
   } else universe_size = *universe_sizep;
   
   connected = false;
   active = false;
   my_type = rfi_type;
   name = "N/A";
   particle_size = 0;
   if (sizeof(real_t) == 8)
    MPI_REAL_T = MPI_DOUBLE;
   else if (sizeof(real_t) == 4)
    MPI_REAL_T = MPI_FLOAT;
   else {
    ERROR("Unknown type in RemoteForceInterface");
   }
   
   if (intercomm != MPI_COMM_NULL) {
     Connect();
   }
}

template < typename real_t, rfi_type_t rfi_type >
RemoteForceInterface < real_t, rfi_type >::~RemoteForceInterface() {
  if (intercomm != MPI_COMM_NULL) {
    ERROR("RFI: This should never happen\n"); // LCOV_EXCL_LINE
  }
}

template < typename real_t, rfi_type_t rfi_type >
int RemoteForceInterface < real_t, rfi_type >::Negotiate(int storage_type, int rot_or_nrot) {
  if (! connected) return -1;
  output("RFI: %s: Starting negotiations ...\n", name);
  MPI_Barrier(intercomm);
  int other_type;
  MPI_Allreduce( &my_type, &other_type, 1, MPI_INT, MPI_MAX, intercomm);
  output("RFI: %s: my_type=%d other_type=%d\n",name,my_type, other_type);
  if (my_type == ForceCalculator) {
   if (other_type != ForceIntegrator) return -1;
  } else if (my_type == ForceIntegrator) {
   if (other_type != ForceCalculator) return -1;
  } else {
    ERROR("RFI: %s: Unknown type\n", name);
    return -1;
  }
  int other_storage_type;
  MPI_Allreduce( &storage_type, &other_storage_type, 1, MPI_INT, MPI_BAND, intercomm);
  output("RFI: %s: my_storage_type=%d other_storage_type=%d\n", name, storage_type, other_storage_type);
  storage = other_storage_type & storage_type;
  if (storage == 0) {
    ERROR("RFI: %s: Cannot agree on storage type\n", name);
    return -1;
  }
  if (storage & RFI_ArrayOfStructures) 
   storage = RFI_ArrayOfStructures;
  else if (storage & RFI_StructureOfArrays)
   storage = RFI_StructureOfArrays;
  else {
    ERROR("RFI: %s: Unknown storage type\n", name);
    return -1;
  }
  output("RFI: %s: Decided on storage type: %d\n", name, storage);

  int other_rot_or_nrot;
  MPI_Allreduce( &rot_or_nrot, &other_rot_or_nrot, 1, MPI_INT, MPI_BAND, intercomm);
  output("RFI: %s: my_rot_or_nrot=%d other_rot_or_nrot=%d\n", name, rot_or_nrot, other_rot_or_nrot);
  rot_or_nrot = other_rot_or_nrot & rot_or_nrot;
  if (rot_or_nrot == 0) {
    ERROR("RFI: %s: Cannot agree on rot_or_nrot type\n", name);
    return -1;
  }
  if (rot_or_nrot & RFI_Rot) 
   rot = true;
  else if (rot_or_nrot & RFI_NRot)
   rot = false;
  else {
    ERROR("RFI: %s: Unknown rot_or_nrot type\n",name);
    return -1;
  }
  
  if (rot) {
   particle_size = 20;
   output("RFI: %s: Decided to calculate with rotation\n",name);
  } else {
   particle_size = 20;
   output("RFI: %s: Decided to calculate without rotation\n",name);
  }

  if (storage == RFI_ArrayOfStructures) {
    output("Adding type ...\n");
    MPI_Type_contiguous(particle_size, MPI_REAL_T, &MPI_PARTICLE);
  } else {
    MPI_Type_vector(particle_size, 1, totsize, MPI_REAL_T, &MPI_PARTICLE);
  }
  MPI_Type_commit(&MPI_PARTICLE);

  output("RFI: %s: Finished negotiations\n",name);
  MPI_Barrier(intercomm);
  active = true;
  return 0;
}


template < typename real_t, rfi_type_t rfi_type >
int RemoteForceInterface < real_t, rfi_type >::Connect() {
   MPI_Comm_remote_size(intercomm, &workers);
   MPI_Comm_size(intercomm, &masters);
   MPI_Comm_rank(intercomm, &rank);
   sizes.resize(workers, 0);
   offsets.resize(workers+1, 0);
   output("done %d\n",my_type);
   connected=true;
   Zero();
   return 0;
}


template < typename real_t, rfi_type_t rfi_type >
int RemoteForceInterface < real_t, rfi_type >::Spawn(char * worker_program, char * args[]) {
   if (Connected()) {
    error("RFI: %s: Already started\n", name);
    return -2;
   }
   if (universe_size == world_size) {
    ERROR("No room to start workers"); 
    return -1;
   }
   MPI_Comm everyone;
   MPI_Comm_spawn(worker_program, args, universe_size - world_size,
             MPI_INFO_NULL, 0, MPI_COMM_WORLD, &everyone,  
             MPI_ERRCODES_IGNORE); 
             
//   MPI_Group group;
//   MPI_Comm_group(MPI_COMM_WORLD, &group);
//   MPI_Comm_create(everyone, group, &intercomm);
   intercomm = everyone;
   
   return Connect();
}

template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::Alloc() {
    offsets[0] = 0;
    for (int i=0; i<workers; i++) offsets[i+1] = offsets[i] + sizes[i];
    totsize = offsets[workers];
    ntab = totsize * particle_size;
    if (ntab > tab.size()) tab.resize(ntab);
    if (storage == RFI_ArrayOfStructures) {
    } else {
      MPI_Type_vector(particle_size, 1, totsize, MPI_REAL_T, &MPI_PARTICLE);
      MPI_Type_commit(&MPI_PARTICLE);
    }
}


template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::Zero() {
  totsize = 0;
  for (int i=0; i<workers; i++) {
   sizes[i] = 0;
   offsets[i] = 0;
  }
}

template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::Close() {
  if (! Active()) return;
  output("RFI: %s: Closing ...\n", name);
  if (rfi_type == ForceIntegrator) {
   for (int i=0; i<workers; i++) {
    sizes[i] = RFI_FINISHED;
   }
  }
  SendSizes();
  if (rfi_type == ForceIntegrator) {
   Finish();
  }
}

template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::Finish() {
  if (! Active()) return;
  Zero();
  MPI_Barrier(intercomm);
  output("RFI: %s: Closed.\n", name);
  MPI_Comm_free(&intercomm);
  intercomm = MPI_COMM_NULL;
  connected = false;
  active = false;
}


template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::SendSizes() {
  if (! Active()) return;
  output("RFI: %s: SendSizes ...\n", name);
  if (rfi_type == ForceCalculator) {
    MPI_Alltoall(NULL, 0, MPI_SIZE_T, &sizes[0], 1, MPI_SIZE_T, intercomm);
    if (sizes[0] == RFI_FINISHED) {
     Finish();
    } else {
     Alloc();
    }
  } else {
    MPI_Alltoall(&sizes[0], 1, MPI_SIZE_T, NULL, 0, MPI_SIZE_T, intercomm);
  }
}


template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::SendForces() {
    if (! Active()) return;
    debug1("RFI: %s: SendForces ...\n", name);
    for (int i=0; i<workers; i++) if (sizes[i] > 0) {
      if (rfi_type == ForceCalculator) {
        MPI_Send(&Data(offsets[i],0), sizes[i], MPI_PARTICLE, i, 0xF1, intercomm);
      } else {
        MPI_Status stat;
        MPI_Recv(&Data(offsets[i],0), sizes[i], MPI_PARTICLE, i, 0xF1, intercomm, &stat);
      }
    }
}

template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::SendParticles() {
    if (! Active()) return;
    debug1("RFI: %s: SendParticles ...\n", name);
    for (int i=0; i<workers; i++) if (sizes[i] > 0) {
      if (rfi_type == ForceCalculator) {
        MPI_Status stat;
        MPI_Recv(&Data(offsets[i],0), sizes[i], MPI_PARTICLE, i, 0xF2, intercomm, &stat);
      } else {
        MPI_Send(&Data(offsets[i],0), sizes[i], MPI_PARTICLE, i, 0xF2, intercomm);
      }
    }
}

};