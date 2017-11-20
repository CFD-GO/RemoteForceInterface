
#include <mpi.h>
#include "../../RemoteForceInterface.hpp"
#include "ForceIntegrator.hpp"
#include "ForceCalculator.hpp"

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

   rfi::RemoteForceInterface< double, rfi::ForceCalculator > RFI(parent);
   
   RFI.name = "slave";
   
   if (! RFI.Connected() ) {
   /* This should never happen */
     ERROR("Not connected. Exit."); // LCOV_EXCL_LINE
     return -1; // LCOV_EXCL_LINE
   }
   
   ret = RFI.Negotiate(RFI_ArrayOfStructures, RFI_Rot);
   if (ret) return ret;

   RunForceCalculator(RFI);

   MPI_Finalize();
   return 0;
}
