#include "RemoteForceInterface.hpp"

namespace rfi {
    template class RemoteForceInterface< double, ArrayOfStructures>;
    template class RemoteForceInterface< double, StructureOfArrays>;
    template class RemoteForceInterface< float, ArrayOfStructures>;
    template class RemoteForceInterface< float, StructureOfArrays>;
};