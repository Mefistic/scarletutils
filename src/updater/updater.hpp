#pragma once
#include <functional>
#include <vector>

namespace SU::UpdateHook {
    void preTps(std::function<void(bool)> func);
    void latePostTps(std::function<void(bool)> func);
    void postTps(std::function<void(bool)> func);
} // namespace SU::UpdateHook
