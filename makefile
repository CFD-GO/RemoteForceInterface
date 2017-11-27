
all : RemoteForceInterface.o

test : all example/spawn/test example/mpmd/test

%/test :
	@echo Running test for $(dir $@)
	$(MAKE) test -C $(dir $@)

RemoteForceInterface.o : RemoteForceInterface.cpp RemoteForceInterface.hpp RemoteForceInterface.h

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
