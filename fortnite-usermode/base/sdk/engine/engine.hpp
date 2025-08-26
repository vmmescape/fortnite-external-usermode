#include "../../entities/update/update.hpp"

namespace uengine
{
	class m_engine
	{
	public:

		class camera {
		public:
			static inline uemath::f_rotator rotation{ };
			static inline uemath::f_vector location{ };
			static inline float field_of_view;
		}; 

		static auto get_view_state() -> std::uintptr_t;

		static auto queuery_camera() -> void;

		static auto get_bone_index(int bone_id, std::uintptr_t ptr, uemath::ftransform component_to_world) -> uemath::f_vector;

		static auto world_to_screen(uemath::f_vector world_location) -> uemath::f_vector2d;

	}; inline m_engine engine_m;
}