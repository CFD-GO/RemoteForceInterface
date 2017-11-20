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
