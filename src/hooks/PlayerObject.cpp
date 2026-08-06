#include "../includes.hpp"
#include <Geode/Enums.hpp>
#include <Geode/binding/RingObject.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(ScarletPlayerObject, PlayerObject) {
    void spawnPortalCircle(ccColor3B color, float startRadius) {
        if (!noEffect)
        PlayerObject::spawnPortalCircle(color, startRadius);
    }

    void propellPlayer(float yVelocity, bool noEffects, int objectType) {
        if (objectType == 10) {
            if (!clickedJumpPad && clickGravityPads) {
                GJBaseGameLayer::get()->queueButton((int)PlayerButton::Jump, true,
                this->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(
                GameVar::Flip2PlayerControls), 0.0);
                GJBaseGameLayer::get()->queueButton((int)PlayerButton::Jump, false,
                this->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(
                GameVar::Flip2PlayerControls), 0.0);
                clickedJumpPad = true;
            }
        }
        PlayerObject::propellPlayer(yVelocity, noEffects, objectType);
    }

    void bumpPlayer(float bumpMod, int objectType, bool noEffects, GameObject* object) {
        if (objectType == 9 || objectType == 8 || objectType == 34) {
            if (!clickedJumpPad && clickJumpPads) {
                GJBaseGameLayer::get()->queueButton((int)PlayerButton::Jump, true,
                this->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(
                GameVar::Flip2PlayerControls), 0.0);
                GJBaseGameLayer::get()->queueButton((int)PlayerButton::Jump, false,
                this->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(
                GameVar::Flip2PlayerControls), 0.0);
                clickedJumpPad = true;
            }
        }
        PlayerObject::bumpPlayer(bumpMod, objectType, noEffects, object);
    }

    void playDeathEffect() {
        if (!noDeathEffect)
        PlayerObject::playDeathEffect();
    }

    void startDashing(DashRingObject* object) {
        static int lastOrb;
        if (noEffect || (optimizeStackedOrbs && optimizeStartDashing && lastOrb == object->m_objectID)) {
            auto og = this->m_playEffects;
            this->m_playEffects = false;
            PlayerObject::startDashing(object);
            this->m_playEffects = og;
        }
        else PlayerObject::startDashing(object);
        optimizeStartDashing = true;
        lastOrb = object->m_objectID;
    }

    void stopDashing() {
        if (noEffect || (optimizeStackedOrbs && optimizeStopDashing)) {
            auto og = this->m_maybeReducedEffects;
            this->m_maybeReducedEffects = true;
            PlayerObject::stopDashing();
            this->m_maybeReducedEffects = og;
        }
        else PlayerObject::stopDashing();
        optimizeStopDashing = true;
    }

    void playSpiderDashEffect(CCPoint from, CCPoint to) {
        if (!noEffect)
        PlayerObject::playSpiderDashEffect(from, to);
    }
};