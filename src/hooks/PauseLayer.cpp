#include "../includes.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsPauseLayerHook, PauseLayer)
{
    void customSetup() {
        PauseLayer::customSetup();

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::create("logo-button.png"_spr),
            this,
            menu_selector(ScarletUtilsPauseLayerHook::onButton)
        );
        
        auto rightButtonMenu = this->getChildByID("right-button-menu");
        if (!rightButtonMenu) return;

        rightButtonMenu->addChild(btn);
        rightButtonMenu->updateLayout();
    }

    void onButton(CCObject* sender) {
        menuVisible = !menuVisible;
    }
};