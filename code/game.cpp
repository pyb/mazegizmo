// Forked from Erin's box2d-raylib example

#include "raylib.h"
#include "box2d/box2d.h"

#include <fstream>
#include <string>

#include <stdio.h>
#include <iostream>
#include "getbno055.h"

#include "debugdraw.h"

using namespace std;

//#define SENSOR true

int verbose = 0;

const int screen_width = 800;
const int screen_height = 480;
const float ballDensity = 1.0f;
const float wallLengthV = .5f;
const float wallWidth = .020f;
const float ballRadius = .013f;
const float trapCatchRadius = 0.03f;

const b2Vec2 ballStart = (b2Vec2){0.0f, 0.0f};
#ifdef PC
b2Vec2 gravity = {0.0f, -1.0f};
#else
b2Vec2 gravity = {0.0f, 0.0f};
#endif
const float wallLengthH = wallLengthV * ((float)screen_width / (float)screen_height);
	 
const float pixelsPerMeter = screen_height / (wallLengthV);

const int sensorFramesSkipped = 10;

const float kbdGravityStep = 0.02f;

// Sensor stuff. most of these are unused
int outflag = 0;
int argflag = 0; // 1 dump, 2 reset, 3 load calib, 4 write calib
char opr_mode[9] = {0};
char pwr_mode[8] = {0};
char datatype[256];
char senaddr[256] = "0x28";
char i2c_bus[256] = I2CBUS;

void initSensor()
{
	 get_i2cbus(i2c_bus, senaddr);
	 cout << "init sensor..." << endl;
	 set_mode(ndof);
}

b2Vec2 readGravity()
{
	 int res = -1;
	 struct bnogra bnod;
	 res = get_gra(&bnod);
	 if(res != 0) {
		  printf("Error: Cannot read gravity vector data.\n");
		  exit(-1);
	 }
	 return (b2Vec2){(float)bnod.gravityy, (float)bnod.gravityx}; // axes are inverted rn
}

b2DebugDraw m_debugDraw;

b2AABB bounds = { { -FLT_MAX, -FLT_MAX }, { FLT_MAX, FLT_MAX } };

const int MAX_TRAPS = 100;

struct trap {
	 float x;
	 float y;
	 float diameter;
	 float angle;
	 float angleRange;
	 float speed;
	 float delay;
};

int ntraps = 0;
struct trap traps[MAX_TRAPS];

void initDebug() {
	m_debugDraw = {};
	m_debugDraw.DrawCircle = drawCircle;
	m_debugDraw.DrawPolygon = drawPoly;
	m_debugDraw.drawShapes = true;

	m_debugDraw.DrawSolidPolygon = drawSolidPoly;
	m_debugDraw.DrawSolidCircle = drawSolidCircle;
	
	m_debugDraw.DrawSolidCapsule = DrawSolidCapsuleFcn;
	m_debugDraw.DrawSegment = DrawSegmentFcn;
	m_debugDraw.DrawTransform = DrawTransformFcn;
	m_debugDraw.DrawPoint = DrawPointFcn;
	m_debugDraw.DrawString = DrawStringFcn;
	
	m_debugDraw.drawingBounds = bounds;

	m_debugDraw.useDrawingBounds = true;
	m_debugDraw.drawShapes = true;
	m_debugDraw.drawJoints = false;
	m_debugDraw.drawJointExtras = false;
	m_debugDraw.drawAABBs = true;
	m_debugDraw.drawMass = true;
	m_debugDraw.drawContacts = true;
	m_debugDraw.drawGraphColors = true;
	m_debugDraw.drawContactNormals = true;
	m_debugDraw.drawContactImpulses = true;
	m_debugDraw.drawFrictionImpulses = true;

	m_debugDraw.context = nullptr;
}

void addTrap (struct trap t)
{
	 // Fire back the ball after <delay> towards (<angle>, <speed>) with +-<angleRange> randomness
	 if (ntraps == MAX_TRAPS - 1)
		  return;
	 traps[ntraps] = t;
	 ntraps++;
}

void addTraps()
{
	 struct trap trap1;
	 trap1.x = -0.06;
	 trap1.y = -0.04;
	 trap1.diameter = 0.02;
	 trap1.angle = -3.14/4;
	 trap1.angleRange;
	 trap1.speed = 10.0;
	 trap1.delay = 2.0;
	 addTrap(trap1);
}

void initTraps(b2WorldId worldId)
{
	 for (int i = 0; i < ntraps ; i++)
	 {
		  struct trap t = traps[i];
		  b2BodyDef trapBodyDef = b2DefaultBodyDef();
		  trapBodyDef.type = b2_staticBody;
		  trapBodyDef.position = (b2Vec2){t.x, t.y};
		  trapBodyDef.enableSleep = false;
		  trapBodyDef.isEnabled = true;

		  b2BodyId trapBodyId = b2CreateBody(worldId, &trapBodyDef);

		  b2ShapeDef trapShapeDef = b2DefaultShapeDef();
		  b2Circle circle;
		  circle.center = (b2Vec2){t.x, t.y};
		  circle.radius = t.diameter;
		  b2CreateCircleShape(trapBodyId, &trapShapeDef, &circle);
	 }
}

void trapBall (int trapId)
{
	 cout << "trapped!" << endl;
}

void releaseBall (int trapId)
{

}

int main(void)
{
#ifdef SENSOR
	 initSensor();
#endif
	 InitWindow(screen_width, screen_height, "Ball Game");

	 Camera2D camera = { 0 };
	 camera.target = (Vector2){ 0.0f, 0.0f };
	 camera.offset = (Vector2){ screen_width/2.0f, screen_height/2.0f };
	 camera.rotation = 0.0f;
	 camera.zoom = pixelsPerMeter;
	
	 SetTargetFPS(60);

	 initDebug();

	 b2WorldDef worldDef = b2DefaultWorldDef();
	 worldDef.gravity.x = gravity.x;
	 worldDef.gravity.y = gravity.y;
	 b2WorldId worldId = b2CreateWorld(&worldDef);

	 b2BodyDef wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){0.0f, -wallLengthV/2 + wallWidth/2};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall0Id = b2CreateBody(worldId, &wallBodyDef);

	 wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){-wallLengthH/2 + wallWidth/2, 0};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall1Id = b2CreateBody(worldId, &wallBodyDef);

	 wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){0.0f, wallLengthV/2 - wallWidth/2};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall2Id = b2CreateBody(worldId, &wallBodyDef);

	 wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){wallLengthH/2 - wallWidth/2, 0};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall3Id = b2CreateBody(worldId, &wallBodyDef);
	 
	 b2Vec2 boxHExtent = { wallLengthH/2, wallWidth/2 };
	 b2Vec2 boxVExtent = { wallLengthV/2, wallWidth/2 };

	 b2Polygon wallHBox = b2MakeBox(boxHExtent.x, boxHExtent.y);
	 b2Polygon wallVBox = b2MakeBox(boxVExtent.y, boxVExtent.x);
	 b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	 b2CreatePolygonShape(wall0Id, &groundShapeDef, &wallHBox);
	 b2CreatePolygonShape(wall1Id, &groundShapeDef, &wallVBox);
	 b2CreatePolygonShape(wall2Id, &groundShapeDef, &wallHBox);
	 b2CreatePolygonShape(wall3Id, &groundShapeDef, &wallVBox);
	 
	 b2BodyDef ballBodyDef = b2DefaultBodyDef();
	 ballBodyDef.type = b2_dynamicBody;
	 ballBodyDef.position = ballStart;
	 ballBodyDef.enableSleep = false;
	 b2BodyId ballBodyId = b2CreateBody(worldId, &ballBodyDef);

	 b2ShapeDef ballShapeDef = b2DefaultShapeDef();
	 ballShapeDef.density = ballDensity;
	 ballShapeDef.friction = 0.06f;
	 b2Circle circle;
	 circle.center = (b2Vec2){0.0f, 0.0f};
	 circle.radius = ballRadius;
	 b2CreateCircleShape(ballBodyId, &ballShapeDef, &circle);

	 addTraps();
	 initTraps(worldId);
	 
	 bool pause = false;

	 int k = 0;
	 while (!WindowShouldClose())
	 {
		  if (IsKeyDown(KEY_UP))
		  {
			   gravity.y -= kbdGravityStep;
			   b2World_SetGravity (worldId, gravity);
		  }
		  else if (IsKeyDown(KEY_DOWN))
		  {
			   gravity.y += kbdGravityStep;
			   b2World_SetGravity (worldId, gravity);
		  }
		  else if (IsKeyDown(KEY_LEFT))
		  {
			   gravity.x -= kbdGravityStep;
			   b2World_SetGravity (worldId, gravity);
		  }
		  else if (IsKeyDown(KEY_RIGHT))
		  {
			   gravity.x += kbdGravityStep;
			   b2World_SetGravity (worldId, gravity);
		  }
#ifdef SENSOR
		  else if (k++ == sensorFramesSkipped)
		  {// read rate?
			   k = 0;
			   gravity = readGravity();
			   b2World_SetGravity (worldId, gravity);
		  }
#endif
		  // Caught in traps?
		  {
			   b2Vec2 position = b2Body_GetPosition(ballBodyId);
			   float x = position.x;
			   float y = position.y;
			   
			   for (int i = 0; i < ntraps ; i++)
			   {
					float xt = traps[i].x;
					float yt = traps[i].y;
					
					if ( ( (x-xt)*(x-xt) + (y-yt)*(y-yt) ) < (trapCatchRadius * trapCatchRadius) )
					{
						 trapBall(i);
						 cout << x << " " << y << endl;
						 cout << xt << " " << yt << endl;
					}
			   }
		  }
		  
		  float deltaTime = GetFrameTime();
		  b2World_Step(worldId, deltaTime, 4);
//		  cout << "" << endl;

		  BeginDrawing();
		  BeginMode2D(camera);
			
		  ClearBackground(DARKGRAY);

		  b2World_Draw(worldId, &m_debugDraw );
		  EndMode2D();
		  EndDrawing();	  
	 }
	 
	 CloseWindow();
	 return 0;
}
