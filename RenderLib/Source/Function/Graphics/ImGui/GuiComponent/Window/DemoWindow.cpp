#include "DemoWindow.h"

#include <imgui.h>

namespace RL
{
    DemoWindow::DemoWindow() : GuiToolWindowBase(L"ImGuiDemoWindow")
    {

    }

    void DemoWindow::Draw()
    {
        if (m_Show)
        {
            ImGui::ShowDemoWindow(&m_Show);
        }

        GuiToolWindowBase::Draw();
    }
}
