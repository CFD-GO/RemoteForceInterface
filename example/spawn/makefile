all: master slave

test: all master_single_fail_test slave_single_fail_test

%_single_fail_test : %
	@echo "--- Test $@ ---"
	! ./$<
	@echo "---------------"

master.o : master.cpp ../../RemoteForceInterface.hpp
slave.o : slave.cpp ../../RemoteForceInterface.hpp

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
