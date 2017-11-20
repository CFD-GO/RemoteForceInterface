#!/usr/bin/env Rscript

tab = expand.grid(real_t=c("double","float"), rot=c("NRot","Rot"), storage=c("SoA","AoS"),
type=c("ForceCalculator","ForceIntegrator"));
tab$storage_flag = c(SoA="RFI_StructureOfArrays", AoS="RFI_ArrayOfStructures")[as.character(tab$storage)]
tab$stub = paste(tab$type, tab$real_t, tab$rot, tab$storage, sep="_")
write.csv(tab, file="tab.csv", row.names=FALSE)
