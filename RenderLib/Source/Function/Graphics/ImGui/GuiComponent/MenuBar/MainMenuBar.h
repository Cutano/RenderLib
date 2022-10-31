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

        void SetAutoRecompile(bool autoRecompile);
        [[nodiscard]] bool& IsAutoRecompile();

    private:
        bool m_AutoRecompile {false};
    };
}
