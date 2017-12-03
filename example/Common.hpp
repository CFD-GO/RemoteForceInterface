
template <class rfi_class>
void PrintParticles(const char* str, rfi_class & RFI, MPMDHelper& MPMD) {
  for (int r=0; r < MPMD.work_size; r++) {
     MPI_Barrier(MPMD.work);
     if (r == MPMD.work_rank) {
       for (size_t i = 0; i < RFI.size(); i++) {
         printf("%s: %d: %ld:", str, MPMD.work_rank,i);
         for (int j=0; j<RFI.particle_size; j++) printf(" %f", RFI.Data(i, j));
         printf("\n");
       }
     }
  }
     MPI_Barrier(MPMD.work);
}

template <class rfi_class>
void RunForceCalculator(rfi_class & RFI, MPMDHelper& MPMD)
{
   double di = 100.0 * MPMD.work_rank;
   while (RFI.Active()) {
       RFI.SendSizes();
       RFI.SendParticles();
       PrintParticles("PartRecv", RFI, MPMD);
       for (size_t i = 0; i < RFI.size(); i++) {
         RFI.SetData(i, RFI_DATA_VOL,       di+i+0.001);
         RFI.SetData(i, RFI_DATA_FORCE+0,   di+i+0.002);
         RFI.SetData(i, RFI_DATA_FORCE+1,   di+i+0.003);
         RFI.SetData(i, RFI_DATA_FORCE+2,   di+i+0.004);
         if (RFI.Rot()) {
           RFI.SetData(i, RFI_DATA_MOMENT+0, di+i+0.005);
           RFI.SetData(i, RFI_DATA_MOMENT+1, di+i+0.006);
           RFI.SetData(i, RFI_DATA_MOMENT+2, di+i+0.007);
         }
       }         
       RFI.SendForces();
   }
   RFI.Close();
}

template <class rfi_class>
void RunForceIntegrator(rfi_class & RFI, MPMDHelper& MPMD)
{
   double di = 100.0 * MPMD.work_rank;
   for (int i = 0; i < RFI.Workers(); i++) {
     RFI.Size(i) = 3;
   }
   RFI.Alloc();
   for (size_t i = 0; i < RFI.size(); i++) {
     RFI.SetData(i, RFI_DATA_R,      di+i+0.001);
     RFI.SetData(i, RFI_DATA_POS+0,  di+i+0.002);
     RFI.SetData(i, RFI_DATA_POS+1,  di+i+0.003);
     RFI.SetData(i, RFI_DATA_POS+2,  di+i+0.004);
     RFI.SetData(i, RFI_DATA_VEL+0,  di+i+0.005);
     RFI.SetData(i, RFI_DATA_VEL+1,  di+i+0.006);
     RFI.SetData(i, RFI_DATA_VEL+2,  di+i+0.007);
     if (RFI.Rot()) {
       RFI.SetData(i, RFI_DATA_ANGVEL+0,  i+0.008);
       RFI.SetData(i, RFI_DATA_ANGVEL+1,  i+0.009);
       RFI.SetData(i, RFI_DATA_ANGVEL+2,  i+0.010);
     }
   }
   
   for (int iter = 0; iter < 2; iter++) {
       if ( ! RFI.Active() ) break;
       RFI.SendSizes();
       PrintParticles("PartSend", RFI, MPMD);
       RFI.SendParticles();
       RFI.SendForces();
   }
   
   RFI.Close();
}
