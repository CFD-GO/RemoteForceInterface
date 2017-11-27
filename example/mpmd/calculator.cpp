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
   MPMD.Init(MPI_COMM_WORLD, "ForceCalculator");
   MPMD.Identify();

   MPMDIntercomm inter = MPMD["ForceIntegrator"];
   if (!inter) {
     ERROR("Didn't find ForceIntegrator in MPMD\n");
     return -1;
   }

   ret = RFI.Connect(inter.work);
   if (ret) return ret;
   assert(RFI.Connected());
   
   RunForceCalculator(RFI);

   RFI.Close();

   MPI_Finalize();
   return 0;
}
