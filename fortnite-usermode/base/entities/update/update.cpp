#include "update.hpp"

namespace {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    constexpr auto update_interval = std::chrono::milliseconds( 400 );
}


void entities::m::run( )
{
    auto next_update = clock::now( );

    while ( true ) {
        const auto current_time = clock::now( );

        if ( current_time >= next_update ) {
            {
                std::unique_lock<std::mutex> lock( entities::m::mutex );
                entities::m::execute( );
            }

            next_update += update_interval;
            if ( next_update < current_time ) {
                next_update = current_time + update_interval;
            }
        }
        else {
            std::this_thread::sleep_until( next_update );
        }
    }
}

void entities::m::execute( ) {
    std::vector<entities::pointers> new_entities;

    entities::pointers::uworld = sdk::u_world::queuery_uworld( );
    entities::pointers::game_instance = entities::pointers::uworld->game_instance( );
    entities::pointers::game_state = entities::pointers::uworld->game_state( );
    entities::pointers::local_player = entities::pointers::game_instance->get_localplayer( );
    entities::pointers::player_controller =  entities::pointers::local_player->controller( );
    entities::pointers::acknowledged_pawn = entities::pointers::player_controller->acknowledged_pawn( );

    auto players = game_state->player_array( ).get_itter( );

    const std::size_t max_players = min( players.size( ), 200ul );

    for ( size_t i = 0; i < max_players; ++i ) {
        auto state = players[ i ];
        if ( !state ) continue;

        auto pawn = state->pawn_private( );
        if ( !pawn ) continue;

        if ( !pawn || pawn == entities::pointers::acknowledged_pawn )
            continue;

        auto mesh = pawn->mesh( );
        if ( !mesh ) continue;

        entities::pointers info{ };
        info.player_state = state;
        info.pawn_private = pawn;
        info.mesh = mesh;
        info.b_is_in_lobby = game_state->is_in_lobby( );

        if ( state ) { 
            info.season_level = state->season_level( );
            info.team_index = state->team_index( );
            info.kill_score = state->kill_score( );
        }

        if ( vars::visuals.username && state ) {
            auto cached_name = entities::m::m_e_cached_names.find( ( std::uintptr_t )state );
            if ( cached_name != entities::m::m_e_cached_names.end( ) ) {
                info.user_name = cached_name->second;
            }
            else {
                auto name = game_state->get_user_name( state ); 
                if ( !name.empty( ) ) {
                    entities::m::m_e_cached_names[ ( std::uintptr_t )state ] = name;
                    info.user_name = std::move( name );
                }
                else {

                }

            }
        }

        new_entities.emplace_back( std::move( info ) );
    }

    entities::m::entities_list = std::move( new_entities );
}




