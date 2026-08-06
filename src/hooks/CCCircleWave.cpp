#include "../includes.hpp"
#include <Geode/modify/CCCircleWave.hpp>

class $modify(ScarletCCCircleWave, CCCircleWave) {
    void draw() {
        if (!noEffect)
        CCCircleWave::draw();
    }
};