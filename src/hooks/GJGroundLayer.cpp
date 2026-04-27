#include "../includes.hpp"
#include <Geode/modify/GJGroundLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsGJGLHook, GJGroundLayer) {
    static GJGroundLayer* create(int groundID, int lineType) {
        if (layoutMode) {
            groundID = 1;
            lineType = 1;
        }
        GJGroundLayer* ret = GJGroundLayer::create(groundID, lineType);

        return ret;
    }
};