#include <mpi.h>
#include "../../RemoteForceInterface.hpp"

int main(int argc, char *argv[])
{
   int ret;
   MPI_Init(&argc, &argv);

   rfi::RemoteForceInterface< double, rfi::ForceIntegrator > RFI;
   
   if (! RFI.Connected() ) {
     ERROR("I'm not a slave. Exit.");
     return -1;
   }
   
   RFI.Negotiate( RFI_ArrayOfStructures | RFI_StructureOfArrays, RFI_Rot );

   for (int i = 0; i < RFI.Workers(); i++) {
     RFI.Size(i)++;
   }
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
   
   while ( RFI.Active() ) {
       RFI.SetParticles();
       RFI.GetParticles();
   }
   
   RFI.Close();

   MPI_Finalize();
   return 0;
}
