#pragma once
#include "Function/Graphics/ImGui/GuiWindowBase.h"

namespace RL
{
    class SceneCamera;
    
    class SceneWindow final : public GuiWindowBase
    {
    public:
        explicit SceneWindow(uint16_t index);

        ~SceneWindow() override;
        SceneWindow(const SceneWindow& other) = delete;
        SceneWindow(SceneWindow&& other) noexcept = delete;
        SceneWindow& operator=(const SceneWindow& other) = delete;
        SceneWindow& operator=(SceneWindow&& other) noexcept = delete;

        void Draw() override;

        uint16_t GetIndex();

    private:
        void OnClosed();
        void OnShow();
        
        uint16_t m_Index {0};
        
        bool m_IsSceneWindowHovered {false};

        float m_SceneWindowWidth {1440.0f};
        float m_SceneWindowHeight {900.0f};
        float m_SceneWindowPosX {0};
        float m_SceneWindowPosY {0};

        SceneCamera* m_Camera {nullptr};
    };
}
