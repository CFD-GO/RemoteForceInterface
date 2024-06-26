
template <class rfi_class>
void PrintParticles(const char* str, rfi_class & RFI, MPMDHelper& MPMD) {
  for (int r=0; r < MPMD.work_size; r++) {
     MPI_Barrier(MPMD.work);
     if (r == MPMD.work_rank) {
       for (size_t i = 0; i < RFI.size(); i++) {
         printf("%s: %d: %ld:", str, MPMD.work_rank,i);
         for (int j=0; j<RFI.particle_size; j++) printf(" %f", RFI.getData(i, j));
         printf("\n");
       }
     }
  }
     MPI_Barrier(MPMD.work);
}

template <class rfi_class>
void RunForceCalculator(int maxiter, rfi_class & RFI, MPMDHelper& MPMD)
{
   double di = 100.0 * MPMD.work_rank;
   int active = 0;
   for (int iter = 0; iter < maxiter; iter++) {
    if (RFI.Active()) {
        RFI.SendSizes();
    }
    if (RFI.Active()) {
        active++;
        RFI.SendParticles();
        PrintParticles("PartRecv", RFI, MPMD);
        for (size_t i = 0; i < RFI.size(); i++) {
//          RFI.setData(i, RFI_DATA_VOL,       di+i+0.001);
          RFI.setData(i, RFI_DATA_FORCE+0,   di+i+0.002);
          RFI.setData(i, RFI_DATA_FORCE+1,   di+i+0.003);
          RFI.setData(i, RFI_DATA_FORCE+2,   di+i+0.004);
          if (RFI.Rot()) {
            RFI.setData(i, RFI_DATA_MOMENT+0, di+i+0.005);
            RFI.setData(i, RFI_DATA_MOMENT+1, di+i+0.006);
            RFI.setData(i, RFI_DATA_MOMENT+2, di+i+0.007);
          }
        }         
        RFI.SendForces();
    }
   }
   RFI.Close();
   printf("Executed %d iterations (%d active)\n", maxiter, active);
}

template <class rfi_class>
void RunForceIntegrator(int maxiter, rfi_class & RFI, MPMDHelper& MPMD)
{
   double di = 100.0 * MPMD.work_rank;
   for (int i = 0; i < RFI.Workers(); i++) {
     RFI.Size(i) = 3;
   }
   RFI.Alloc();
   for (size_t i = 0; i < RFI.size(); i++) {
     RFI.setData(i, RFI_DATA_R,      di+i+0.001);
     RFI.setData(i, RFI_DATA_POS+0,  di+i+0.002);
     RFI.setData(i, RFI_DATA_POS+1,  di+i+0.003);
     RFI.setData(i, RFI_DATA_POS+2,  di+i+0.004);
     RFI.setData(i, RFI_DATA_VEL+0,  di+i+0.005);
     RFI.setData(i, RFI_DATA_VEL+1,  di+i+0.006);
     RFI.setData(i, RFI_DATA_VEL+2,  di+i+0.007);
     if (RFI.Rot()) {
       RFI.setData(i, RFI_DATA_ANGVEL+0,  i+0.008);
       RFI.setData(i, RFI_DATA_ANGVEL+1,  i+0.009);
       RFI.setData(i, RFI_DATA_ANGVEL+2,  i+0.010);
     }
   }
   
   int active = 0;
   for (int iter = 0; iter < maxiter; iter++) {
       if ( RFI.Active() ) {
         RFI.SendSizes();
         PrintParticles("PartSend", RFI, MPMD);
         RFI.SendParticles();
         RFI.SendForces();
         if ( RFI.Active() ) {
          // Force can be used;
          active++;
         }          
       }
   }
   
   RFI.Close();
   printf("Executed %d iterations (%d active)\n", maxiter, active);
}
