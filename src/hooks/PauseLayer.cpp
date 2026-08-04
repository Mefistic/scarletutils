#include "../includes.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(ScarletUtilsPauseLayerHook, PauseLayer)
{
    void customSetup() {
        PauseLayer::customSetup();

        auto buttonLabel = CCLabelBMFont::create("Scarlet\nUtils", "goldFont.fnt");
        auto btn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::create(buttonLabel),
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