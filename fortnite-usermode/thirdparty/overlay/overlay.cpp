#include "overlay.hpp"

bool render::c_overlay::create_overlay_contex( )
 {
     render::c_overlay::window_handle = FindWindowA( ( "AMDDVROVERLAYWINDOWCLASS" ), ( "amd dvr overlay" ) );
   
     DwmExtendFrameIntoClientArea( c_overlay::window_handle, &c_overlay::margin );
     SetLayeredWindowAttributes ( c_overlay::window_handle, RGB(0, 0, 0), 255, 0x00000002 );

     UpdateWindow( c_overlay::window_handle);
     ShowWindow( c_overlay::window_handle, 5);
     if (! c_overlay::window_handle )
         return false;
    
     DXGI_SWAP_CHAIN_DESC scd = { };
     scd.BufferCount = 2;
     scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
     scd.BufferDesc.RefreshRate = { 240, 1 };
     scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
     scd.OutputWindow = render::c_overlay::window_handle;
     scd.SampleDesc.Count = 1;
     scd.Windowed = TRUE;
     scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
     scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

     D3D_FEATURE_LEVEL levels[ ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
     D3D_FEATURE_LEVEL used_level;

     if ( FAILED( D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
         levels, 2, D3D11_SDK_VERSION, &scd, &c_overlay::swap_chain, &c_overlay::device, &used_level, &c_overlay::device_ctx ) ) )
         return false;

     ID3D11Texture2D* back_buffer = nullptr;
     if ( SUCCEEDED( render::c_overlay::swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) ) ) ) {
         render::c_overlay::device->CreateRenderTargetView( back_buffer, nullptr, &c_overlay::render_target );
         back_buffer->Release( );
     }

     device_ctx->OMSetRenderTargets( 1, &render::c_overlay::render_target, nullptr );

     return true;
}

bool render::c_overlay::setup_imgui() {
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( );
    io.IniFilename = nullptr;

    io.Fonts->AddFontDefault( );

    ImGui_ImplWin32_Init( render::c_overlay::window_handle );
    ImGui_ImplDX11_Init( render::c_overlay::device, render::c_overlay::device_ctx );

    render::c_overlay::style_imgui( );

    return true;
}

void render::c_overlay::style_imgui( )
{
    ImGuiStyle& style = ImGui::GetStyle( );
    auto& colors = style.Colors;

    colors[ ImGuiCol_WindowBg ] = ImColor( 40, 40, 40 );
    colors[ ImGuiCol_Button ] = ImColor( 40, 40, 40 );
    colors[ ImGuiCol_ButtonHovered ] = ImColor( 25, 25, 25 );
    colors[ ImGuiCol_ButtonActive ] = ImColor( 40, 40, 40 );
    colors[ ImGuiCol_Text ] = ImColor( 255, 255, 255 );
    colors[ ImGuiCol_CheckMark ] = ImColor( 255, 20, 147 );
    colors[ ImGuiCol_SliderGrab ] = ImColor( 255, 20, 147 );
    colors[ ImGuiCol_SliderGrabActive ] = ImColor( 255, 20, 147 );

    style.FramePadding = ImVec2( 3.f, 3.f );
    style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
    style.WindowBorderSize = 1.0f;
}

void render::c_overlay::toggle_input( bool state ) {
    LONG style = GetWindowLongA( window_handle, GWL_EXSTYLE );
    if ( state ) {
        style &= ~WS_EX_TRANSPARENT;
        style &= ~WS_EX_LAYERED;
    }
    else {
        style |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
    } 
    SetWindowLongA( window_handle, GWL_EXSTYLE, style );
}

void render::c_overlay::draw_fps( ) {
    auto draw_list = ImGui::GetForegroundDrawList( );
    ImGuiIO& io = ImGui::GetIO( );

    std::string text = "FPS: " + std::to_string( ( int )io.Framerate );
    ImVec2 pos = { 10, 10 };

    ImU32 bg = IM_COL32( 0, 0, 0, 255 );
    ImU32 fg = IM_COL32( 255, 20, 147, 255 );

    draw_list->AddText( { pos.x - 1, pos.y }, bg, text.c_str( ) );
    draw_list->AddText( { pos.x + 1, pos.y }, bg, text.c_str( ) );
    draw_list->AddText( { pos.x, pos.y - 1 }, bg, text.c_str( ) );
    draw_list->AddText( { pos.x, pos.y + 1 }, bg, text.c_str( ) );
    draw_list->AddText( pos, fg, text.c_str( ) );
}

void render::c_overlay::draw_menu( ) {
    ImGui::SetNextWindowSize( { 600, 400 } );
    ImGui::Begin( "usermode", nullptr, ImGuiWindowFlags_NoDecoration );
    ImGui::Text( "made ( in 5 h ) by vmescape <3");
    ImGui::End( );
}

void render::c_overlay::render_loop( ) {
    std::thread([ ]( ) { entities::m::run( ); }).detach( );

    while ( true ) {
        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        render::c_overlay::toggle_input( render::c_overlay::menu_open );

        render::c_overlay::draw_fps( );
        entities::visuals::execute( );

        if ( GetAsyncKeyState( VK_INSERT ) & 1 )
            render::c_overlay::menu_open = !render::c_overlay::menu_open;

        if ( render::c_overlay::menu_open ) {

            render::c_overlay::draw_menu( );
        }

        ImGui::Render( );
        const float clear_color[4] = { 0, 0, 0, 0 };
        render::c_overlay::device_ctx->OMSetRenderTargets( 1, &render::c_overlay::render_target, nullptr );
        render::c_overlay::device_ctx->ClearRenderTargetView( render::c_overlay::render_target, clear_color );
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData( ) );
        render::c_overlay::swap_chain->Present( 0, 0 );
    }
}

bool render::c_overlay::run( ) {
    if ( !render::c_overlay::create_overlay_contex( ) )
        return false;

    if ( !render::c_overlay::setup_imgui( ) )
        return false;

        render::c_overlay::render_loop( );
    return true;
}