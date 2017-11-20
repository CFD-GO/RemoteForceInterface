
all: master_ForceCalculator_double_NRot_SoA master_ForceCalculator_float_NRot_SoA master_ForceCalculator_double_Rot_SoA master_ForceCalculator_float_Rot_SoA master_ForceCalculator_double_NRot_AoS master_ForceCalculator_float_NRot_AoS master_ForceCalculator_double_Rot_AoS master_ForceCalculator_float_Rot_AoS master_ForceIntegrator_double_NRot_SoA master_ForceIntegrator_float_NRot_SoA master_ForceIntegrator_double_Rot_SoA master_ForceIntegrator_float_Rot_SoA master_ForceIntegrator_double_NRot_AoS master_ForceIntegrator_float_NRot_AoS master_ForceIntegrator_double_Rot_AoS master_ForceIntegrator_float_Rot_AoS slave_ForceCalculator_double_NRot_SoA slave_ForceCalculator_float_NRot_SoA slave_ForceCalculator_double_Rot_SoA slave_ForceCalculator_float_Rot_SoA slave_ForceCalculator_double_NRot_AoS slave_ForceCalculator_float_NRot_AoS slave_ForceCalculator_double_Rot_AoS slave_ForceCalculator_float_Rot_AoS slave_ForceIntegrator_double_NRot_SoA slave_ForceIntegrator_float_NRot_SoA slave_ForceIntegrator_double_Rot_SoA slave_ForceIntegrator_float_Rot_SoA slave_ForceIntegrator_double_NRot_AoS slave_ForceIntegrator_float_NRot_AoS slave_ForceIntegrator_double_Rot_AoS slave_ForceIntegrator_float_Rot_AoS

test: all


master_ForceCalculator_double_NRot_SoA.o : master_ForceCalculator_double_NRot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_double_NRot_SoA.o : slave_ForceCalculator_double_NRot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_float_NRot_SoA.o : master_ForceCalculator_float_NRot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_float_NRot_SoA.o : slave_ForceCalculator_float_NRot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_double_Rot_SoA.o : master_ForceCalculator_double_Rot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_double_Rot_SoA.o : slave_ForceCalculator_double_Rot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_float_Rot_SoA.o : master_ForceCalculator_float_Rot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_float_Rot_SoA.o : slave_ForceCalculator_float_Rot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_double_NRot_AoS.o : master_ForceCalculator_double_NRot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_double_NRot_AoS.o : slave_ForceCalculator_double_NRot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_float_NRot_AoS.o : master_ForceCalculator_float_NRot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_float_NRot_AoS.o : slave_ForceCalculator_float_NRot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_double_Rot_AoS.o : master_ForceCalculator_double_Rot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_double_Rot_AoS.o : slave_ForceCalculator_double_Rot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceCalculator_float_Rot_AoS.o : master_ForceCalculator_float_Rot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceCalculator_float_Rot_AoS.o : slave_ForceCalculator_float_Rot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_double_NRot_SoA.o : master_ForceIntegrator_double_NRot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_double_NRot_SoA.o : slave_ForceIntegrator_double_NRot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_float_NRot_SoA.o : master_ForceIntegrator_float_NRot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_float_NRot_SoA.o : slave_ForceIntegrator_float_NRot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_double_Rot_SoA.o : master_ForceIntegrator_double_Rot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_double_Rot_SoA.o : slave_ForceIntegrator_double_Rot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_float_Rot_SoA.o : master_ForceIntegrator_float_Rot_SoA.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_float_Rot_SoA.o : slave_ForceIntegrator_float_Rot_SoA.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_double_NRot_AoS.o : master_ForceIntegrator_double_NRot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_double_NRot_AoS.o : slave_ForceIntegrator_double_NRot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_float_NRot_AoS.o : master_ForceIntegrator_float_NRot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_float_NRot_AoS.o : slave_ForceIntegrator_float_NRot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_double_Rot_AoS.o : master_ForceIntegrator_double_Rot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_double_Rot_AoS.o : slave_ForceIntegrator_double_Rot_AoS.cpp ../../RemoteForceInterface.hpp


master_ForceIntegrator_float_Rot_AoS.o : master_ForceIntegrator_float_Rot_AoS.cpp ../../RemoteForceInterface.hpp
slave_ForceIntegrator_float_Rot_AoS.o : slave_ForceIntegrator_float_Rot_AoS.cpp ../../RemoteForceInterface.hpp



% : %.o
	mpic++ -o $@ $(CXXFLAGS) $^

%.o : %.cpp
	mpic++ -c -o $@ $(CXXFLAGS) $<
