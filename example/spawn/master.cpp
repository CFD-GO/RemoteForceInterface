#include <mpi.h>
#include "../../RemoteForceInterface.hpp"



int main(int argc, char *argv[])
{
   int ret;
   MPI_Init(&argc, &argv);

   rfi::RemoteForceInterface< double > RFI( RFI_ForceCalculator, RFI_ArrayOfStructures | RFI_StructureOfArrays, RFI_Rot );
   
   ret = RFI.Spawn("./slave", MPI_ARGV_NULL);
   if (ret) return ret;
   
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
