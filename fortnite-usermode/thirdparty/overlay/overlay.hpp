#pragma once
#include <D3DX11tex.h>
#include <dwmapi.h>
#include <dxgi.h>
#include <d3d11.h>
#include <functional>
#include <iostream>
#include <thread>
#include "../../base/entities/features/visuals/visuals.hpp"


namespace render {

    class c_overlay {
    public:
        static inline MARGINS margin = { -1 };
        static inline HWND window_handle = nullptr;
        static inline ID3D11Device* device = nullptr;
        static inline  ID3D11DeviceContext* device_ctx = nullptr;
        static inline IDXGISwapChain* swap_chain = nullptr;
        static inline ID3D11RenderTargetView* render_target = nullptr;

        static inline bool menu_open = false;

        static inline bool create_overlay_contex( );

        static inline bool setup_imgui( );

        static inline void style_imgui( );

        static inline void toggle_input( bool state );

        static inline void draw_fps( );

        static inline void draw_menu( );

        static inline void render_loop( );

        static bool run( );
    };

} 