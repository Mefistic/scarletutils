#include "../includes.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsPauseLayerHook, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto rightButtonMenu = this->getChildByID("right-button-menu");
        if (!rightButtonMenu) return;

        auto sprite = CCSprite::create("logo-button.png"_spr);

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(ScarletUtilsPauseLayerHook::onButton)
        );
        btn->setID("scarlet-utils-button"_spr);

        rightButtonMenu->addChild(btn);
        rightButtonMenu->updateLayout();
    }

    void onButton(CCObject* sender) {
        menuVisible = !menuVisible;
    }
};