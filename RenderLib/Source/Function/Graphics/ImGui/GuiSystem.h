#pragma once

#define MAX_SCENE_WINDOW_COUNT 16

#include <vector>
#include <memory>
#include <string>
#include <array>

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

        void AddSceneWindowNextFrame();
        void RemoveSceneWindowNextFrame(uint16_t number);

        [[nodiscard]] std::shared_ptr<GuiBase> GetUIComponentByName(const std::wstring& name) const;

    private:
        GuiSystem() = default;

        bool NewSceneWindow();
        void RemoveSceneWindow(uint16_t number);

        void* m_ImGuiCtx {nullptr};

        std::vector<std::shared_ptr<GuiBase>> m_GuiRegistry;

        std::array<bool, MAX_SCENE_WINDOW_COUNT> m_AvailableSceneWindow {false};
    };
}
