#pragma once
#include "Function/Graphics/ImGui/GuiWindowBase.h"

namespace RL
{
    class DemoWindow final : public GuiWindowBase
    {
    public:
        explicit DemoWindow();

        ~DemoWindow() override = default;
        DemoWindow(const DemoWindow& other) = delete;
        DemoWindow(DemoWindow&& other) noexcept = delete;
        DemoWindow& operator=(const DemoWindow& other) = delete;
        DemoWindow& operator=(DemoWindow&& other) noexcept = delete;

        void Draw() override;
    };
}
