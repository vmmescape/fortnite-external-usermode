#include "visuals.hpp"

void entities::visuals::execute( ) {
 
    auto draw_list = ImGui::GetBackgroundDrawList( );
    if ( !draw_list )
        return;

    uengine::m_engine::queuery_camera( );

    for ( const auto& info : entities::m::entities_list ) {

        auto player_state = info.player_state;
        if ( !player_state ) continue;

        auto pawn = info.pawn_private;
        if ( !pawn ) continue;

        auto mesh = info.mesh;
        if ( !mesh ) continue;

        auto head = mesh->get_bones( 110 );
        auto root = mesh->get_bones( 0 );

        auto distance = uengine::m_engine::camera::location.distance( root ) / 100.0f;
        if ( distance >= 275 ) continue;

        auto head2d = uengine::m_engine::world_to_screen( { head.x, head.y, head.z } );
        auto root2d = uengine::m_engine::world_to_screen( { root.x, root.y, root.z } );

        auto head_ = uengine::m_engine::world_to_screen( { head.x, head.y, head.z + 15 } );

        float height = std::abs( head_.y - root2d.y );
        float width = height * 0.5f;

        bool is_visible = { };

        auto box_color = entities::visuals::f_color( 255, 40, 140, 255 );

        draw_list->AddCircle( ImVec2( 1920 / 2.f, 1080 / 2.f ), 300.f, ImColor( 255, 255, 255, 120 ), 100, 1.5f );

        if ( vars::visuals.visible_check ) {
            is_visible = mesh->is_visible( ( uintptr_t )entities::pointers::uworld );
        }

        if ( vars::visuals.box ) {
            entities::visuals::rect( draw_list, head_.x - width * 0.5f, head_.y, width, height, box_color, 1 );
        }

    }
    
}