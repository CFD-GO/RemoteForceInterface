#include "RemoteForceInterface.hpp"

namespace rfi {
    template class RemoteForceInterface< double, ForceCalculator >;
    template class RemoteForceInterface< double, ForceIntegrator >;
    template class RemoteForceInterface< float, ForceCalculator >;
    template class RemoteForceInterface< float, ForceIntegrator >;
};