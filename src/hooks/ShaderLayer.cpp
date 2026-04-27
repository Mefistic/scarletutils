#include "../includes.hpp"
#include <Geode/modify/ShaderLayer.hpp>

class $modify(ScarletUtilsSLHook, ShaderLayer) {
    void performCalculations() {
        if (layoutMode) return;

        ShaderLayer::performCalculations();
    }
};