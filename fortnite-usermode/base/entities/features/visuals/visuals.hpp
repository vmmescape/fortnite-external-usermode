#include "../../../sdk/engine/engine.hpp"
#include "../../../../thirdparty/imgui/imgui_internal.h"
#include "../../../../thirdparty/imgui/imgui_impl_dx11.h"
#include "../../../../thirdparty/imgui/imgui_impl_win32.h"
#include <d3d11.h>

namespace entities {

    class visuals : public m {
    private:

    public:

		static void rect( ImDrawList* draw_list,float x,float y,float width,float height,uemath::flinearcolor color,int thickness ) {

			if ( true ) {
				ImU32 im_color = IM_COL32( color.r, color.g, color.b, color.a );
				draw_list->AddRect( ImVec2( x - 1, y - 1 ), ImVec2( x + width + 1, y + height + 1 ), ImColor( 0, 0, 0, 255 ), 0, 0, thickness );
			}

			ImU32 im_color = IM_COL32( color.r, color.g, color.b, color.a );
			draw_list->AddRect( ImVec2( x, y ), ImVec2( x + width, y + height ), im_color, 0, 0, thickness );
		}

		static auto f_color( float r, float g, float b, float a ) -> uemath::flinearcolor {
			return uemath::flinearcolor { r,g,b,a };
		}

        static void execute( );

    };

}