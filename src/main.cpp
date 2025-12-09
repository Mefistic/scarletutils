#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/ui/BasedButtonSprite.hpp"
#include <Geode/Enums.hpp>
#include <Geode/Geode.hpp>
#include <Geode/binding/PauseLayer.hpp>
#include <Geode/binding/RingObject.hpp>
#include <imgui-cocos.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/RingObject.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <matjson/reflect.hpp>

using namespace geode::prelude;
using namespace keybinds;

#ifdef GEODE_IS_WINDOWS
HWND hwnd = FindWindow(NULL, "Geometry Dash");
#endif
bool preventDeath = Mod::get()->getSavedValue<bool>("preventDeath", false);
bool fadeLevel = Mod::get()->getSavedValue<bool>("fadeLevel", false);
double fadeLevelInDuration = Mod::get()->getSavedValue<double>("fadeLevelInDuration", 0.5);
double fadeLevelOutDuration = Mod::get()->getSavedValue<double>("fadeLevelOutDuration", 1.0);
bool fadeAudio = Mod::get()->getSavedValue<bool>("fadeAudio", false);
double fadeAudioInDuration = Mod::get()->getSavedValue<double>("fadeAudioInDuration", 0.5);
double fadeAudioOutDuration = Mod::get()->getSavedValue<double>("fadeAudioOutDuration", 1.0);
bool flipOnDeath = Mod::get()->getSavedValue<bool>("flipOnDeath", false);
bool flipOnDeathLogicP1 = true;
bool flipOnDeathLogicP2 = true;
bool autoUnfreeze = Mod::get()->getSavedValue<bool>("autoUnfreeze", false);
bool flipOnDeathSwift = Mod::get()->getSavedValue<bool>("flipOnDeathSwift", false);
bool clickGreenDashOrbs = Mod::get()->getSavedValue<bool>("clickGreenDashOrbs", false);
bool noDeathEffect = Mod::get()->getSavedValue<bool>("noDeathEffect", false);
bool hideEndscreen = Mod::get()->getSavedValue<bool>("hideEndscreen", false);
bool hideNewBest = Mod::get()->getSavedValue<bool>("hideNewBest", false);
bool blackOrbUfo = Mod::get()->getSavedValue<bool>("blackOrbUfo", false);
bool clickBlackOrbs = Mod::get()->getSavedValue<bool>("clickBlackOrbs", false);
bool noEffect = Mod::get()->getSavedValue<bool>("noEffect", false);
bool clickJumpPads = Mod::get()->getSavedValue<bool>("clickJumpPads", false);

class $modify(scarlet, GJBaseGameLayer)
{
    void playExitDualEffect(PlayerObject* player)
    {
        if (m_playerDied && noDeathEffect)
        {
            return;
        }
        GJBaseGameLayer::playExitDualEffect(player);
    }

    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("GJBaseGameLayer::processQueuedButtons", Priority::First)) {
            geode::log::warn("Failed to set hook priority.");
        }
    }

    void processQueuedButtons()
    {
        for (auto& button : m_queuedButtons)
        {
            auto player = button.m_isPlayer2 ?  m_player2 : m_player1;

            if (clickGreenDashOrbs && button.m_isPush)
            {
                for (auto i : CCArrayExt<RingObject *>(player->m_touchingRings))
                {
                    if (i->m_objectType == GameObjectType::DashRing)
                    {
                        this->queueButton(1, false, button.m_isPlayer2);
                        this->queueButton(1, true, button.m_isPlayer2);
                        
                    }
                }
            }

            if ((blackOrbUfo || clickBlackOrbs) && button.m_isPush)
            {
                for (auto i : CCArrayExt<RingObject *>(player->m_touchingRings))
                {
                    if (i->m_objectType == GameObjectType::DropRing && (player->m_yVelocity <= 0 && !player->m_isUpsideDown) || (player->m_yVelocity >= 0 && player->m_isUpsideDown) || clickBlackOrbs)
                    {
                        this->queueButton(1, false, button.m_isPlayer2);
                        this->queueButton(1, true, button.m_isPlayer2);
                    }
                }
            }
        }
        GJBaseGameLayer::processQueuedButtons();
    }

    // THANK YOU PEONY
    void collisionCheckObjects(PlayerObject *player, gd::vector<GameObject *> *objects, int objectCount, float dt)
    {
        if (objectCount <= 0)
            return;

        for (int i = 0; i < objectCount; ++i)
        {
            GameObject *object = objects->at(i);

            if (
                object->m_objectType != GameObjectType::RedJumpPad &&
                object->m_objectType != GameObjectType::YellowJumpPad &&
                object->m_objectType != GameObjectType::PinkJumpPad
            )
            {
                continue;
            }

            EffectGameObject *obj = (EffectGameObject *)object;

            if (obj->hasBeenActivatedByPlayer(player))
            {
                continue;
            }

            if (!player->getObjectRect().intersectsRect(obj->getObjectRect()))
            {
                continue;
            }

            bool overlaps = true;
            // rotated hitbox
            if (obj->m_shouldUseOuterOb)
            {
                OBB2D *box = object->getOrientedBox();
                player->updateOrientedBox();
                OBB2D *playerBox = ((GameObject *)player)->getOrientedBox();
                overlaps = box->overlaps1Way(playerBox);
            }

            if (!overlaps)
                continue;

            switch (obj->m_objectType)
            {
                case GameObjectType::RedJumpPad:
                    if (clickJumpPads)
                    {           
                        InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", true).post();
                        InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", false).post();
                    }
                    break;
                case GameObjectType::YellowJumpPad:
                    if (clickJumpPads)
                    {           
                        InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", true).post();
                        InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", false).post();
                    }
                case GameObjectType::PinkJumpPad:
                    if (clickJumpPads)
                    {           
                        InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", true).post();
                        InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", false).post();
                    }
            default:
                break;
            }
        }

        GJBaseGameLayer::collisionCheckObjects(player, objects, objectCount, dt);
    }

    void lightningFlash(CCPoint from, CCPoint to, ccColor3B color, float lineWidth, float duration, int displacement, bool flash, float opacity) {
        if (noEffect)
        return;
    GJBaseGameLayer::lightningFlash(from, to, color, lineWidth, duration, displacement, flash, opacity);
}

void teleportPlayer(TeleportPortalObject* object, PlayerObject* player) {
    if (noEffect)
    object->m_hasNoEffects = true;
        GJBaseGameLayer::teleportPlayer(object, player);
    }
};

class $modify(PlayLayer)
{
    void destroyPlayer(PlayerObject* player, GameObject* object)
    {
        
        if (player->m_isDead || object == m_anticheatSpike)
        { return PlayLayer::destroyPlayer(player, object); }

        PlayLayer::destroyPlayer(player, object);

        if (flipOnDeath && preventDeath)
        {
            if (flipOnDeathSwift)
            {
                Loader::get()->queueInMainThread([player]
                {
                Loader::get()->queueInMainThread([player]
                {
                Loader::get()->queueInMainThread([player]
                {
                    InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", true).post();
                    InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", false).post();
                });
                });
                });
            }
            else
            {
                Loader::get()->queueInMainThread([player]
                {
                Loader::get()->queueInMainThread([player]
                {
                Loader::get()->queueInMainThread([player]
                {
                    InvokeBindEvent(player->m_isSecondPlayer ? "robtop.geometry-dash/jump-p2" : "robtop.geometry-dash/jump-p1", 
                        player->m_isSecondPlayer ? flipOnDeathLogicP2 : flipOnDeathLogicP1).post();
                });
                });
                });
                (player->m_isSecondPlayer ? flipOnDeathLogicP2 : flipOnDeathLogicP1) = !(player->m_isSecondPlayer ? flipOnDeathLogicP2 : flipOnDeathLogicP1);
            }
        }

        #ifdef GEODE_IS_WINDOWS
        if (preventDeath)
        Loader::get()->queueInMainThread([] {
            SendMessage(hwnd, WM_KEYDOWN, 0x42, 0);
            SendMessage(hwnd, WM_KEYUP, 0x42, 0);

            if (autoUnfreeze)
                Loader::get()->queueInMainThread([] {
                SendMessage(hwnd, WM_KEYDOWN, 0x56, 0);
                SendMessage(hwnd, WM_KEYUP, 0x56, 0);
            });
        });
        #endif
    }

    void applyStartFade()
    {
        if (fadeLevel)
        {
            if (this->getChildByID("start-fade-layer"_spr))
            {
                return;
            }
            
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto fadeLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), winSize.width, winSize.height);
            fadeLayer->setID("start-fade-layer"_spr);
            this->addChild(fadeLayer, 100);

            auto fadeAction = CCFadeOut::create(fadeLevelInDuration); 
            auto removeAction = CCRemoveSelf::create();
            fadeLayer->runAction(CCSequence::create(fadeAction, removeAction, nullptr));
        }
        if (fadeAudio)
        {
            FMODAudioEngine::sharedEngine()->fadeMusic(
                fadeAudioInDuration, 0, 0.0f, 1.0f
            );
        };
    }

    void startGame()
    {
        PlayLayer::startGame();
        this->applyStartFade();
    }

    void showEndLayer()
    {
        PlayLayer::showEndLayer(); 

        if (fadeLevel)
        {
            if (this->getChildByID("end-fade-layer"_spr))
            {
                return;
            }

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto fadeLayer = CCLayerColor::create(ccc4(0, 0, 0, 0), winSize.width, winSize.height);
            fadeLayer->setID("end-fade-layer"_spr);
            this->addChild(fadeLayer, 1000); 

            auto fadeIn = CCFadeIn::create(fadeLevelOutDuration); 
            
            fadeLayer->runAction(fadeIn);
        }

        if (fadeAudio)
        {
            FMODAudioEngine::sharedEngine()->fadeMusic(
                fadeAudioOutDuration, 0, 1.0f, 0.0f
            );
        }
    }

    void showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
        if (hideNewBest)
            return;
        PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
    }
};

class $modify(EndLevelLayer)
{
    void showLayer(bool instant)
    {
        if (hideEndscreen)
            return;
        EndLevelLayer::showLayer(instant);
    }
};

class $modify(RingObject)
{
    void spawnCircle() {
        if (noEffect)
            return;
        RingObject::spawnCircle();
    }
};

class $modify(PlayerObject)
{
    void playDeathEffect() {
        if (noDeathEffect)
            return;
        PlayerObject::playDeathEffect();
    }

    void ringJump(RingObject* object, bool skipCheck) {
        if (noEffect)
            m_playEffects = false;
        PlayerObject::ringJump(object, skipCheck);
    }

    void startDashing(DashRingObject* object) {
        if (noEffect)
            m_playEffects = false;
        PlayerObject::startDashing(object);
    }

    void updateDashAnimation() {
        if (noEffect)
            return;
        PlayerObject::updateDashAnimation();
    }

    void spawnDualCircle() {
        if (noEffect)
            return;
      PlayerObject::spawnDualCircle();
    }

    void spawnPortalCircle(ccColor3B color, float startRadius) {
        if (noEffect)
            return;
        PlayerObject::spawnPortalCircle(color, startRadius);
    }

    void spawnScaleCircle() {
        if (noEffect)
            return;
        PlayerObject::spawnScaleCircle();
    }

    void spawnCircle() {
        if (noEffect)
            return;
        PlayerObject::spawnCircle();
    }

    void playSpiderDashEffect(CCPoint from, CCPoint to) {
        if (noEffect)
            return;
        PlayerObject::playSpiderDashEffect(from, to);
    }
};

class $modify(EffectGameObject)
{
    void playTriggerEffect() {
        if (noEffect)
            return;
        EffectGameObject::playTriggerEffect();
    }
};

class $modify(GameObject)
{
    void playShineEffect() {
        if (noEffect)
            return;
        GameObject::playShineEffect();
    }
};

bool menuVisible = false;

$execute
{
    BindManager::get()->registerBindable(
    {
        "menu"_spr,
        "Show Menu",
        "",
        {Keybind::create(KEY_F, Modifier::None)},
        "Scarlet Utils"
    });

    new EventListener(+[](InvokeBindEvent* event)
    {
        if (event->isDown()) {
            menuVisible = !menuVisible;
        }
        return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "menu"_spr));
};

$on_mod(Loaded)
{
    ImGuiCocos::get().setup([] {
        auto* font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "font.ttf").string().c_str(), 18.0f);
        ImGui::GetIO().FontDefault = font;
    }).draw([&]
        {
            if (!menuVisible)
                return;

            ImGuiStyle &style = ImGui::GetStyle();
            style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
            style.WindowMenuButtonPosition = ImGuiDir_None;
            style.WindowBorderSize = 0.f;
            style.WindowRounding = 6.0f;
            style.PopupBorderSize = 0.f;
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.016f, 0.025f, 0.9f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.016f, 0.025f, 0.9f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5f, 0.16f, 0.25f, 1.0f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.2f, 0.35f, 1.0f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.5f, 0.16f, 0.25f, 0.4f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.16f, 0.25f, 1.0f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.16f, 0.25f, 1.0f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.f, 0.25f, 0.4f, 0.33f);

            ImGui::SetNextWindowSizeConstraints(ImVec2(450.f, -1.0f), ImVec2(INFINITY, -1.0f));
            ImGui::Begin("Scarlet Utils", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            #ifdef GEODE_IS_WINDOWS
            ImGui::Checkbox("Auto Backstep On Death", &preventDeath);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("preventDeath", preventDeath); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("REQUIRES SILICATE");
                ImGui::Text("Simulates a B press after death, wont work if your silicate backstep keybind is set to something else.");
                ImGui::Text("Warning: Silicate UI may be laggy while using this.");
                ImGui::EndTooltip();
            }
            
            ImGui::Checkbox("Auto Unfreeze On Death", &autoUnfreeze);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("autoUnfreeze", autoUnfreeze); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("REQUIRES SILICATE");
                ImGui::Text("Simulates a V press after death, wont work if your silicate frame advance keybind is set to something else.");
                ImGui::Text("Only works with auto backstep on death.");
                ImGui::Text("Warning: Silicate UI may be laggy while using this.");
                ImGui::EndTooltip();
            }
            #endif

            ImGui::Checkbox("Click Green Dash Orbs", &clickGreenDashOrbs);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("clickGreenDashOrbs", clickGreenDashOrbs); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("When you click, all green dash orbs your player can touch will be activated.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Click Black Orbs", &clickBlackOrbs);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("clickBlackOrbs", clickBlackOrbs); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("When you click, all black orbs your player can touch will be activated.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Easy Black Orb UFO", &blackOrbUfo);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("blackOrbUfo", blackOrbUfo); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("When you click, all black orbs your player can touch will be activated.");
                ImGui::Text("If your player is going up (or down if gravity flipped), use a black orb normally.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Click Jump Pads", &clickJumpPads);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("clickJumpPads", clickJumpPads); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("When you hit pink, yellow or red pads, automatically swift click.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Flip Input On Death", &flipOnDeath);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("flipOnDeath", flipOnDeath); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("REQUIRES SILICATE");
                ImGui::Text("After death, clicks or releases automatically.");
                ImGui::Text("Only works with auto backstep.");
                ImGui::Text("Note: To click both players, use mirror inputs in Silicate.");
                ImGui::EndTooltip();
            }

            static bool flipInputOptions = false;
            ImGui::SameLine();
            if (ImGui::ArrowButton("Flip Input Options", flipInputOptions ? ImGuiDir_Down : ImGuiDir_Right)) flipInputOptions = !flipInputOptions;

            if (flipInputOptions) {
                ImGui::Checkbox("Swift", &flipOnDeathSwift);
                if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("flipOnDeathSwift", flipOnDeathSwift); }
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Instead of clicking or releasing, do both.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Level Fade In/Out", &fadeLevel);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("fadeLevel", fadeLevel); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Levels will fade in and out on level start/end.");
                ImGui::Text("For use with renderer.");
                ImGui::EndTooltip();
            }

            static bool levelFadeOptions = false;
            ImGui::SameLine();
            if (ImGui::ArrowButton("Level Fade Options", levelFadeOptions ? ImGuiDir_Down : ImGuiDir_Right)) levelFadeOptions = !levelFadeOptions;
            
            if (levelFadeOptions) {
                ImGui::DragScalar("Level Fade In (s)", ImGuiDataType_Double, &fadeLevelInDuration, 0.01, 0, NULL, "%.2f");
                if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<double>("fadeLevelInDuration", fadeLevelInDuration); }

                ImGui::DragScalar("Level Fade Out (s)", ImGuiDataType_Double, &fadeLevelOutDuration, 0.01, 0, NULL, "%.2f");
                if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<double>("fadeLevelOutDuration", fadeLevelOutDuration); }
            }

            ImGui::Checkbox("Audio Fade In/Out", &fadeAudio);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("fadeAudio", fadeAudio); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Music will fade in and out on level start/end.");
                ImGui::Text("For use with renderer.");
                ImGui::EndTooltip();
            }

            static bool audioFadeOptions = false;
            ImGui::SameLine();
            if (ImGui::ArrowButton("Audio Fade Options", audioFadeOptions ? ImGuiDir_Down : ImGuiDir_Right)) audioFadeOptions = !audioFadeOptions;

            if (audioFadeOptions) {
                ImGui::DragScalar("Audio Fade In (s)", ImGuiDataType_Double, &fadeAudioInDuration, 0.01, 0, NULL, "%.2f");
                if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<double>("fadeAudioInDuration", fadeAudioInDuration); }

                ImGui::DragScalar("Audio Fade Out (s)", ImGuiDataType_Double, &fadeAudioOutDuration, 0.01, 0, NULL, "%.2f");
                if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<double>("fadeAudioOutDuration", fadeAudioOutDuration); }
            }

            ImGui::Checkbox("Hide Endscreen", &hideEndscreen);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("hideEndscreen", hideEndscreen); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("After level complete, dont show the statistics screen.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("Hide New Best", &hideNewBest);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("hideNewBest", hideNewBest); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Stops the new best popup from rendering.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("No Death Effect", &noDeathEffect);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("noDeathEffect", noDeathEffect); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Stops the death effect from rendering.");
                ImGui::EndTooltip();
            }

            ImGui::Checkbox("No Effects", &noEffect);
            if (ImGui::IsItemEdited()) { Mod::get()->setSavedValue<bool>("noEffect", noEffect); }
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Stops most effects from rendering.");
                ImGui::EndTooltip();
            }

            ImGui::End();
            });
}

#ifdef GEODE_MOBILE
class $modify(ScarletUtilsPauseLayerHook, PauseLayer)
{
    void customSetup() {
        PauseLayer::customSetup();

        auto buttonLabel = CCLabelBMFont::create("Utils", "goldFont.fnt");
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
#endif