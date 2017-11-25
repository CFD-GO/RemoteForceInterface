#include <mpi.h>
#include "../../MPMD.hpp"
#include "../../RemoteForceInterface.hpp"

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

   for (int i = 0; i < RFI.Workers(); i++) {
     RFI.Size(i)++;
   }
   RFI.Alloc();
   for (int i = 0; i < RFI.Workers(); i++) {
     RFI.SetData(i, RFI_DATA_R, 1.0);
     RFI.SetData(i, RFI_DATA_POS+0,  0.0);
     RFI.SetData(i, RFI_DATA_POS+1,  0.0);
     RFI.SetData(i, RFI_DATA_POS+2,  0.0);
     RFI.SetData(i, RFI_DATA_VEL+0,  0.0);
     RFI.SetData(i, RFI_DATA_VEL+1,  0.0);
     RFI.SetData(i, RFI_DATA_VEL+2,  0.0);
     if (RFI.Rot()) {
       RFI.SetData(i, RFI_DATA_ANGVEL+0,  0.0);
       RFI.SetData(i, RFI_DATA_ANGVEL+1,  0.0);
       RFI.SetData(i, RFI_DATA_ANGVEL+2,  0.0);
     }
   }
   
   for (int iter = 0; iter < 10; iter++) {
       if ( ! RFI.Active() ) break;
       RFI.SendSizes();
       RFI.SendParticles();
       RFI.SendForces();
   }
   
   RFI.Close();

   MPI_Finalize();
   return 0;
}
