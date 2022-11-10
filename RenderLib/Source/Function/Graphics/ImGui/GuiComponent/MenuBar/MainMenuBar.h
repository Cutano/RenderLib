#pragma once
#include "Function/Graphics/ImGui/GuiBase.h"

namespace RL
{
    class MainMenuBar final : public GuiBase
    {
    public:
        explicit MainMenuBar();

        ~MainMenuBar() override = default;
        MainMenuBar(const MainMenuBar& other) = delete;
        MainMenuBar(MainMenuBar&& other) noexcept = delete;
        MainMenuBar& operator=(const MainMenuBar& other) = delete;
        MainMenuBar& operator=(MainMenuBar&& other) noexcept = delete;

        void Draw() override;

        [[nodiscard]] bool& IsAutoRecompile();
        void SetAutoRecompile(bool autoRecompile);
        void ToggleSceneWindow(int index, bool show, bool spread);
        void ToggleImGuiDemoWindow(bool show, bool spread);

    private:
        void LoadPreferences();
        
        bool m_AutoRecompile {false};
        
        bool m_ShowSceneWindow[4] {true, false, false, false};
        bool m_ShowImGuiDemoWindow {true};
    };
}
