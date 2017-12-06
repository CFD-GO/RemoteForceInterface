#include <mpi.h>
#include "../../MPMD.hpp"
#include "../../RemoteForceInterface.hpp"
#include "../Common.hpp"


int main(int argc, char *argv[])
{
   int ret;
   MPMDHelper MPMD;
   rfi::RemoteForceInterface< rfi::ForceCalculator, rfi::RotParticle > RFI;

   MPI_Init(&argc, &argv);
   MPMD.Init(MPI_COMM_WORLD, "MASTER");

   int how_many_spawn = MPMD.universe_size - MPMD.world_size;
   if (how_many_spawn < 1) {
     fprintf(stderr, "Not enough space for spawning\n");
     exit(-1);
   }
   printf("how_many_spawn: %d\n", how_many_spawn);
   MPMDIntercomm inter = MPMD.Spawn("./slave", MPI_ARGV_NULL, how_many_spawn, MPI_INFO_NULL);
   
   MPMD.Identify();

   ret = RFI.Connect(MPMD.work, inter.work);
   if (ret) return ret;
   assert(RFI.Connected());
   
   RunForceCalculator(RFI,MPMD);

   RFI.Close();

   MPI_Finalize();
   return 0;
}
