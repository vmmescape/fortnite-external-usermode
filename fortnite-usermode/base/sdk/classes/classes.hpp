#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <memory>
#include <stdexcept>
#include "../math/math.hpp"
#include "../offsets/offsets.hpp"
#include <unordered_set>
#define queuery(type, name, offset) type name() { return exc->read<type>( reinterpret_cast<uint64_t>( this ) + offset ); } 
#define queuery_bit(Bit, Name, Offset) bool Name( ) { return bool( exc->read<char>( reinterpret_cast<uint64_t>( this ) + Offset) & (1 << Bit)); }
#define apply_member(type, name, offset) void name( type val ) { exc->write<type>( reinterpret_cast<uint64_t>( this ) + offset, val); }

namespace sdk {

	class u_scene_component
	{
	public:
		queuery(uemath::f_vector, component_velocity, offsets.component_velocity);
		queuery(uemath::f_vector, relative_location, offsets.relative_location);


		queuery(uemath::f_vector, relative_rotation, 0x150);

	};

	class u_primitive_component : public u_scene_component
	{
	public:
	};

	class u_mesh_component : public u_primitive_component
	{
	public:
	};

	class u_skinned_mesh_component : public u_mesh_component
	{
	public:


	}; inline u_skinned_mesh_component skinned_mesh;

	class u_skeletal_mesh_component : public u_skinned_mesh_component
	{
	public:
		auto is_visible(uintptr_t uworld) -> bool
		{
			auto current_time = exc->read<double>(uworld + 0x180);

			auto mesh_render_time = exc->read<float>((uintptr_t)this + 0x32C);

			return (current_time - mesh_render_time) <= 0.06f;
		}

		uemath::f_vector get_bones(int id)
		{	
			uintptr_t bone_array = exc->read<uintptr_t>((uintptr_t(this) + 0x5E8));
			if (bone_array == NULL) {
				bone_array = exc->read<uintptr_t>((uintptr_t(this) + 0x5E8 + 0x10));
			}
			auto BoneTransform = exc->read<d3dmatrix::FTransform>(bone_array + (id * 0x60));
			d3dmatrix::FTransform ComponentToWorld2 = exc->read<d3dmatrix::FTransform>((uintptr_t)this + 0x1e0);
			D3DMATRIX Matrix = MatrixMultiplication1(BoneTransform.ToMatrixWithScale(), ComponentToWorld2.ToMatrixWithScale());
			return uemath::f_vector(Matrix._41, Matrix._42, Matrix._43);
		}


	};


	class a_actor : public u_object
	{
	public:
		queuery(u_scene_component*, root_component, offsets.root_component);
		queuery(uintptr_t, f_data, 0x20);
		queuery(int32_t, f_length, 0x28);
	};


	class f_text : public a_actor
	{
	public:
		queuery(a_actor*, name, 0x40);
		queuery(uintptr_t, data, 0x20);
		queuery(int32_t, length, 0x28);

	};


	struct a_controller : public a_actor
	{
	public:



	};

	struct u_fort_item_definition : public f_text
	{
		queuery(f_text*, display_name, 0x40);

	};

	struct u_fort_world_item_definition : public u_fort_item_definition
	{
	public:
		queuery(f_text*, weapon_display_name, offsets.item_name);

	};

	struct u_fort_weapon_item_defintion : public u_fort_world_item_definition
	{
	public:

	};


	struct u_fort_weapon_anim_set
	{
	public:

	};


	struct f_fort_ranged_weapon_recoil
	{
	public:

	};

	struct u_fort_weapon_mod_runtime_data : public u_object
	{
	public:

	};

	struct a_fort_weapon : public a_actor
	{
	public:

	};

	struct f_fort_item_entry
	{

	};

	class a_fort_pickup : public a_actor
	{
	public:

	};

	class a_pawn : public a_actor
	{
	private:


	public:

	};

	class a_character : public a_pawn
	{
	public:
		queuery(u_skeletal_mesh_component*, mesh, offsets.mesh);
	};

	struct afgf_character : public a_character
	{
	public:
	};


	struct a_fort_pawn : public afgf_character
	{
	public:

		queuery(f_text*, pickup_entry, 0x370 + 0x10);
		queuery(a_fort_weapon*, current_weapon, offsets.current_weapon);
	};

	struct f_replicated_athena_vehicle_attributes
	{
	public:
	};

	struct a_fort_athena_vehicle : public a_pawn
	{
	public:
	};

	struct u_character_movement_component : public u_object
	{
	public:

	};

	struct a_fort_player_pawn : public a_fort_pawn
	{
	public:
		queuery_bit(2, b_already_searched, 0xdd2);


		int get_item_index() const {

			return exc->read<int>(reinterpret_cast<uintptr_t>(this) + 0x18);
		}

		uintptr_t local_state()
		{			
			return exc->read<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x2c8);
		}

	};

	struct a_player_controller : public a_controller
	{
	public:

		queuery(a_fort_pawn*, targeted_fort_pawn, 0x18d0);
		queuery(a_fort_player_pawn*, acknowledged_pawn, offsets.acknowledged_pawn);
	};


	struct a_fort_player_pawn_athena : public a_fort_player_pawn
	{
	public:
		queuery(a_actor*, current_vehicle, 0x2d00);
		queuery_bit(1, b_is_crouched, 0x440);
		queuery_bit(4, b_is_sliding, 0x72b);
		queuery_bit(0, b_is_slope_sliding, 0x9ec);
		queuery(bool, b_is_skydiving, 0x2347);
		queuery(bool, b_is_parachute_open, 0x2348);
		queuery(a_player_controller*, controller, 0x2c8);
		queuery(e_fort_building_state, building_state, 0x2178);
	};

	class a_player_state : public u_object
	{
	public:
		queuery(a_fort_player_pawn_athena*, pawn_private, offsets.pawn_private);
		queuery(std::uint32_t, player_id, 0x2ac);

		queuery(std::uintptr_t, platform_info, 0x430);

		std::string get_platform()
		{	
			auto test_platform = platform_info();

			wchar_t platform[64] = { 0 };
			exc->read_array((test_platform), reinterpret_cast<uint8_t*>(platform), sizeof(platform));

			std::wstring platform_wstr(platform);
			std::string platform_str(platform_wstr.begin(), platform_wstr.end());

			return platform_str;
		}


	public:

	};

	class a_fort_player_state : public a_player_state
	{
	public:
	};


	class a_fort_player_state_zone : public a_fort_player_state
	{
	public:

	};


	class a_fort_player_state_athena : public a_fort_player_state_zone
	{
	public:

		queuery(std::int32_t, kill_score, 0x12A8);
		queuery(std::int32_t, season_level, 0x12ac);
		queuery(std::int32_t, team_index, offsets.team_index);

		queuery_bit(3, b_is_a_bot, 0x2b2);

		queuery_bit(4, b_is_dying, offsets.b_is_dying);
		queuery_bit(4, b_is_dbno, offsets.b_is_dbno);


	};

	struct u_scene_view_state : public u_object
	{
	public:
	};

	struct hehe : public u_object
	{
	public:

	};

	struct u_localplayer : public u_object
	{
	public:

		queuery(a_player_controller*, controller, offsets.player_controller);
		queuery(uemath::tarray<u_scene_view_state*>, view_state, offsets.view_state);
		queuery(hehe*, view_port_client, 0x78);

		uemath::tarray< std::uintptr_t > get_view_state( )
		{
			return  exc->read< uemath::tarray< std::uintptr_t > >( reinterpret_cast<uint64_t>( this ) + 0xD0 );
		}

	};


	struct u_level : public u_object
	{
	public:
		queuery( uemath::tarray<a_fort_player_pawn* >, actors, 0xa0 );
	};

	struct u_game_instance : public u_object
	{
	public:
		queuery( uemath::tarray<u_localplayer* >, localplayers, offsets.local_players );

		auto get_localplayer( ) -> u_localplayer*
		{	
			auto vec = localplayers( ).get_itter( );
			if ( vec.empty( ) ) return nullptr;
			return vec[ 0 ];
		}
	};

	class a_fort_safe_zone_indicator : public a_actor {
	public:

	};

	class u_fort_game_state_component_battle_royale_game_phase_logic : public u_object {
	public:

	};

	class u_athena_game_phase_logic_c : public u_fort_game_state_component_battle_royale_game_phase_logic {
	public:
	};


	struct a_game_state_base : public u_object
	{
	public:

		queuery( float, server_world_time, offsets.server_world_time );
		queuery( uemath::tarray<a_fort_player_state_athena*>, player_array, offsets.player_array );

		inline bool is_in_lobby( ) { return server_world_time() ? false : true; }

		auto get_user_name(a_fort_player_state_athena* state) -> std::string 
		{		
			__int64 f_text;
			int length;

			float in_lobby = exc->read<float>( ( uintptr_t )reinterpret_cast<uint64_t>( this ) + 0x2e0 );

			if ( in_lobby == 0.0f ) {
				length = exc->read< int >( std::uintptr_t( state + 0x340 + 0x8 ) );
				f_text = exc->read< __int64 >( std::uintptr_t( state + 0x340 ) );
			}
			else {
				auto f_string = exc->read< __int64 >( std::uintptr_t( state + 0xb40 ) );
				length = exc->read< int > (f_string + 16 );
				f_text = exc->read< __int64 >( f_string + 8 );
			}

			wchar_t* buffer = new wchar_t[ length ];
			exc->read_array( f_text, buffer, length * sizeof( wchar_t ) );

			std::int64_t v6 = length;
			if ( !v6 ) {
				delete[ ] buffer;
				return std::string( ( ( "AI" ) ) );
			}

			char v21 = v6 - 1;
			if ( ! ( std::uint32_t ) v6 ) v21 = 0;
			int v22 = 0;
			std::uint16_t* v23 = ( std::uint16_t* )buffer;

			for ( int i = v21 & 3; ; *v23++ += i & 7 ) {
				int v25 = v6 - 1;
				if ( !( std::uint32_t ) v6 ) v25 = 0;
				if ( v22 >= v25 ) break;
				i += 3;
				++v22;
			}

			std::wstring username( buffer );
			delete[ ] buffer;

			return std::string( username.begin( ), username.end( ) );
		}
	};

	struct u_world : public u_object
	{
	public:
		queuery( double, world_time_seconds, 0x198 );
		queuery( u_level*, persistent_level, 0x38 );

		queuery( uemath::tarray<u_level*>, item_levels, offsets.levels );

		queuery( a_game_state_base*, game_state, offsets.game_state );

		queuery( u_game_instance*, game_instance, offsets.game_instance );

		static inline auto queuery_uworld( ) -> u_world* {
			
			return exc->read<u_world*>( exc->image_base + offsets.u_world );
		}
	};

}
