#include <mpi.h>
#include "../../MPMD.hpp"
#include "../../RemoteForceInterface.hpp"
#include "../Common.hpp"

int main(int argc, char *argv[])
{
   int ret;
   MPMDHelper MPMD;

   MPI_Init(&argc, &argv);
   MPMD.Init(MPI_COMM_WORLD, "SLAVE");

   MPMD.Identify();

   MPMDIntercomm inter = MPMD.parent;
   if (!inter) {
     ERROR("Didn't find parent in MPMD\n");
     return -1;
   }
   rfi::RemoteForceInterface< double, rfi::ForceIntegrator > RFI(inter.work);
   RFI.name = "ForceIntegrator";
   
   if (! RFI.Connected() ) {
   /* This should never happen */
     ERROR("Not connected. Exit."); // LCOV_EXCL_LINE
     return -1; // LCOV_EXCL_LINE
   }
   
   RFI.Negotiate( RFI_ArrayOfStructures | RFI_StructureOfArrays, RFI_Rot );

   RunForceIntegrator(RFI);
   
   RFI.Close();

   MPI_Finalize();
   return 0;
}
