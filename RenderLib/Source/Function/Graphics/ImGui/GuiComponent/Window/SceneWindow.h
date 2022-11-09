#pragma once
#include "Function/Graphics/ImGui/GuiWindowBase.h"

namespace RL
{
    class SceneWindow final : GuiWindowBase
    {
    public:
        explicit SceneWindow();
        explicit SceneWindow(const std::wstring& name);

        ~SceneWindow() override = default;
        SceneWindow(const SceneWindow& other) = delete;
        SceneWindow(SceneWindow&& other) noexcept = delete;
        SceneWindow& operator=(const SceneWindow& other) = delete;
        SceneWindow& operator=(SceneWindow&& other) noexcept = delete;

        void Draw() override;

    private:
        bool m_IsSceneWindowHovered {false};

        float m_SceneWindowWidth {1920.0f};
        float m_SceneWindowHeight {1080.0f};
        float m_SceneWindowPosX {0};
        float m_SceneWindowPosY {0};
    };
}
