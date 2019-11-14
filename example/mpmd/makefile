all: calculator integrator

test: all calculator_single_fail_test integrator_single_fail_test calculator_1_1_test calculator_2_1_test calculator_2_2_test calculator_1_2_test

%_single_fail_test : %
	@echo "--- Test $@ ---"
	! ./$<
	@echo

calculator_1_1_test : calculator
	@echo "--- Test $@ ---"
	$(TEST) ./run 1 3 1 3
	@echo

calculator_2_1_test : calculator
	@echo "--- Test $@ ---"
	$(TEST) ./run 2 3 1 3
	@echo

calculator_2_2_test : calculator
	@echo "--- Test $@ ---"
	$(TEST) ./run 2 3 2 3
	@echo

calculator_1_2_test : calculator
	@echo "--- Test $@ ---"
	$(TEST) ./run 1 3 2 3
	@echo

calculator.o : calculator.cpp ../../RemoteForceInterface.hpp ../../MPMD.hpp ../Common.hpp
integrator.o : integrator.cpp ../../RemoteForceInterface.hpp ../../MPMD.hpp ../Common.hpp

% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
