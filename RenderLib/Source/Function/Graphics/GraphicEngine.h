#pragma once

#include <memory>

typedef struct HWND__ *HWND;

namespace Diligent
{
    struct IEngineFactoryD3D12;
    struct IRenderDevice;
    struct IDeviceContext;
    struct ITextureView;
}

namespace RL
{
    class Window;
    class ImGuiImplRenderLib;
    
    class GraphicEngine
    {
    public:
        static GraphicEngine& Get()
        {
            static GraphicEngine s_Instance;
            return s_Instance;
        }
        
        ~GraphicEngine() = default;
        GraphicEngine(const GraphicEngine& other) = delete;
        GraphicEngine(GraphicEngine&& other) noexcept = delete;
        GraphicEngine& operator=(const GraphicEngine& other) = delete;
        GraphicEngine& operator=(GraphicEngine&& other) noexcept = delete;

        void Init();
        void Update();
        void Render();
        void Shutdown();

        void AttachMainWindow();
        void AttachGuiBackend();
        void AttachWindow(const std::shared_ptr<Window>& window);
        void AttachRawWindow(HWND hwnd);

        Diligent::IDeviceContext* GetDeviceContext() const;
        Diligent::IRenderDevice*  GetDevice() const;
        
    private:
        GraphicEngine() = default;

        Diligent::IEngineFactoryD3D12* m_EngineFactory {nullptr};
        Diligent::IRenderDevice* m_RenderDevice {nullptr};
        Diligent::IDeviceContext* m_DeviceContext {nullptr};

        // Hold SceneWindow RenderTargets

        ImGuiImplRenderLib* m_ImGuiImpl {nullptr};
    };
}
