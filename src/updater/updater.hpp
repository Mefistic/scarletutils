#pragma once
#include <functional>
#include <vector>

namespace ScarletUtils::UpdateHook {
    void preTps(std::function<void(bool)> func);
    void latePostTps(std::function<void(bool)> func);
    void postTps(std::function<void(bool)> func);
}
