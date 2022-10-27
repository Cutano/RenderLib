#include "DemoWindow.h"

#include <imgui.h>

namespace RL
{
    DemoWindow::DemoWindow() : GuiWindowBase(L"ImGui Demo Window")
    {
        m_Show = true;
    }

    void DemoWindow::Draw()
    {
        if (m_Show)
        {
            ImGui::ShowDemoWindow(&m_Show);
        }
    }
}
