#include "../includes.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsGJBGLHook, GJBaseGameLayer) {
    void runMaintainGravity() {
        bool p1maintain = m_player1->m_holdingButtons[1] != m_player1->m_isUpsideDown;
        bool p2maintain = m_player2->m_holdingButtons[1] != m_player2->m_isUpsideDown;

        bool p1holding = m_uiLayer->m_p1Jumping;
        bool p2holding = m_uiLayer->m_p2Jumping;

        if (GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls))
            std::swap(p1holding, p2holding);

        if (maintainGravity) {
            if (maintainGravityP1)
                m_queuedButtons.erase(
                    std::remove_if(
                        m_queuedButtons.begin(), m_queuedButtons.end(),
                        [](auto x) {
                            return !(x.m_isPlayer2 ^
                                     GameManager::sharedState()->getGameVariable(
                                         GameVar::Flip2PlayerControls));
                        }
                    ),
                    m_queuedButtons.end()
                );

            if ((p1holding || (autoclickerHoldingP1 && autoclickerP1)) != p1maintain &&
                maintainGravityP1) {
                queueButton(
                    1, !m_player1->m_holdingButtons[1],
                    GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
                autoclickerTimerP1 = INT32_MAX;
            }

            if (maintainGravityP2)
                m_queuedButtons.erase(
                    std::remove_if(
                        m_queuedButtons.begin(), m_queuedButtons.end(),
                        [](auto x) {
                            return (x.m_isPlayer2 ^
                                    GameManager::sharedState()->getGameVariable(
                                        GameVar::Flip2PlayerControls));
                        }
                    ),
                    m_queuedButtons.end()
                );

            if ((p2holding || (autoclickerHoldingP2 && autoclickerP2)) != p2maintain &&
                maintainGravityP2 && m_gameState.m_isDualMode &&
                m_levelSettings->m_twoPlayerMode) {
                queueButton(
                    1, !m_player2->m_holdingButtons[1],
                    !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
                autoclickerTimerP2 = INT32_MAX;
            }
        }
    }

    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        if (layoutMode)
            toggleGlitter(false);
        clickedJumpPad = false;

        if (straightUfo) {
            if (straightUfoP1 &&
                ((m_player1->getPositionY() < straightUfoTargetP1 - straightUfoThresholdP1 &&
                  m_player1->m_yVelocity <= 0 && !m_player1->m_isUpsideDown) ||
                 (m_player1->getPositionY() > straightUfoTargetP1 + straightUfoThresholdP1 &&
                  m_player1->m_yVelocity > 0 && !m_player1->m_isUpsideDown) ||
                 (m_player1->getPositionY() > straightUfoTargetP1 + straightUfoThresholdP1 &&
                  m_player1->m_yVelocity >= 0 && m_player1->m_isUpsideDown) ||
                 (m_player1->getPositionY() < straightUfoTargetP1 - straightUfoThresholdP1 &&
                  m_player1->m_yVelocity < 0 && m_player1->m_isUpsideDown))) {
                queueButton(
                    1, false,
                    GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
                queueButton(
                    1, true,
                    GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
            }

            if (straightUfoP2 &&
                ((m_player2->getPositionY() < straightUfoTargetP2 - straightUfoThresholdP2 &&
                  m_player2->m_yVelocity <= 0 && !m_player2->m_isUpsideDown) ||
                 (m_player2->getPositionY() > straightUfoTargetP2 + straightUfoThresholdP2 &&
                  m_player2->m_yVelocity > 0 && !m_player2->m_isUpsideDown) ||
                 (m_player2->getPositionY() > straightUfoTargetP2 + straightUfoThresholdP2 &&
                  m_player2->m_yVelocity >= 0 && m_player2->m_isUpsideDown) ||
                 (m_player2->getPositionY() < straightUfoTargetP2 - straightUfoThresholdP2 &&
                  m_player2->m_yVelocity < 0 && m_player2->m_isUpsideDown))) {
                queueButton(
                    1, false,
                    !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
                queueButton(
                    1, true,
                    !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
            }
        }

        if (straightFly) {
            if (straightFlyP1 &&
                ((m_player1->getYVelocity() < -straightFlyThresholdP1 &&
                  !m_player1->m_holdingButtons[1] && !m_player1->m_isUpsideDown) ||
                 (m_player1->getYVelocity() > straightFlyThresholdP1 &&
                  m_player1->m_holdingButtons[1] && !m_player1->m_isUpsideDown) ||
                 (m_player1->getYVelocity() > straightFlyThresholdP1 &&
                  !m_player1->m_holdingButtons[1] && m_player1->m_isUpsideDown) ||
                 (m_player1->getYVelocity() < -straightFlyThresholdP1 &&
                  m_player1->m_holdingButtons[1] && m_player1->m_isUpsideDown)))
                queueButton(
                    1, !m_player1->m_holdingButtons[1],
                    GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );

            if (straightFlyP2 &&
                ((m_player2->getYVelocity() < -straightFlyThresholdP2 &&
                  !m_player2->m_holdingButtons[1] && !m_player2->m_isUpsideDown) ||
                 (m_player2->getYVelocity() > straightFlyThresholdP2 &&
                  m_player2->m_holdingButtons[1] && !m_player2->m_isUpsideDown) ||
                 (m_player2->getYVelocity() > straightFlyThresholdP2 &&
                  !m_player2->m_holdingButtons[1] && m_player2->m_isUpsideDown) ||
                 (m_player2->getYVelocity() < -straightFlyThresholdP2 &&
                  m_player2->m_holdingButtons[1] && m_player2->m_isUpsideDown)))
                queueButton(
                    1, !m_player2->m_holdingButtons[1],
                    !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                    0.0
                );
        }

        if (autoclickerP1) {
            if (autoclickerHoldingP1 && autoclickerTimerP1 >= autoclickerHoldP1) {
                if (!autoclickerSwiftP1)
                    queueButton(
                        1, false,
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                else {
                    queueButton(
                        1, true,
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                    queueButton(
                        1, false,
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                }
                autoclickerHoldingP1 = false;
                autoclickerTimerP1 = 0;
            }
            if (!autoclickerHoldingP1 && autoclickerTimerP1 >= autoclickerEveryP1) {
                if (!autoclickerSwiftP1)
                    queueButton(
                        1, true,
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                else {
                    queueButton(
                        1, true,
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                    queueButton(
                        1, false,
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                }
                autoclickerHoldingP1 = true;
                autoclickerTimerP1 = 0;
            }
            autoclickerTimerP1++;
        }
        if (autoclickerP2) {
            if (autoclickerHoldingP2 && autoclickerTimerP2 >= autoclickerHoldP2) {
                if (!autoclickerSwiftP2)
                    queueButton(
                        1, false,
                        !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                else {
                    queueButton(
                        1, true,
                        !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                    queueButton(
                        1, false,
                        !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                }
                autoclickerHoldingP2 = false;
                autoclickerTimerP2 = 0;
            }
            if (!autoclickerHoldingP2 && autoclickerTimerP2 >= autoclickerEveryP2) {
                if (!autoclickerSwiftP2)
                    queueButton(
                        1, true,
                        !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                else {
                    queueButton(
                        1, true,
                        !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                    queueButton(
                        1, false,
                        !GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls),
                        0.0
                    );
                }
                autoclickerHoldingP2 = true;
                autoclickerTimerP2 = 0;
            }
            autoclickerTimerP2++;
        }

        runMaintainGravity();
        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    }

    void playExitDualEffect(PlayerObject* player) {
        if (m_playerDied && noDeathEffect)
            return;
        GJBaseGameLayer::playExitDualEffect(player);
    }

    void processQueuedButtons(float dt, bool clearInputQueue) {
        runMaintainGravity();
        bool didReleaseGravityOrb = false;
        for (auto& button : m_queuedButtons) {
            if (autoSwift || (autoReleaseGravityOrbs && releaseGravityOrbsPrevent)) {
                m_queuedButtons.erase(
                    std::remove_if(
                        m_queuedButtons.begin(), m_queuedButtons.end(),
                        [](auto x) { return !x.m_isPush; }
                    ),
                    m_queuedButtons.end()
                );
                releaseGravityOrbsPrevent = false;
            }

            auto player =
                button.m_isPlayer2 ^
                        GameManager::sharedState()->getGameVariable(GameVar::Flip2PlayerControls)
                    ? m_player2
                    : m_player1;

            if (button.m_isPush) {
                for (auto i : CCArrayExt<RingObject*>(player->m_touchingRings)) {
                    switch (i->m_objectType) {
                    case GameObjectType::DashRing:
                        if (clickGreenDash) {
                            auto copy = button;
                            copy.m_isPush = false;
                            m_queuedButtons.insert(m_queuedButtons.begin(), copy);
                            copy.m_isPush = true;
                            m_queuedButtons.insert(m_queuedButtons.begin(), copy);
                        }
                        continue;

                    case GameObjectType::DropRing:
                        if ((blackOrbUfo || straightUfo || clickBlackOrbs)) {
                            if ((player->m_yVelocity <= 0 && !player->m_isUpsideDown) ||
                                (player->m_yVelocity >= 0 && player->m_isUpsideDown) ||
                                clickBlackOrbs) {
                                auto copy = button;
                                copy.m_isPush = false;
                                m_queuedButtons.insert(m_queuedButtons.begin(), copy);
                                copy.m_isPush = true;
                                m_queuedButtons.insert(m_queuedButtons.begin(), copy);
                            }
                        }
                        continue;

                    case GameObjectType::GravityDashRing:
                    case GameObjectType::GravityRing:
                    case GameObjectType::GreenRing:
                        if (maintainGravity &&
                            ((maintainGravityP1 && !player->m_isSecondPlayer) ||
                             (maintainGravityP2 && player->m_isSecondPlayer)) &&
                            !didReleaseGravityOrb) {
                            auto copy = button;
                            copy.m_isPush = false;
                            m_queuedButtons.insert(m_queuedButtons.end(), copy);
                            didReleaseGravityOrb = true;
                            releaseGravityOrbsPrevent = true;
                        }
                        continue;

                    default:
                        break;
                    }
                }

                if (extraClick) {
                    for (int i = 0; i < extraClickAmount; i++) {
                        auto copy = button;
                        copy.m_isPush = false;
                        m_queuedButtons.insert(m_queuedButtons.begin(), copy);
                        copy.m_isPush = true;
                        m_queuedButtons.insert(m_queuedButtons.begin(), copy);
                    }
                }

                if (autoSwift) {
                    auto copy = button;
                    copy.m_isPush = false;
                    m_queuedButtons.insert(m_queuedButtons.end(), copy);
                }
            }
        }
        GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);
    }

    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("GJBaseGameLayer::processQueuedButtons", Priority::First)) {
            geode::log::warn("Failed to set hook priority.");
        }
    }

    void updateColor(
        ccColor3B& color, float fadeTime, int colorID, bool blending, float opacity,
        ccHSVValue& copyHSV, int colorIDToCopy, bool copyOpacity,
        EffectGameObject* callerObject, int unk1, int unk2
    ) {
        if (!PlayLayer::get() || !layoutMode)
            return GJBaseGameLayer::updateColor(
                color, fadeTime, colorID, blending, opacity, copyHSV,
                colorIDToCopy, copyOpacity, callerObject, unk1, unk2
            );

        switch (colorID) {
        case 1000: { // BG
            color = layoutModeColorBackground;
            break;
        }
        case 1001: { // G1
            color = layoutModeColorGround;
            break;
        }
        case 1002: { // Line
            color = {255, 255, 255};
            break;
        }
        default: {
            color = {255, 255, 255};
            break;
        }
        }
        GJBaseGameLayer::updateColor(
            color, fadeTime, colorID, blending, opacity, copyHSV,
            colorIDToCopy, copyOpacity, callerObject, unk1, unk2
        );
    }

    void createBackground(int background) {
        if (layoutMode)
            background = 13;
        GJBaseGameLayer::createBackground(background);
    }

    void createMiddleground(int middleground) {
        if (layoutMode)
            middleground = 0;
        GJBaseGameLayer::createMiddleground(middleground);
    }
};