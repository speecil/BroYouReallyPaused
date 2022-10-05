#include "main.hpp"
#include "questui/shared/QuestUI.hpp"
#include "MainConfig.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "extern/includes/bs-utils/shared/utils.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "UnityEngine/Vector2.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace QuestUI;
using namespace custom_types;
using namespace QuestUI::BeatSaberUI;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
DEFINE_CONFIG(MainConfig);
// Loads the config from disk using our modInfo, then returns it for use
// other config tools such as config-utils don't use this config, so it can be removed if those are in use
Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Creates a mod settings tab within the main menu and further mod settings.
void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation)  
    {   
        // creates the container for the mod settings
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());
        getLogger().info("created settings menu");
        TMPro::TextMeshProUGUI *text;
        TMPro::TextMeshProUGUI *text1;
        TMPro::TextMeshProUGUI *text2;
        // creates a toggle for the mod
        CreateToggle(container->get_transform(), "Enable Mod", getMainConfig().isEnabled.GetValue(), [](bool value) {
            getMainConfig().isEnabled.SetValue(value);
            getLogger().info("Toggled byrp Mod");
        });
        // Splits the UI to seperate mod toggle and the editing of text
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "_______________________________________________");
        // Creates a keyboard and sets the input to pauseText
        text2 = QuestUI::BeatSaberUI::CreateText(container->get_transform(), "Text Editor");
        text2->set_alignment(TMPro::TextAlignmentOptions::Center);
	    BeatSaberUI::CreateStringSetting(container->get_transform(), StringW(getMainConfig().pauseText.GetName()), StringW(getMainConfig().pauseText.GetValue()), [](std::string ptext){
            getMainConfig().pauseText.SetValue(ptext);
        });
        auto colorPicker = BeatSaberUI::CreateColorPicker(container->get_transform(), "Text Color", getMainConfig().pTextColor.GetValue(),[](UnityEngine::Color color) {
            getMainConfig().pTextColor.SetValue(color, true);
        });

    }
}  

// Pause Menu Hook
// Creates text underneath the pause menu and sets it to the editable variable: pauseText
GameObject * PauseScreen;
MAKE_HOOK_MATCH(PauseMenuHook, &PauseMenuManager::ShowMenu, void, PauseMenuManager *self)
{
    PauseMenuHook(self);
    TMPro::TextMeshProUGUI *text;
    
    if(getMainConfig().isEnabled.GetValue()){
        PauseScreen = BeatSaberUI::CreateFloatingScreen(Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 2.0f), Vector3(5.0f, 0.0f, 0.0f));
        getLogger().info("created screen");
        // creates the text underneath the pause menu and sets the text to the pauseText Config value
        text = BeatSaberUI::CreateText(PauseScreen->get_transform(), getMainConfig().pauseText.GetValue(), Vector2(-2.0f, 0.0f));
        // sets the text to the center
        text->set_alignment(TMPro::TextAlignmentOptions::Center);
        // sets the colour of the text to pTextColor
        text->set_color(getMainConfig().pTextColor.GetValue());
        getLogger().info("set TEXT");

    }
    else if (!getMainConfig().isEnabled.GetValue()) {

    }
}
// Ensures that the text deactivates when the song is continued
MAKE_HOOK_MATCH(PauseMenuContinue, &PauseMenuManager::ContinueButtonPressed, void, PauseMenuManager *self) {
    PauseMenuContinue(self);

    PauseScreen->SetActive(false);;
    
    }

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
    getConfig().Load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading
extern "C" void load() {
    il2cpp_functions::Init();
    getMainConfig().Init(modInfo);
    // Initialises Quest UI
    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate);
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), PauseMenuHook);
    INSTALL_HOOK(getLogger(), PauseMenuContinue);

    getLogger().info("Installed all hooks!");
}
