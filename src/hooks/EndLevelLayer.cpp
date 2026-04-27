#include "../includes.hpp"
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsELLHook, EndLevelLayer) {
    void showLayer(bool instant) {
        if (hideEndscreen)
            return;

        EndLevelLayer::showLayer(instant);
    }
};