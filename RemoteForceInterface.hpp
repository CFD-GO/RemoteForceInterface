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
     universe_size = 0;
   } else universe_size = *universe_sizep;
   if (intercomm != MPI_COMM_NULL) {
      connected = true;
   } else {
      connected = false;
   }
   active = false;
}

template < typename real_t, rfi_type_t rfi_type >
RemoteForceInterface < real_t, rfi_type >::~RemoteForceInterface() {
  if (intercomm != MPI_COMM_NULL) {
    MPI_Comm_free(&intercomm);
  }
}

template < typename real_t, rfi_type_t rfi_type >
int RemoteForceInterface < real_t, rfi_type >::Negotiate(int storage_type, int rot_or_nrot) {
  if (! connected) return -1;
  active = true;
}


template < typename real_t, rfi_type_t rfi_type >
int RemoteForceInterface < real_t, rfi_type >::Spawn(char * worker_program, char * args[]) {
   if (intercomm != MPI_COMM_NULL) {
    error("RemoteForceInterface(M) Already started\n");
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
             
   MPI_Group group;
   MPI_Comm_group(MPI_COMM_WORLD, &group);
   MPI_Comm_create(everyone, group, &intercomm);
   
   {
    int s1,s2;
    MPI_Comm_remote_size(everyone, &s1);
    MPI_Comm_remote_size(intercomm, &s2);
    output("RemoteForceInterface(M) Total children: %d, intercomm with: %d\n",s1,s2);
   }
   MPI_Comm_remote_size(intercomm, &workers);
   MPI_Comm_size(intercomm, &masters);
   MPI_Comm_rank(intercomm, &rank);
   sizes.resize(workers, 0);
   offsets.resize(workers+1, 0);
   reqs.resize(workers);
   stats.resize(workers);
   return 0;
}

template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::Close() {
  if (intercomm == MPI_COMM_NULL) return;
  output("RemoteForceInterface(M) Closing ...\n");
  totsize = 0;
  for (int i=0; i<workers; i++) {
   sizes[i] = 0;
   offsets[i] = 0;
  }
  MPI_Comm_free(&intercomm);
  intercomm = MPI_COMM_NULL;
}


template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::GetParticles() {
    if (intercomm == MPI_COMM_NULL) return;
    debug1("RemoteForceInterface(M) Exchange of sizes ...\n");
    MPI_Alltoall(NULL, 0, MPI_SIZE_T, &sizes[0], 1, MPI_SIZE_T, intercomm);
    for (int i=0; i<workers; i++) if (sizes[i] == RFI_FINISHED) { Close(); return; }
    for (int i=0; i<workers; i++) debug1("RemoteForceInterface(M) [%2d] we got %ld from %d\n", rank, (size_t) sizes[i], i);
    for (int i=0; i<workers; i++) offsets[i+1] = offsets[i] + sizes[i];
    totsize = offsets[workers];
    if (totsize > tab.size()) tab.resize(totsize);
    debug1("RemoteForceInterface(M) Sending ...\n");
    for (int i=0; i<workers; i++) {
        MPI_Irecv(&tab[offsets[i]], sizes[i], MPI_REAL_T, i, i, intercomm, &reqs[i]);
    }
    MPI_Waitall(workers, &reqs[0], &stats[0]);
}

template < typename real_t, rfi_type_t rfi_type >
void RemoteForceInterface < real_t, rfi_type >::SetParticles() {
    if (intercomm == MPI_COMM_NULL) return;
    debug1("RemoteForceInterface(M) Receiving ...\n");
    for (int i=0; i<workers; i++) {
        MPI_Isend(&tab[offsets[i]], sizes[i], MPI_REAL_T, i, i+workers, intercomm, &reqs[i]);
    }
    MPI_Waitall(workers, &reqs[0], &stats[0]);
}

};