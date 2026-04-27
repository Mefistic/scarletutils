#include "../includes.hpp"
#include <Geode/modify/CCCircleWave.hpp>

class $modify(ScarletUtilsCCCWHook, CCCircleWave) {
    void draw() {
        if (!noEffect)
            CCCircleWave::draw();
    }
};