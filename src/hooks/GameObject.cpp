#include "../includes.hpp"
#include <Geode/modify/GameObject.hpp>

class $modify(ScarletGameObject, GameObject) {
    void addGlow(gd::string frame) {
        GameObject::addGlow(std::move(frame));
        if (!PlayLayer::get() || !layoutMode) return;
        
        m_isHide = ((m_objectType == GameObjectType::Decoration && m_objectID != 44 && m_objectID != 38 && m_objectID != 749 && m_objectID != 747) || m_isNoTouch || decoration.contains(m_objectID));
    }

    void playShineEffect() {
        if (!noEffect)
        GameObject::playShineEffect();
    }
};