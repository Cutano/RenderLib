cmake -S .\ThirdParty\spdlog -B .\ThirdParty\spdlog\build -A x64
cmake -D DILIGENT_LOAD_PIX_EVENT_RUNTIME=TRUE -S .\ThirdParty\DiligentEngine -B .\ThirdParty\DiligentEngine\build\Win64 -A x64
