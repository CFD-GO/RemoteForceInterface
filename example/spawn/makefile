all: master

test: all
	! ./master


master.o : master.cpp ../../RemoteForceInterface.hpp
slave.o : slave.cpp ../../RemoteForceInterface.hpp

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
