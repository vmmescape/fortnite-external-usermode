#include <mutex>
#include "../../sdk/classes/classes.hpp"

namespace entities {

    class pointers : public sdk::u_world {
    public:

        static uemath::tarray<sdk::u_level*> item_levels;

        static inline sdk::u_world* uworld;
        static inline sdk::u_level* persistent_level;
        static inline sdk::a_game_state_base* game_state;
        static inline sdk::u_game_instance* game_instance;
        sdk::u_skeletal_mesh_component* mesh;
        sdk::a_fort_player_pawn_athena* pawn_private;
        static inline sdk::u_scene_view_state* view_state;
        static inline sdk::u_localplayer* local_player;
        static inline sdk::a_player_controller* player_controller;
        static inline sdk::a_fort_player_pawn* acknowledged_pawn;
        sdk::a_fort_player_state_athena* player_state;
        sdk::a_fort_weapon* current_weapon;
        sdk::f_text* weapon_display_name;
        sdk::a_actor* vehicle;
        sdk::u_fort_weapon_item_defintion* weapon_data;
        static inline uintptr_t local_state;
        static inline uint32_t local_team_index;
        static inline uint32_t local_squad_index;

        sdk::e_fort_rarity tier, local_tier;
        std::uintptr_t data, local_data, bones;

        sdk::u_fort_weapon_item_defintion* local_weapon_data;
        sdk::a_fort_weapon* local_current_weapon;
        sdk::f_text* local_weapon_display_name;

        std::string user_name, platform, weapon_name, local_weapon_name;
        std::int32_t kill_score, season_level, team_index, ammo_count, lenght, local_lenght, rank_progress;
        bool b_is_in_lobby;
    } static inline pointer;

	class m : public pointers {
	private:

        static inline std::mutex m_e_list_cache_mutex;
	public:
        static inline std::unordered_map<std::uintptr_t, std::string> m_e_cached_names;
        static inline std::mutex mutex;
        static inline std::mutex entity_mutex;
        static inline std::vector<pointers> entities_list;
        static void execute( );
        static void actors( );
        static void run( ) ;
	};

}