#include <mpi.h>
#include "../../RemoteForceInterface.hpp"

int main(int argc, char *argv[])
{
   int ret;
   MPI_Init(&argc, &argv);

   MPI_Comm parent;

   MPI_Comm_get_parent(&parent);
   if (parent == MPI_COMM_NULL) {
     error("No parent!\n");
     return -1;
   }

   rfi::RemoteForceInterface< double, rfi::ForceIntegrator > RFI(parent);
   RFI.name = "slave";
   
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
