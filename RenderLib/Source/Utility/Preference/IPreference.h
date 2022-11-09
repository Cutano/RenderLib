#pragma once

#include <nlohmann/json.hpp>

namespace RL
{
    class IPreference
    {
    public:
        virtual ~IPreference() = default;
        virtual void ToJson(nlohmann::json& j) const = 0;
        virtual void FromJson(const nlohmann::json& j) = 0;
    };

    inline void to_json(nlohmann::json& j, const IPreference& p)
    {
        p.ToJson(j);
    }

    inline void from_json(const nlohmann::json& j, IPreference& p)
    {
        p.FromJson(j);
    }
}
