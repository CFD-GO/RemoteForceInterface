#!/bin/bash

Rscript tab.R
RT -f master.cpp.Rt
RT -f slave.cpp.Rt
RT -xf makefile.Rt
