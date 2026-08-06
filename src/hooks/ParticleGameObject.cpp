#include "../includes.hpp"
#include <Geode/modify/ParticleGameObject.hpp>

using namespace geode::prelude;

class $modify(ScarletParticleGameObject, ParticleGameObject) {
	void updateParticle() {
		ParticleGameObject::updateParticle();
		if (noEffect)
		this->setVisible(false);
	}
};