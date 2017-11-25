
template <class rfi_class>
void RunForceCalculator(rfi_class & RFI)
{
   while (RFI.Active()) {
       RFI.SendSizes();
       RFI.SendParticles();
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
       RFI.SendForces();
   }
   RFI.Close();
}

template <class rfi_class>
void RunForceIntegrator(rfi_class & RFI)
{
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
   
   for (int iter = 0; iter < 3; iter++) {
       if ( ! RFI.Active() ) break;
       RFI.SendSizes();
       RFI.SendParticles();
       RFI.SendForces();
   }
   
   RFI.Close();
}
