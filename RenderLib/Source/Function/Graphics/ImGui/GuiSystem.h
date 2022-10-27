#pragma once

namespace RL
{
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
    };
}
