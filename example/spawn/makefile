all: master

test: all


master.o : master.cpp ../../RemoteForceInterface.hpp
slave.o : slave.cpp ../../RemoteForceInterface.hpp

% : %.o
	mpic++ -o $@ $^

%.o : %.cpp
	mpic++ -c -o $@ $<
