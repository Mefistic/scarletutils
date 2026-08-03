#include "../includes.hpp"
#include <Geode/modify/ShaderLayer.hpp>

class $modify(ScarletShaderLayer, ShaderLayer) {
    void performCalculations() {
        if (layoutMode) return;

        ShaderLayer::performCalculations();
    }
};