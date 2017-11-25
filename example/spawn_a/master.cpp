#include <mpi.h>
#include "../../MPMD.hpp"
#include "../../RemoteForceInterface.hpp"
#include "../Common.hpp"


int main(int argc, char *argv[])
{
   int ret;
   MPMDHelper MPMD;

   MPI_Init(&argc, &argv);
   MPMD.Init(MPI_COMM_WORLD, "MASTER");

   int how_many_spawn = MPMD.universe_size - MPMD.world_size;
   if (how_many_spawn < 1) {
     ERROR("Not enough space for spawning\n");
     exit(-1);
   }
   printf("how_many_spawn: %d\n", how_many_spawn);
   MPMDIntercomm inter = MPMD.Spawn("./slave", MPI_ARGV_NULL, how_many_spawn, MPI_INFO_NULL);
   
   MPMD.Identify();

   rfi::RemoteForceInterface< rfi::ForceCalculator, rfi::RotParticle > RFI(inter.work);
   RFI.name = "ForceCalculator";
      
   ret = RFI.Negotiate(RFI_StructureOfArrays, RFI_Rot);
   if (ret) return ret;
   
   RunForceCalculator(RFI);

   RFI.Close();

   MPI_Finalize();
   return 0;
}
