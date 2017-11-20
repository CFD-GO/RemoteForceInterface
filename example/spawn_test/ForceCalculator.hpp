
template <class rfi_class>
void RunForceCalculator(rfi_class & RFI)
{
   for (int iter = 0; iter < 5; iter++) {
       if ( ! RFI.Active() ) break;
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
