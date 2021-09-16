#include "misc/directx11.hh"
#include "misc/console.hh"
#include "misc/gui.hh"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#define LogStr(str) "DirectXHook: " str

// Define Present as a type
typedef HRESULT(__fastcall *IDXGISwapChain_Present)(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags);
typedef HRESULT(__stdcall* IDXGISwapChain_ResizeBuffers)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT format, UINT flags);

IDXGISwapChain_Present gCleanPresent;
IDXGISwapChain_ResizeBuffers gCleanResizeBuffers;

// Allows us to catch window resizes
HRESULT __stdcall ResizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT format, UINT flags)
{
    if (d3dRenderTarget)
    {
        d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
        d3dRenderTarget->Release();
    }

    GuiPreResize(swapChain);
    auto result = gCleanResizeBuffers(swapChain, bufferCount, width, height, format, flags);
    
    ID3D11Texture2D* pBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
    
    ID3D11Device* device = nullptr;
    swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
    device->CreateRenderTargetView(pBuffer, NULL, &d3dRenderTarget);
    pBuffer->Release();

    d3dContext->OMSetRenderTargets(1, &d3dRenderTarget, NULL);

    // Set up the viewport.
    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    d3dContext->RSSetViewports(1, &vp);
    
    GuiPostResize(swapChain);


    return result;
}

// The function which will get executed instead of Present
HRESULT __fastcall Present(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags)
{
    static bool hasInitialized = false;
    if (!hasInitialized)
    {
        GuiSetup(swapChain);
        hasInitialized = true;
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
    swapChainDesc.OutputWindow = FindWindowA(nullptr, "NieR:Automata");
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevelReq = D3D_FEATURE_LEVEL_11_0;
    UINT numFeatureLevels = 1;

    D3D_FEATURE_LEVEL  featureLevelSupported;
    HRESULT res;
    IDXGISwapChain *swapChain = nullptr;
    ID3D11Device *device = nullptr;

    if (FAILED(res = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_REFERENCE,
        nullptr,
        0,
        &featureLevelReq,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        &featureLevelSupported,
        nullptr
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
    gCleanResizeBuffers = reinterpret_cast<IDXGISwapChain_ResizeBuffers>(vtable[13]);
    ConsoleWriteColor(FOREGROUND_GREEN, LogStr("Found Present: %llx\n"), gCleanPresent);
    vtable[8] = reinterpret_cast<uintptr_t>(Present);
    vtable[13] = reinterpret_cast<uintptr_t>(ResizeBuffers);

    // TODO: CLEAN UP THE WINDOW & DIRECTX MESS WE JUST MADE
    swapChain->Release();
    device->Release();
}

void DXHookStart()
{
    ConsoleWriteColor(FOREGROUND_RED, LogStr("Hooking present\n"));
    HookPresent();
}

void DXHookStop()
{

}