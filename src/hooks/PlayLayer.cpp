#include "../includes.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsPLHook, PlayLayer) {
    void addObject(GameObject* object) {
        if (!layoutMode || !PlayLayer::get())
            return PlayLayer::addObject(object);

        if (filter.contains(object->m_objectID)) {
            object->m_isHide = true;
            return;
        }

        object->m_hasNoGlow = true;
        object->m_isHide = false;
        object->m_hasNoAudioScale = false;
        object->m_detailUsesHSV = false;
        object->m_baseUsesHSV = false;
        object->m_activeDetailColorID = -1;
        object->m_activeMainColorID = -1;
        object->m_isDontEnter = true;
        object->m_isDontFade = true;
        object->m_ignoreFade = true;
        object->m_ignoreEnter = true;
        object->m_hasParticles = false;
        object->m_hasNoParticles = true;
        object->m_hasNoEffects = false;
        object->setOpacity(255);

        if (noEffect && object->m_objectType != GameObjectType::InverseMirrorPortal && object->m_objectType != GameObjectType::NormalMirrorPortal)
            object->m_hasNoEffects = true;

        PlayLayer::addObject(object);
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (player->m_isDead || object == m_anticheatSpike)
            return PlayLayer::destroyPlayer(player, object);

        if (noclip) {
            if (noclipP1 && player == m_player1)
                return;
            if (noclipP2 && player == m_player2)
                return;
        }

        PlayLayer::destroyPlayer(player, object);
        bool which = player->m_isSecondPlayer ^ GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls);

        autoclickerHoldingP1 = false;
        autoclickerTimerP1 = INT_MAX;
        autoclickerHoldingP2 = false;
        autoclickerTimerP2 = INT_MAX;
    }

    void applyStartFade() {
        if (fadeLevel && fadeLevelInDuration > 0) {
            if (m_startFadeLayer) return;

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            m_startFadeLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), winSize.width, winSize.height);

            this->addChild(m_startFadeLayer, 100);

            auto fadeAction = CCFadeOut::create(fadeLevelInDuration);
            auto removeAction = CCRemoveSelf::create();
            m_startFadeLayer->runAction(CCSequence::create(fadeAction, removeAction, nullptr));

            if (fadeAudio && fadeAudioInDuration > 0) {
                FMODAudioEngine::sharedEngine()->fadeMusic(fadeAudioInDuration, 0, 0.0f, 1.f);
            }
        }
    }

    void onEnterTransitionDidFinish() {
        if (layoutMode) GJBaseGameLayer::get()->toggleGlitter(false);

        PlayLayer::onEnterTransitionDidFinish();
    }

    void startGame() {
        PlayLayer::startGame();
        this->applyStartFade();

        if (restartFirstFrame) {
            geode::queueInMainThread([this]{ PlayLayer::get()->resetLevel(); });
        }
    }

    void showEndLayer() {
        PlayLayer::showEndLayer();

        if (fadeLevel && fadeLevelOutDuration > 0) {
            if (m_endFadeLayer)
                return;

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            m_endFadeLayer = CCLayerColor::create(ccc4(0, 0, 0 , 255), winSize.width, winSize.height);

            this->addChild(m_endFadeLayer, 1000);

            auto fadeIn = CCFadeIn::create(fadeLevelOutDuration);

            m_endFadeLayer->runAction(fadeIn);

            if (fadeAudio && fadeAudioOutDuration > 0) {
                FMODAudioEngine::sharedEngine()->fadeMusic(fadeAudioOutDuration, 0, 1.0f, 0.0f);
            }
        }
    }

    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
        if (hideNewBest)
            return;

        PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
    }

    void playGravityEffect(bool flip) {
        if (noEffect)
            return;

        PlayLayer::playGravityEffect(flip);
    }
};