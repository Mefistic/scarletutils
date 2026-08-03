#include "../includes.hpp"
#include <Geode/modify/EffectGameObject.hpp>

using namespace geode::prelude;

class $modify(ScarletEffectGameObject, EffectGameObject) {
    void playTriggerEffect() {
        if (noEffect)
            return;

        EffectGameObject::playTriggerEffect();
    }
};