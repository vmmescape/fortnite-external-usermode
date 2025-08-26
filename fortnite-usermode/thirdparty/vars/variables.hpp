#include <string>
#include <Windows.h>

class vars
{
public:

	struct {

	} aimbot;

	struct {
		bool username = false;
		bool visible_check = true;
		bool box = true;
	} static inline visuals;

	struct {

	} world;

	struct {
		const int screen_width = GetSystemMetrics( 0 );
		const int screen_height = GetSystemMetrics( 1 );

		const int screen_center_w = screen_width / 2;
		const int screen_center_h = screen_height / 2;
	} static inline screen;

};