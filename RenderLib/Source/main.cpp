#include <vector>
#include <string>

#include "Platform/Application.h"

extern "C" {
	__declspec(dllexport) extern constexpr unsigned int D3D12SDKVersion = 606;
}

extern "C" {
	__declspec(dllexport) extern const char8_t* D3D12SDKPath = u8".\\D3D12\\";
}

// Args:
//	 0: EXE Path
//	 1: Workspace Dir
int main(const int argc, const char* const argv[])
{
	std::vector<std::string> args;
	args.reserve(argc);
	
	for (int i = 0; i < argc; ++i)
	{
		args.emplace_back(argv[i]);
	}
	
	auto* app = new RL::Application(args);
	app->Run();
	delete app;
	
	return 0;
}