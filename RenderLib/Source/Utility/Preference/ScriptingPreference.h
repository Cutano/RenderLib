#pragma once
#include "IPreference.h"

namespace RL
{
    struct ScriptingPreference final : IPreference
    {
        bool AutoCompile {false};

        ScriptingPreference() = default;

        void ToJson(nlohmann::json& j) const override
        {
            j = nlohmann::json{{"autoCompile", AutoCompile}};
        }
        
        void FromJson(const nlohmann::json& j) override
        {
            j.at("autoCompile").get_to(AutoCompile);
        }
    };
}
