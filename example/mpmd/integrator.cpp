#include <mpi.h>
#include "../../MPMD.hpp"
#include "../../RemoteForceInterface.hpp"
#include "../Common.hpp"

int main(int argc, char *argv[])
{
   int ret;
   MPMDHelper MPMD;
   rfi::RemoteForceInterface< rfi::ForceIntegrator, rfi::RotParticle, rfi::StructureOfArrays > RFI;

   MPI_Init(&argc, &argv);
   MPMD.Init(MPI_COMM_WORLD, "ForceIntegrator");
   MPMD.Identify();

   MPMDIntercomm inter = MPMD["ForceCalculator"];
   if (!inter) {
     ERROR("Didn't find ForceCalculator in MPMD\n");
     return -1;
   }

   ret = RFI.Connect(inter.work);
   if (ret) return ret;
   assert(RFI.Connected());
   
   RunForceIntegrator(RFI);
   
   RFI.Close();

   MPI_Finalize();
   return 0;
}