#include <iostream>
#include <SDL.h>

using namespace std;

const int window_width = 800;
const int window_height = 600;

int main (int argc, char** argv)
{
     if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
	  cout << "Error initializing SDL: " << SDL_GetError() << endl;
	  return 1;
     }
     
}
