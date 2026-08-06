#include "../includes.hpp"
#include <Geode/modify/CCParticleSystem.hpp>

using namespace geode::prelude;

class $modify(ScarletCCParticleSystem, CCParticleSystem) {
	void update(float dt) {
		update(dt);
		if (noEffect)
		this->setVisible(false);
    	}
};