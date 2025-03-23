#include <iostream>
#include <chrono>
#include <thread>
#include <SDL.h>

using namespace std;

const int window_width = 800;
const int window_height = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer;

class Sprite
{
public:
	 // Texture texture;
	 int width;
	 int height;
	 
};

/*
   // examples of good use of enum classes (safe)
   enum class Animal { dog, deer, cat, bird, human };  // enum class
   enum class Mammal { kangaroo, deer, human };   
    Animal a = Animal::deer;
    Mammal m = Mammal::deer;
*/

enum class Shape { Rectangle, Circle }; 
	 
class GameObject
{
public:
	 Shape shape;
	 boolean moves;
	 
	 //Initializes the variables
	 Object();

	 //Takes key presses and adjusts the dot's velocity
	 //void handleEvent( SDL_Event& e );

	 void move( float timeStep );

	 // Use a method here, or a sprite?
	 
	 //Add to the scene. 
	 void render();

	 
	 float x, y;
	 float vx, vy;
	 float ax, ay; // accel needed here? or computed every frame?
private:

};

void init (int width, int height, SDL_Window** window, SDL_Renderer** renderer) {
	 if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		  cout << "Error initializing SDL: " << SDL_GetError() << endl;
		  exit(1);
     }

   	 int result = SDL_CreateWindowAndRenderer( width, height, NULL, window, renderer );
	 if ( result != 0 ) {
		  cout << "Failed to create window and renderer: " << SDL_GetError() << endl;
		  exit(1);
	 }
}

void shutdown() {
	 SDL_DestroyWindow( window );
	 SDL_Quit();
}

int main (int argc, char** argv)
{  
	 init( window_width, window_height, &window, &renderer);
	 /*

	   SDL_FillRect( winSurface, NULL, SDL_MapRGB( winSurface->format, 255, 0, 255 ) );
	   SDL_UpdateWindowSurface( window );
	 */
	 SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
	 //SDL_RenderDrawPoint( renderer, 100, 100 );
	 SDL_Rect r;
	 r.x = 20;
	 r.y = 20;
	 r.w = 100;
	 r.h = 100;
	 SDL_RenderFillRect( renderer, &r );

// Update window
	 SDL_RenderPresent( renderer );

	 /*************/
	 
	 SDL_Event ev;
	 bool running = true;

	 while ( running ) {
		  while ( SDL_PollEvent( &ev ) != 0 ) {
			   switch (ev.type) {
			   case SDL_QUIT:
					running = false;
					break;
			   }
		  }	
		  SDL_Delay(100);
	 }
	 shutdown();
	 return 0;
	 
}
