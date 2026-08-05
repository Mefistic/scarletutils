// reason for updater is processCommands is inlined on mac :3
// astrals solution was to make an updater that could select the right function to hook depending on device

#include "updater.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

namespace SU::UpdateHook {
    // Keep vectors internal to this translation unit with distinct names
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
} // namespace SU::UpdateHook

class $modify(AstralTPSHook, GJBaseGameLayer) {
#ifndef GEODE_IS_MACOS
    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        for (const auto& func : SU::UpdateHook::g_preTps) {
            func(isHalfTick);
        }

        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);

        for (const auto& func : SU::UpdateHook::g_postTps) {
            func(isHalfTick);
        }

        for (const auto& func : SU::UpdateHook::g_latePostTps) {
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

        for (const auto& func : SU::UpdateHook::g_preTps) {
            func(isHalfTick);
        }

        GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);

        for (const auto& func : SU::UpdateHook::g_postTps) {
            func(isHalfTick);
        }

        for (const auto& func : SU::UpdateHook::g_latePostTps) {
            func(isHalfTick);
        }
    }
#endif
};