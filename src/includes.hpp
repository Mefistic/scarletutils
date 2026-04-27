#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

extern bool preventDeath;
extern bool noDeathEffect;
extern bool hideEndscreen;
extern bool hideNewBest;
extern bool noEffect;

extern bool   fadeLevel;
extern double fadeLevelInDuration;
extern double fadeLevelOutDuration;

extern bool   fadeAudio;
extern double fadeAudioInDuration;
extern double fadeAudioOutDuration;

extern bool straightFlyP1;
extern bool straightFlyP2;
extern double straightFlyThresholdP1;
extern double straightFlyThresholdP2;

extern bool straightUfoP1;
extern bool straightUfoP2;
extern double straightUfoTargetP1;
extern double straightUfoTargetP2;
extern double straightUfoThresholdP1;
extern double straightUfoThresholdP2;

extern cocos2d::ccColor3B layoutModeColorBackground;
extern cocos2d::ccColor3B layoutModeColorGround;

extern bool flipOnDeath;
extern bool flipOnDeathLogicP1;
extern bool flipOnDeathLogicP2;
extern bool flipOnDeathSwift;

extern bool autoUnfreeze;
extern bool autoSwift;
extern bool extraClick;
extern int  extraClickAmount;

extern bool clickGreenDash;
extern bool clickBlackOrbs;
extern bool clickJumpPads;
extern bool clickGravityPads;
extern bool clickedJumpPad;

extern bool straightFly;
extern bool straightUfo;

extern bool maintainGravity;
extern bool maintainGravityP1;
extern bool maintainGravityP2;

extern bool autoclickerP1;
extern bool autoclickerP2;

extern int autoclickerEveryP1;
extern int autoclickerEveryP2;
extern int autoclickerHoldP1;
extern int autoclickerHoldP2;

extern int  autoclickerTimerP1;
extern int  autoclickerTimerP2;
extern bool autoclickerHoldingP1;
extern bool autoclickerHoldingP2;

extern bool autoclickerSwiftP1;
extern bool autoclickerSwiftP2;

extern bool noclip;
extern bool noclipP1;
extern bool noclipP2;

extern bool spamCheckpoints;
extern bool restartFirstFrame;

extern bool layoutMode;
extern bool blackOrbUfo;

extern bool autoReleaseGravityOrbs;
extern bool releaseGravityOrbsPrevent;

extern cocos2d::CCLayerColor* m_startFadeLayer;
extern cocos2d::CCLayerColor* m_endFadeLayer;

std::array<float, 3> colorToFloat(cocos2d::ccColor3B color);
cocos2d::ccColor3B floatToColor(float* col);

extern std::unordered_set<int> decoration;
extern std::unordered_set<int> filter;