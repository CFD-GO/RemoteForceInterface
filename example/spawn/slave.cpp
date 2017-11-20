#include <mpi.h>
#include "../../RemoteForceInterface.hpp"

int main(int argc, char *argv[])
{
   int ret;
   MPI_Init(&argc, &argv);

   rfi::RemoteForceInterface< double > RFI( RFI_ForceIntegrator, RFI_ArrayOfStructures | RFI_StructureOfArrays, RFI_Rot );
   
   if (! RFI.CheckIfSlave() ) {
     ERROR("I'm not a slave. Exit.");
     return -1;
   }

   for (int i = 0; i < RFI.workers; i++) {
     RFI.Size(i)++;
   }
   for (int i = 0; i < RFI.workers; i++) {
     RFI.SetData(i, RFI_DATA_R, 1.0);
     RFI.SetData(i, RFI_POS+0,  0.0);
     RFI.SetData(i, RFI_POS+1,  0.0);
     RFI.SetData(i, RFI_POS+2,  0.0);
     RFI.SetData(i, RFI_VEL+0,  0.0);
     RFI.SetData(i, RFI_VEL+1,  0.0);
     RFI.SetData(i, RFI_VEL+2,  0.0);
     if (RFI.Rot()) {
       RFI.SetData(i, RFI_ANGVEL+0,  0.0);
       RFI.SetData(i, RFI_ANGVEL+1,  0.0);
       RFI.SetData(i, RFI_ANGVEL+2,  0.0);
     }
   }
   
   while ( RFI.Active() ) {
       RFI.GetParticles();
       for (size_t i = 0; i < RFI.size(); i++) {
         RFI.SetData(i, RFI_DATA_VOL,       1.0);
         RFI.SetData(i, RFI_DATA_FORCE+0,   0.0);
         RFI.SetData(i, RFI_DATA_FORCE+1, -9.81);
         RFI.SetData(i, RFI_DATA_FORCE+2,   0.0);
         if (RFI.Rot()) {
           RFI.SetData(i, RFI_DATA_MOMENT+0, 0.0);
           RFI.SetData(i, RFI_DATA_MOMENT+1, 0.0);
           RFI.SetData(i, RFI_DATA_MOMENT+2, 0.0);
         }
       }         
       RFI.SetParticles();
   }
   
   RFI.Close();

   MPI_Finalize();
   return 0;
}
