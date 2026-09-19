#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

#include <algorithm>
#include <string>

using namespace geode::prelude;

// ====================== GD GPT Popup ======================
class GDGPTPopup : public Popup<> {
protected:
    TextInput* m_input = nullptr;
    CCLabelBMFont* m_responseLabel = nullptr;

    bool setup() override {
        this->setTitle("GD GPT");

        m_input = TextInput::create(280.f, "Type a command...", "bigFont.fnt");
        m_input->setPosition({0.f, 45.f});
        m_input->setCommonFilter(CommonFilter::Any);
        this->m_mainLayer->addChild(m_input);

        auto sendSpr = ButtonSprite::create("Send", "bigFont.fnt", "GJ_button_01.png", 0.7f);
        auto sendBtn = CCMenuItemSpriteExtra::create(
            sendSpr, this, menu_selector(GDGPTPopup::onSend)
        );
        sendBtn->setPosition({0.f, 5.f});

        auto menu = CCMenu::create();
        menu->setPosition({0.f, 0.f});
        menu->addChild(sendBtn);
        this->m_mainLayer->addChild(menu);

        m_responseLabel = CCLabelBMFont::create(
            "GD GPT ready.\nTry: theme hell\nmake clubstep monster\ngenerate section",
            "chatFont.fnt"
        );
        m_responseLabel->setScale(0.55f);
        m_responseLabel->setAnchorPoint({0.5f, 1.f});
        m_responseLabel->setPosition({0.f, -40.f});
        m_responseLabel->setAlignment(kCCTextAlignmentCenter);
        this->m_mainLayer->addChild(m_responseLabel);

        return true;
    }

    void onSend(CCObject*) {
        auto text = m_input->getString();
        if (text.empty()) return;

        std::string reply = processCommand(text);
        m_responseLabel->setString(reply.c_str());
        m_input->setString("");
    }

    std::string processCommand(std::string const& cmd) {
        std::string lower = cmd;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower.find("theme hell") != std::string::npos || lower.find("make the theme hell") != std::string::npos || lower == "hell")
            return "Hell / Infernal theme activated.\nRed & black death incoming.";
        if (lower.find("theme cyber") != std::string::npos || lower.find("cyber") != std::string::npos)
            return "Cyber / Neon theme locked.\nElectric circuits ready.";
        if (lower.find("theme abyss") != std::string::npos || lower.find("abyss") != std::string::npos || lower.find("void") != std::string::npos)
            return "Abyss / Void theme set.\nEmptiness awaits.";
        if (lower.find("theme blood") != std::string::npos || lower.find("blood") != std::string::npos || lower.find("crimson") != std::string::npos)
            return "Blood / Crimson theme active.";
        if (lower.find("theme ice") != std::string::npos || lower.find("frost") != std::string::npos)
            return "Ice / Frost theme locked in.";
        if (lower.find("factory") != std::string::npos || lower.find("industrial") != std::string::npos)
            return "Factory / Industrial theme ready.";
        if (lower.find("cosmic") != std::string::npos || lower.find("space") != std::string::npos)
            return "Cosmic / Space theme activated.";
        if (lower.find("nature") != std::string::npos || lower.find("forest") != std::string::npos)
            return "Nature / Forest theme set.";

        if (lower.find("clubstep monster") != std::string::npos || lower.find("clubstep") != std::string::npos)
            return "Clubstep Monster template ready.\n(Full object placement coming next update)";

        if (lower.find("generate") != std::string::npos || lower.find("make section") != std::string::npos || lower.find("build") != std::string::npos)
            return "Generation system loading...\nPreview + real placement coming soon.";

        if (lower.find("help") != std::string::npos || lower == "?")
            return "Commands:\n- theme hell / cyber / abyss / blood\n- make clubstep monster\n- generate section\nMore power coming!";

        return "Got it: \"" + cmd + "\"\nI currently understand themes and basic requests.\nFull placement system is next.";
    }

public:
    static GDGPTPopup* create() {
        auto ret = new GDGPTPopup();
        if (ret && ret->initAnchored(320.f, 240.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

// ====================== Button on MAIN MENU ======================
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto btn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName(
                "GJ_chatBtn_001.png", 1.0f,
                CircleBaseColor::Green, CircleBaseSize::Medium
            ),
            this,
            menu_selector(MyMenuLayer::onGDGPT)
        );
        btn->setID("gd-gpt-menu-button"_spr);

        // Put it on the bottom row near the other buttons
        if (auto menu = this->getChildByID("bottom-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        } else {
            // Fallback - left side
            auto menu = CCMenu::create();
            menu->setID("gd-gpt-main-menu"_spr);
            menu->addChild(btn);
            menu->setPosition({50.f, 100.f});
            this->addChild(menu);
        }

        return true;
    }

    void onGDGPT(CCObject*) {
        GDGPTPopup::create()->show();
    }
};

// ====================== Button in EDITOR ======================
class $modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* layer) {
        if (!EditorUI::init(layer)) return false;

        auto btn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName(
                "edit_eCreateBtn_001.png", 1.0f,
                CircleBaseColor::Green, CircleBaseSize::Medium
            ),
            this,
            menu_selector(MyEditorUI::onGDGPT)
        );
        btn->setID("gd-gpt-editor-button"_spr);

        // Try to place it nicely on the side
        if (auto menu = this->getChildByID("undo-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        } else if (auto menu = this->getChildByID("toolbar-categories-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        } else {
            // Fallback - left side of screen
            auto menu = CCMenu::create();
            menu->setID("gd-gpt-editor-menu"_spr);
            menu->addChild(btn);
            menu->setPosition({50.f, 180.f});
            this->addChild(menu);
        }

        return true;
    }

    void onGDGPT(CCObject*) {
        GDGPTPopup::create()->show();
    }
};

$execute {
    log::info("GD GPT loaded successfully!");
}
