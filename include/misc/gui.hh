#pragma once

#include <d3d11.h>

void GuiSetup(IDXGISwapChain *pChain);
void GuiRender();

extern ID3D11Device *d3dDevice;
extern ID3D11DeviceContext *d3dContext;
extern ID3D11RenderTargetView *d3dRenderTarget;