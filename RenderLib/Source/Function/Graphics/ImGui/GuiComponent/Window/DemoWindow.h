#pragma once
#include "Function/Graphics/ImGui/GuiToolWindowBase.h"

namespace RL
{
    class DemoWindow final : public GuiToolWindowBase
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
