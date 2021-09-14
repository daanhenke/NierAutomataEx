#include "misc/gui.hh"
#include "misc/console.hh"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "features/debuggui.hh"

#define LogStr(str) "GUI: " str

IDXGISwapChain *swapChain;
ID3D11Device *d3dDevice;
ID3D11DeviceContext *d3dContext;
ID3D11RenderTargetView *d3dRenderTarget;

bool showMenu = false;

// ImGui doesn't export this by default, so we define it ourself
IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WNDPROC gOriginalWndProc;
LRESULT CALLBACK GuiWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Handle toggling the gui
    if (msg == WM_KEYUP && wParam == VK_INSERT) showMenu = !showMenu;

    if (showMenu)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
        return 1;
    }

    // Make sure the game still receives input
    return CallWindowProcA(gOriginalWndProc, hWnd, msg, wParam, lParam);
}

void GuiSetup(IDXGISwapChain *pChain)
{
    ConsoleWrite(LogStr("Setting up ImGui\n"));

    swapChain = pChain;

    DXGI_SWAP_CHAIN_DESC desc;
    swapChain->GetDesc(&desc);

    HWND gameWindowHandle = desc.OutputWindow;

    // Hook the game window's WndProc so we can handle input
    gOriginalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(gameWindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(GuiWndProc)));

    // Get the ID3D11Device & context from the current swapchain
    HRESULT res = swapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<PVOID*>(&d3dDevice));
    if (FAILED(res))
    {
        ConsoleWriteError(LogStr("Failed to get D3D11Device!\n"));
        return;
    }

    d3dDevice->GetImmediateContext(&d3dContext);

    // Create render target
    ID3D11Texture2D* buffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&buffer));
    d3dDevice->CreateRenderTargetView(buffer, nullptr, &d3dRenderTarget);
    buffer->Release();

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // We are gaming
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    io.Fonts->AddFontDefault();

    ImGui_ImplWin32_Init(gameWindowHandle);
    ImGui_ImplDX11_Init(d3dDevice, d3dContext);
}

void GuiRender()
{
    if (showMenu)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow(&showMenu);
        DebugGuiRender();

        ImGui::EndFrame();
        ImGui::Render();

        d3dContext->OMSetRenderTargets(1, &d3dRenderTarget, nullptr);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void GuiLogPtr(std::string label, void* ptr)
{
    ImGui::InputScalar(label.c_str(), ImGuiDataType_U64, &ptr, 0, 0, "%llx", ImGuiInputTextFlags_CharsHexadecimal);
}