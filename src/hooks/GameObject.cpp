#include "../includes.hpp"
#include <Geode/modify/GameObject.hpp>

class $modify(ScarletUtilsGObjHook, GameObject) {
    void addGlow(gd::string frame) {
        GameObject::addGlow(std::move(frame));
        if (!PlayLayer::get() || !layoutMode) return;
        
        if ((m_objectType == GameObjectType::Decoration && m_objectID != 44 && m_objectID != 38 && m_objectID != 749 && m_objectID != 747) || m_isNoTouch || decoration.contains(m_objectID))
            m_isHide = true;
        else
            m_isHide = false;
    }

    void playShineEffect() {
        if (noEffect) return;

        GameObject::playShineEffect();
    }
};