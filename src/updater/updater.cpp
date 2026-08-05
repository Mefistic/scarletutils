// macos fix by gwddos

#include "updater.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

namespace ScarletUtils::UpdateHook {
    namespace {
        std::vector<std::function<void(bool)>> g_preTps;
        std::vector<std::function<void(bool)>> g_postTps;
        std::vector<std::function<void(bool)>> g_latePostTps;
    }

    void preTps(std::function<void(bool)> func) {
        g_preTps.push_back(func);
    }

    void postTps(std::function<void(bool)> func) {
        g_postTps.push_back(func);
    }

    void latePostTps(std::function<void(bool)> func) {
        g_latePostTps.push_back(func);
    }
}

#ifdef GEODE_IS_MACOS
static bool g_inBaseGameLayerUpdate = false;

class $modify(GJBaseGameLayer) {
    void update(float dt) {
        g_inBaseGameLayerUpdate = true;
        GJBaseGameLayer::update(dt);
        g_inBaseGameLayerUpdate = false;
    }
};
#endif

class $modify(GJBaseGameLayer) {
#ifndef GEODE_IS_MACOS
    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        for (const auto& func : ScarletUtils::UpdateHook::g_preTps) {
            func(isHalfTick);
        }

        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);

        for (const auto& func : ScarletUtils::UpdateHook::g_postTps) {
            func(isHalfTick);
        }

        for (const auto& func : ScarletUtils::UpdateHook::g_latePostTps) {
            func(isHalfTick);
        }
    }
#else
    static void onModify(auto& self) {
        (void)self.setHookPriority("GJBaseGameLayer::processQueuedButtons", -0x500000);
    }

    void processQueuedButtons(float dt, bool clearInputQueue) {
        if (!g_inBaseGameLayerUpdate) {
            GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);
            return;
        }

        bool isHalfTick = m_isBetweenSteps;

        for (const auto& func : ScarletUtils::UpdateHook::g_preTps) {
            func(isHalfTick);
        }

        GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);

        for (const auto& func : ScarletUtils::UpdateHook::g_postTps) {
            func(isHalfTick);
        }

        for (const auto& func : ScarletUtils::UpdateHook::g_latePostTps) {
            func(isHalfTick);
        }
    }
#endif
};