
all : RemoteForceInterface.o

RemoteForceInterface.o : RemoteForceInterface.cpp RemoteForceInterface.hpp RemoteForceInterface.h

%.o : %.cpp
	mpic++ -c -o $@ $<
