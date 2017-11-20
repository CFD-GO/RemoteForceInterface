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

#define RFI_DATA_VOL 0
#define RFI_DATA_FORCE 1
#define RFI_DATA_MOMENT 4

#define RFI_DATA_SIZE 13

#define MPI_SIZE_T MPI_UNSIGNED_LONG

#define RFI_ArrayOfStructures 0x01
#define RFI_StructureOfArrays 0x02

#define RFI_NRot 0x01
#define RFI_Rot 0x02

enum rfi_type_t {
  ForceCalculator,
  ForceIntegrator
};

template < typename real_t, rfi_type_t rfi_type >
class RemoteForceInterface {
private:
  int world_size; ///< Size of current program world
  int universe_size; ///< Size of the universe (both integrated programs)
  int rank; ///< My rank in world
  int workers; ///< Number of workers
  int masters; ///< Number of masters
  MPI_Comm intercomm; ///< Intercomm between master and slave
  size_t ntab; ///< Length of tab
  size_t totsize; ///< Total number of particles
  std::vector<real_t> tab; ///< Array storing all the data of particles
  std::vector<size_t> sizes; ///< Array of sizes of data recieved from each slave/master 
  std::vector<size_t> offsets; ///< Array of offsets of data recieved from each slave/master
  std::vector<MPI_Request> reqs; ///< Array of MPI requests for non-blocking calls
  std::vector<MPI_Status> stats; ///< Array of MPI status for non-blocking calls
  MPI_Datatype MPI_REAL_T; ///< The MPI datatype handle for real_t (either MPI_FLOAT or MPI_DOUBLE)
  int storage; ///< Storage type (either RFI_ArrayOfStructures, RFI_StructureOfArrays)
  int particle_size;
  bool rot;
  bool active;
  bool connected;
public:
  RemoteForceInterface();
  ~RemoteForceInterface();
  
  int Negotiate(int, int);
  
  int Spawn(char * worker_program, char * args[]);
  inline const size_t size() const { return totsize; }
  inline real_t* Particles() { return &tab[0]; }
  void GetParticles();
  void SetParticles();
  void Close();
  inline bool Active() { return active; }
  inline bool Connected() { return connected; }
  inline int Workers() { return workers; }
  inline size_t& Size(int i) { return sizes[i]; }
  inline bool Rot() { return rot; }
  inline int space_for_workers() { return universe_size - world_size; };
  inline real_t& Data(size_t i, int j) {
    if (storage == RFI_ArrayOfStructures) {
      return tab[i*particle_size + j];
    } else {
      return tab[i + j*totsize];
    }
  }
  inline void SetData(size_t i, int j, real_t val) {
    Data(i,j) = val;
  }
};

};
#endif
