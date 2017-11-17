#ifndef REMOTEFORCEINTERFACE_H
#define REMOTEFORCEINTERFACE_H

#include "mpi.h" 
#include <stdlib.h>
#include <stdio.h>
#include <vector>

namespace rfi {

#define RFI_CODE_HANDSHAKE 1
#define RFI_CODE_FINISH 2
#define RFI_CODE_PARTICLES 3
#define RFI_CODE_FORCES 4
#define RFI_CODE_ABORT 0xFF

#define RFI_FINISHED ((size_t) -1)

#define RFI_DATA_R 0
#define RFI_DATA_POS 1
#define RFI_DATA_VEL 4
#define RFI_DATA_ANGVEL 7
#define RFI_DATA_IN_SIZE 10

#define RFI_DATA_VOL 0
#define RFI_DATA_FORCE 1
#define RFI_DATA_SIGMA 4
#define RFI_DATA_OUT_SIZE 13

#define RFI_DATA_SIZE 13

#define MPI_SIZE_T MPI_UNSIGNED_LONG

enum StorageType {
 ArrayOfStructures,
 StructureOfArrays
};

template < typename real_t, StorageType Storage >
class RemoteForceInterface {
private:
  int world_size, universe_size;
  int rank;
  int workers;
  int masters;
  MPI_Comm intercomm;
  size_t totsize;
  std::vector<real_t> tab;
  std::vector<size_t> sizes;
  std::vector<size_t> offsets;
  std::vector<MPI_Request> reqs;
  std::vector<MPI_Status> stats;
  MPI_Datatype MPI_REAL_T;
public:
  RemoteForceInterface();
  ~RemoteForceInterface();
  int Start(char * worker_program, char * args[]);
  inline const size_t size() const { return totsize; }
  inline real_t* Particles() { return &tab[0]; }
  void GetParticles();
  void SetParticles();
  void Close();
  inline bool Active() { return intercomm != MPI_COMM_NULL; }
  inline int space_for_workers() { return universe_size - world_size; };
};

};
#endif
