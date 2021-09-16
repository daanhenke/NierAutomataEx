#pragma once

#include <d3d11.h>
#include <string>

void GuiSetup(IDXGISwapChain *pChain);
void GuiPreResize(IDXGISwapChain* pChain);
void GuiPostResize(IDXGISwapChain* pChain);
void GuiRender();

void GuiLogPtr(std::string label, void* ptr);

extern ID3D11Device *d3dDevice;
extern ID3D11DeviceContext *d3dContext;
extern ID3D11RenderTargetView *d3dRenderTarget;