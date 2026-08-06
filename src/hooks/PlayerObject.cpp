#include "../includes.hpp"
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
        if (noEffect) {
            auto og = this->m_playEffects;
            this->m_playEffects = false;
            PlayerObject::startDashing(object);
            this->m_playEffects = og;
            return;
        } else
        PlayerObject::startDashing(object);
    }

    void stopDashing() {
        if (noEffect) {
            auto og = this->m_maybeReducedEffects;
            this->m_maybeReducedEffects = true;
            PlayerObject::stopDashing();
            this->m_maybeReducedEffects = og;
            return;
        }
        PlayerObject::stopDashing();
    }

    void ringJump(RingObject* object, bool skipCheck) {
        if (noEffect) {
            auto og = this->m_playEffects;
            this->m_playEffects = true;
            PlayerObject::ringJump(object, skipCheck);
            this->m_playEffects = og;
            return;
        }
        PlayerObject::ringJump(object, skipCheck);
    }

    void playSpiderDashEffect(CCPoint from, CCPoint to) {
        if (!noEffect)
        PlayerObject::playSpiderDashEffect(from, to);
    }
};