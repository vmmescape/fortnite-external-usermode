#include <cstdint>
#include <memory>
#include "../../../thirdparty/vars/variables.hpp"

struct offsetss
{
public:

	// u_world : public u_object
	std::uintptr_t u_world = 0x14E2C5F8; //GWorld: 0x17FE0B70, 0x186C09C8 (both works)

	static constexpr std::uintptr_t game_state = 0x1B0;

	static constexpr std::uintptr_t game_instance = 0x228;

	static constexpr std::uintptr_t levels = 0x1e8;

	// u_game_instance : public u_object
	static constexpr std::uintptr_t actors = 0xe0;
	static constexpr std::uintptr_t actor_count = 0x1f8;
	static constexpr std::uintptr_t local_players = 0x38;

	// u_localplayer : public u_object
	static constexpr std::uintptr_t view_state = 0xd0;

	// class a_player_controller : public a_controller
	static constexpr std::uintptr_t player_controller = 0x30;

	static constexpr std::uintptr_t acknowledged_pawn = 0x350;

	static constexpr std::uintptr_t player_state = 0x2c8;

	static constexpr std::uintptr_t targeted_fort_pawn = 0x18d0;

	static constexpr std::uintptr_t relative_location = 0x138;

	// a_character : public a_pawn
	static constexpr std::uintptr_t root_component = 0x1B0;

	static constexpr std::uintptr_t component_velocity = 0x180;

	static constexpr std::uintptr_t current_weapon = 0xaf0;

	static constexpr std::uintptr_t weapon_data = 0x548;

	static constexpr std::uintptr_t ammo_count = 0x10ec;

	static constexpr std::uintptr_t weapon_core_animation = 0x15f8;

	// u_skeletal_mesh_component : public u_skinned_mesh_component
	static constexpr std::uintptr_t mesh = 0x328;

	static constexpr std::uintptr_t bone_space_transforms = 0x5e8;

	static constexpr std::uintptr_t is_cached = 0x5f8;

	static constexpr std::uintptr_t component_to_world = 0x1e0;

	static constexpr std::uintptr_t get_bounds = 0xf8;

	// a_player_state : public u_object
	static constexpr std::uintptr_t pawn_private = 0x320;

	static constexpr std::uintptr_t team_index = 0x1291;

	static constexpr std::uintptr_t b_is_dying = 0x728;

	static constexpr std::uintptr_t b_is_dbno = 0x9a1;

	// a_game_state_base : public u_object
	static constexpr std::uintptr_t server_world_time = 0x2e0; //

	static constexpr std::uintptr_t player_array = 0x2c0;

	// redacted : dump yourself
	static constexpr std::uintptr_t item_name = 0x40; //

	static constexpr std::uintptr_t data = 0x20; //

	static constexpr std::uintptr_t length = 0x28; //

	static constexpr std::uintptr_t weapon_rarity = 0xa2;

	static constexpr std::uintptr_t projectile_speed = 0x1fdc;

	static constexpr std::uintptr_t b_is_reloading = 0x399;

	static constexpr std::uintptr_t projectile_gravity_scale = 0x1fe0;

}; inline offsetss offsets;
