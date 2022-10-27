#pragma once

#include <vector>
#include <memory>

namespace RL
{
    class GuiBase;
    
    class GuiSystem
    {
    public:
        static GuiSystem& Get()
        {
            static GuiSystem s_Instance;
            return s_Instance;
        }

        ~GuiSystem() = default;
        GuiSystem(const GuiSystem& other) = delete;
        GuiSystem(GuiSystem&& other) noexcept = delete;
        GuiSystem& operator=(const GuiSystem& other) = delete;
        GuiSystem& operator=(GuiSystem&& other) noexcept = delete;

        void Init();
        void Update();

    private:
        GuiSystem() = default;

        void* m_ImGuiCtx {nullptr};

        std::vector<std::shared_ptr<GuiBase>> m_GuiRegistry;
    };
}
