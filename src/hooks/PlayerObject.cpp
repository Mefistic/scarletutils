#include "../includes.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsPObjHook, PlayerObject) {
    void spawnPortalCircle(ccColor3B color, float startRadius) {
        if (noEffect)
            return;

        PlayerObject::spawnPortalCircle(color, startRadius);
    }

    void propellPlayer(float yVelocity, bool noEffects, int objectType) {
        if (objectType == 10) {
            if (!clickedJumpPad && clickGravityPads) {
                GJBaseGameLayer::get()->queueButton(
                    1,
                    true,
                    this->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
                clickedJumpPad = true;
            }
        }
        PlayerObject::propellPlayer(yVelocity, noEffects, objectType);
    }

    void bumpPlayer(float bumpMod, int objectType, bool noEffects, GameObject* object) {
        if (objectType == 9 || objectType == 8 || objectType == 34) {
            if (!clickedJumpPad && clickJumpPads) {
                GJBaseGameLayer::get()->queueButton(
                    1,
                    true,
                    this->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
                clickedJumpPad = true;
            }
        }
        PlayerObject::bumpPlayer(bumpMod, objectType, noEffects, object);
    }

    void playDeathEffect() {
        if (noDeathEffect)
            return;

        PlayerObject::playDeathEffect();
    }

    void stopDashing() {
        if (noEffect)
            m_dashFireSprite->setScale(0.f);

        PlayerObject::stopDashing();
    }
    void playSpiderDashEffect(CCPoint from, CCPoint to) {
        if (noEffect)
            return;

        PlayerObject::playSpiderDashEffect(from, to);
    }
};