#pragma once

#include "IPreference.h"

namespace RL
{
    struct SceneWindowPreference final : IPreference
    {
        bool ShowSceneWindow1 {true};
        bool ShowSceneWindow2 {false};
        bool ShowSceneWindow3 {false};
        bool ShowSceneWindow4 {false};
        
        void ToJson(nlohmann::json& j) const override
        {
            j = nlohmann::json{
                {"showSceneWindow1", ShowSceneWindow1},
                {"showSceneWindow2", ShowSceneWindow2},
                {"showSceneWindow3", ShowSceneWindow3},
                {"showSceneWindow4", ShowSceneWindow4},
            };
        }
        
        void FromJson(const nlohmann::json& j) override
        {
            j.at("showSceneWindow1").get_to(ShowSceneWindow1);
            j.at("showSceneWindow2").get_to(ShowSceneWindow2);
            j.at("showSceneWindow3").get_to(ShowSceneWindow3);
            j.at("showSceneWindow4").get_to(ShowSceneWindow4);
        }
    };
    
    struct WindowPreference final : IPreference
    {
        SceneWindowPreference SceneWindowPref;
        
        void ToJson(nlohmann::json& j) const override
        {
            nlohmann::json sceneWindowJson;
            SceneWindowPref.ToJson(sceneWindowJson);
            
            j = nlohmann::json{
                {"sceneWindow", sceneWindowJson}
            };
        }
        
        void FromJson(const nlohmann::json& j) override
        {
            SceneWindowPref.FromJson(j["sceneWindow"]);
        }
    };
}
