
#include <mpi.h>
#include "../../RemoteForceInterface.hpp"
#include "ForceIntegrator.hpp"
#include "ForceCalculator.hpp"

int main(int argc, char *argv[])
{
   int ret;
   MPI_Init(&argc, &argv);

   rfi::RemoteForceInterface< float, rfi::ForceIntegrator > RFI;
   RFI.name = "master";
      
   ret = RFI.Spawn("./slave", MPI_ARGV_NULL);
   if (ret) return ret;
   
   ret = RFI.Negotiate(RFI_StructureOfArrays, RFI_Rot);

   if (ret) return ret;
   
   RunForceIntegrator(RFI);

   MPI_Finalize();
   return 0;
}
