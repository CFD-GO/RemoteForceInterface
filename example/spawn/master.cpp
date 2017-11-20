#include <mpi.h>
#include "../../RemoteForceInterface.hpp"



int main(int argc, char *argv[])
{
   int ret;
   MPI_Init(&argc, &argv);

   rfi::RemoteForceInterface< double > RFI( RFI_ForceCalculator, RFI_ArrayOfStructures | RFI_StructureOfArrays );
   
   ret = RFI.Spawn("./slave", MPI_ARGV_NULL);
   if (ret) return ret;
   
   while ( RFI.Active() ) {
       RFI.GetParticles();
       RFI.SetParticles();
   }
   
   RFI.Close();

   MPI_Finalize();
   return 0;
}
