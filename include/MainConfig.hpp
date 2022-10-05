#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(MainConfig,
    CONFIG_VALUE(isEnabled, bool, "Enable Mod", true);
    CONFIG_VALUE(pauseText, std::string, "Pause Text", "Bro You Really Paused? Cringe");
     CONFIG_VALUE(pTextColor, UnityEngine::Color, "Color", UnityEngine::Color(1.0, 1.0, 1.0, 1.0));


    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(isEnabled);
        CONFIG_INIT_VALUE(pauseText);
        CONFIG_INIT_VALUE(pTextColor);

    )
)
