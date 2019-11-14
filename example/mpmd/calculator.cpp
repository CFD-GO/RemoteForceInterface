#include <mpi.h>
#include "../../MPMD.hpp"
// #define debug1 printf
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

   int iter = 3;
   if (argc > 1) iter = atoi(argv[1]);

   MPMDIntercomm inter = MPMD["ForceIntegrator"];
   if (!inter) {
     fprintf(stderr,"Didn't find ForceIntegrator in MPMD\n");
     return -1;
   }

   ret = RFI.Connect(MPMD.work,inter.work);
   if (ret) return ret;
   assert(RFI.Connected());
   
   RunForceCalculator(iter, RFI, MPMD);

   RFI.Close();

   MPI_Finalize();
   return 0;
}
