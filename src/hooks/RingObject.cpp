#include "../includes.hpp"
#include <Geode/modify/RingObject.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsRingObjectHook, RingObject) {
	void spawnCircle() {
        	static int lastOrb;
		if (!noEffect || !(optimizeStackedOrbs && optimizeRingJump && lastOrb == this->m_objectID))
			RingObject::spawnCircle();
		optimizeRingJump = true;
        	lastOrb = this->m_objectID;
	}
};