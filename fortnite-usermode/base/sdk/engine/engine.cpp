#include "engine.hpp"

auto uengine::m_engine::get_view_state( ) -> std::uintptr_t {
	uemath::tarray< std::uintptr_t > view_state = entities::pointer.local_player->get_view_state( );
	return view_state.get( 1 );
}

void uengine::m_engine::queuery_camera( ) {
	if ( !get_view_state( ) )
		return;

	auto projection = exc->read< uemath::fmatrix >( get_view_state( ) + 0x930 );
	auto radians = 2.0f * std::atanf( 1.0f / static_cast< float > ( exc->read< double >( get_view_state( ) + 0x730 ) ) );

	uengine::m_engine::camera::rotation.pitch = std::asin( projection.z_plane.w ) * 180.0f / std::numbers::pi;
	uengine::m_engine::camera::rotation.yaw = std::atan2( projection.y_plane.w, projection.x_plane.w ) * 180.0f / std::numbers::pi;
	uengine::m_engine::camera::rotation.roll = 0.0f;

	uengine::m_engine::camera::location.x = projection.m[ 3 ][ 0 ];
	uengine::m_engine::camera::location.y = projection.m[ 3 ][ 1 ];
	uengine::m_engine::camera::location.z = projection.m[ 3 ][ 2 ];

	uengine::m_engine::camera::field_of_view = radians * 180.0f / m_pi;
}

uemath::f_vector2d uengine::m_engine::world_to_screen( uemath::f_vector world_location ) {
	D3DMATRIX rotation_matrix = uemath::rotation_matrix( uengine::m_engine::camera::rotation );

	auto& axis_x = rotation_matrix.m[ 0 ];
	auto& axis_y = rotation_matrix.m[ 1 ];
	auto& axis_z = rotation_matrix.m[ 2 ];

	uemath::f_vector delta = world_location - uengine::m_engine::camera::location;
	auto transformed_x = delta.dot( uemath::f_vector( axis_y[ 0 ], axis_y[ 1 ], axis_y[ 2 ] ) );
	auto transformed_y = delta.dot( uemath::f_vector( axis_z[ 0 ], axis_z[ 1 ], axis_z[ 2 ] ) );
	auto transformed_z = max( delta.dot( uemath::f_vector( axis_x[ 0 ], axis_x[ 1 ], axis_x[ 2 ] ) ), 1.0f );

	float fov_rad = uengine::m_engine::camera::field_of_view * std::numbers::pi / 180.0f;
	float fov_tan = tanf( fov_rad / 2.0f );

	float screen_x = vars::screen.screen_center_w + ( transformed_x / fov_tan ) * vars::screen.screen_center_w / transformed_z;
	float screen_y = vars::screen.screen_center_h - ( transformed_y / fov_tan ) * vars::screen.screen_center_w / transformed_z;

	return uemath::f_vector2d( screen_x, screen_y );
}

