all: master slave

test: all master_single_fail_test slave_single_fail_test master_1_1_test master_2_1_test master_2_2_test master_1_2_test

%_single_fail_test : %
	@echo "--- Test $@ ---"
	! ./$<
	@echo

master_1_1_test : master
	@echo "--- Test $@ ---"
	timeout 5s ./run 1 1
	@echo

master_2_1_test : master
	@echo "--- Test $@ ---"
	timeout 5s ./run 2 1
	@echo

master_2_2_test : master
	@echo "--- Test $@ ---"
	timeout 5s ./run 2 2
	@echo

master_1_2_test : master
	@echo "--- Test $@ ---"
	timeout 5s ./run 1 2
	@echo

master.o : master.cpp ../../RemoteForceInterface.hpp
slave.o : slave.cpp ../../RemoteForceInterface.hpp

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
