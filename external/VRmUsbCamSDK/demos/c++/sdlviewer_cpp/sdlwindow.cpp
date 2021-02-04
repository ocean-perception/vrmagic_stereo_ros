// ==============================================================================================
// This file is part of the VRmagic USB2.0 Camera Demo Application
// ==============================================================================================
// SDL Window
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>

#ifdef __linux__
#include  <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#ifdef __arm__
#include <unistd.h>
#include <termios.h>

struct termios initial_settings, new_settings;
#endif

#ifdef D2_PLATFORM
const int pixel_depth= 16;
#else
const int pixel_depth= 32;
#endif

SDL_Surface* surface;
SDL_Surface* screen;

bool g_use_sysmem;

using namespace VRmUsbCamCPP;

bool SDLWindowInit( const char* fp_caption, ImageFormat f_format)
{

	//set some environment vars - not recommended by SDL (because of platform compatibility)
#ifdef __arm__
	SDL_putenv((char*)"SDL_NOMOUSE=1");
#endif

#ifdef D2_PLATFORM
	SDL_putenv((char*)"SDL_VIDEODRIVER=directfb");
#endif

	// initialize the SDL library
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "Failed to initialize the SDL library: " << SDL_GetError() << std::endl;
		return false;
	}

	// set flags
	int flags = 0;

	// switch for system(SW) or video memory(HW)
	flags |= (g_use_sysmem)? SDL_SWSURFACE : SDL_HWSURFACE;

	SDL_WM_SetCaption( fp_caption, NULL );

	// create the window
#ifdef D3_PLATFORM
	SDL_Rect **screen_rect;
	screen_rect = SDL_ListModes(NULL, flags);
	screen = SDL_SetVideoMode( screen_rect[0]->w, screen_rect[0]->h, pixel_depth, flags );
#else
	screen = SDL_SetVideoMode(  f_format.get_Size().m_width, f_format.get_Size().m_height, pixel_depth, flags  );
#endif

	if ( screen == NULL )
	{
		std::cout << "Failed to set the SDL video mode: "  << SDL_GetError() << std::endl;
		return false;
	}

#ifdef D2_PLATFORM
	Uint32 rmask = 0xF800;
	Uint32 gmask = 0x07E0;
	Uint32 bmask = 0x001F;
	Uint32 amask = 0x0000;
#else
	Uint32 rmask = 0x00ff0000;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x000000ff;
	Uint32 amask = 0xff000000;
#endif

	surface = SDL_CreateRGBSurface( flags, f_format.get_Size().m_width, f_format.get_Size().m_height, pixel_depth, rmask, gmask, bmask, amask );

#ifdef __arm__
	// Prepare kbhit() for console inputs (
	tcgetattr(0, &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
#endif

	return true;
}


void 	SDLWindowClose()
{
	if ( surface != NULL )
	{
		SDL_FreeSurface( surface );
		surface = NULL;
	}
	if ( screen != NULL )
	{
		SDL_FreeSurface( screen );
		screen = NULL;
	}

	SDL_Quit();

#ifdef __arm__
	// Restore initial keyboard settings of the remote console
	tcsetattr(0, TCSANOW, &initial_settings);
#endif
}

unsigned char* SDLLockBuffer( unsigned int& f_pitch )
{
	SDL_LockSurface(surface);

	// offset to same pixel in next row (32bit surface)
	f_pitch= surface->pitch;
	return static_cast<unsigned char*>(surface->pixels);
}

void SDLUnlockBuffer()
{
	SDL_UnlockSurface(surface);
}

void SDLUpdate()
{
	SDL_BlitSurface(surface, 0, screen, 0);
	SDL_UpdateRect( screen, 0, 0, 0, 0);
}

bool kbhit()
{
#ifdef __arm__
	// Check for remote console keyboard input
	unsigned char ch;
	int nread;

	new_settings.c_cc[VMIN]=0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0, &ch, 1);
	new_settings.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_settings);
	if (nread == 1)
		return true;
#endif

	return false;
}
