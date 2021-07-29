#include "misc/directx11.hh"
#include "misc/console.hh"
#include "misc/gui.hh"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#define LogStr(str) "DirectXHook: " str

// Define Present as a type
typedef HRESULT(__fastcall *IDXGISwapChain_Present)(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags);

IDXGISwapChain_Present gCleanPresent;

// The function which will get executed instead of Present
HRESULT __fastcall Present(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags)
{
    // First time we get called we want to do some setup
    static bool isFirstCall = true;
    if (isFirstCall)
    {
        isFirstCall = false;
        GuiSetup(swapChain);
    }

    GuiRender();

    return gCleanPresent(swapChain, syncInterval, flags);
}

// Our custom WndProc handler for grabbing the SwapChain vtable, does nothing
LRESULT CALLBACK DXWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

/*
 * Present is the DX function you want to hook if you want to render over your game's output
 * It's a method of IDXGSwapChain, so to obtain it's address we just make a temporary swap chain and steal it's vtable
 */
void HookPresent()
{
    // Create a custom window type
    WNDCLASSEXA windowClass =
    {
        sizeof(WNDCLASSEXA),
        CS_CLASSDC,
        DXWndProc,
        0,
        0,
        GetModuleHandleA(nullptr),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        "DX11Hook",
        nullptr
    };

    RegisterClassExA(&windowClass);

    // Create an instance of said window
    HWND windowHandle = CreateWindowA("DX11Hook", nullptr, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, nullptr, nullptr, windowClass.hInstance, nullptr);

    // Setup DirectX
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = 2;
    swapChainDesc.BufferDesc.Height = 2;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = windowHandle;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevelReq = D3D_FEATURE_LEVEL_11_0;
    UINT numFeatureLevels = 1;

    D3D_FEATURE_LEVEL  featureLevelSupported;
    HRESULT res;
    IDXGISwapChain *swapChain = nullptr;
    ID3D11Device *device = nullptr;
    ID3D11DeviceContext *ctx = nullptr;

    if (FAILED(res = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        &featureLevelReq,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        &featureLevelSupported,
        &ctx
    )))
    {
        ConsoleWriteColor(FOREGROUND_RED, LogStr("Failed to initialize DirectX!"));
        // TODO: DIE HERE
        return;
    }

    // The first sizeof(void*) bytes of a class always contain a pointer to it's vtable
    // which is basically an array of method pointers
    uintptr_t* vtable = reinterpret_cast<uintptr_t**>(swapChain)[0];
    ConsoleWriteColor(FOREGROUND_GREEN, LogStr("Found vtable: %llx\n"), vtable);

    // VTable is probably mapped as read-only
    DWORD oldProtect;
    VirtualProtect(vtable, sizeof(uintptr_t) * 10 /* Complete bullshit */, PAGE_READWRITE, &oldProtect);

    // Hook present
    gCleanPresent = reinterpret_cast<IDXGISwapChain_Present>(vtable[8]);
    ConsoleWriteColor(FOREGROUND_GREEN, LogStr("Found Present: %llx\n"), gCleanPresent);
    vtable[8] = reinterpret_cast<uintptr_t>(Present);

    // TODO: CLEAN UP THE WINDOW & DIRECTX MESS WE JUST MADE
}

void DXHookStart()
{
    ConsoleWriteColor(FOREGROUND_RED, LogStr("Hooking present\n"));
    HookPresent();
}

void DXHookStop()
{

}