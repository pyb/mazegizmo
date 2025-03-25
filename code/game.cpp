// Forked from Erin's box2d-raylib example

#include "raylib.h"
#include "box2d/box2d.h"

#include <stdio.h>
#include <iostream>
#include "getbno055.h"

#include <fstream>
#include <string>
using namespace std;

//#define SENSOR true

int verbose = 0;

const int screen_width = 800;
const int screen_height = 480;
const float ballDensity = 1.0f;
const float wallLengthV = .5f;
const float wallWidth = .020f;
const float ballRadius = .013f;
const b2Vec2 ballStart = (b2Vec2){0.0f, 0.0f};
//b2Vec2 gravity = {0.0f, -9.8f};
b2Vec2 gravity = {0.0f, 0.0f};
const float wallLengthH = wallLengthV * ((float)screen_width / (float)screen_height);
	 
const float pixelsPerMeter = screen_height / (wallLengthV);

const float drawTransformAxisScale = .05f;
const int sensorFramesSkipped = 10;

const float mazeX = -0.3f;
const float mazeY = -0.2f;
const float mazeWallLength = 0.034f;
const float mazeWallThickness = 0.01f;
const float kbdGravityStep = 0.1f;

// Sensor stuff. most of these are unused
int outflag = 0;
int argflag = 0; // 1 dump, 2 reset, 3 load calib, 4 write calib
char opr_mode[9] = {0};
char pwr_mode[8] = {0};
char datatype[256];
char senaddr[256] = "0x28";
char i2c_bus[256] = I2CBUS;

const int maze_width = 19;
const int maze_height = 13;

const int EMPTY = 0;
const int VERTWALL = 1;
const int HORWALL = 2;

int maze[maze_width][maze_height];

void initMaze()
{
	 int j = 0;
	 
	 std::ifstream infile("maze.txt");

	 std::string line;
	 while (std::getline(infile, line))
	 {
//		  cout << line << endl;
		  for (int i = 0 ; i < maze_width; i++)
		  {
			   char c = line.at(i);
			   int val;
			   switch(c)
			   {
			   case '.':
					val = EMPTY;
					break;
			   case 'I':
					val = VERTWALL;
					break;
			   case '-':
					val = HORWALL;
					break;
			   default:
					val = EMPTY;
					break;
			   }
			   maze[i][j] = val;
		  }
		  j++;
	 }
	 //maze[20][20] = 1;
	 //exit(0);
}

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
//	 printf("GRA %3.2f %3.2f %3.2f\n", (float)bnod.gravityx, (float)bnod.gravityy, bnod.gravityz);
	 return (b2Vec2){(float)bnod.gravityy, (float)bnod.gravityx}; // axes are inverted rn
}

b2DebugDraw m_debugDraw;

Vector2 v2(b2Vec2 vec) {
	 return (Vector2){vec.x, vec.y};
}

void drawPoly (const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context)
{
	 Vector2 from;
	 Vector2 to;
	 from = v2(vertices[0]);
	 to = from;
	 
	 for (int i = 1 ; i < vertexCount ; i++)
	 {

		  to = v2(vertices[i]);
		  DrawLineV(to, from, GetColor(color));
		  from = to;
	 }

	 to = v2(vertices[0]);
	 DrawLineV(from, to, GetColor(color));	 
}

void drawCircle (b2Vec2 center, float radius, b2HexColor color, void *context)
{
	 DrawCircleLinesV(v2(center), radius, GetColor(color));
}

void drawSolidCircle (b2Transform transform, float radius, b2HexColor color, void *context)
{
	 DrawCircleV(v2(transform.p), radius, GetColor(color));
}

void drawSolidPoly (b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius, b2HexColor color, void *context)
{
	 Vector2 from;
	 Vector2 to;
	 Vector2 tf = v2(transform.p);
	 from = v2(vertices[0]);
	 from.x += tf.x;
	 from.y += tf.y;
	 to = from;
	 
	 for (int i = 1 ; i < vertexCount ; i++)
	 {
		  to = v2(vertices[i]);
		  to.x += tf.x;
		  to.y += tf.y;
		  DrawLineV(to, from, GetColor(color));
		  from = to;
	 }

	 to = v2(vertices[0]);
	 to.x += tf.x;
	 to.y += tf.y;
	 DrawLineV(from, to, GetColor(color));
}

void DrawSolidCapsuleFcn( b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context )
{
	 cout << "Draw solid capsule" << endl;
}

void DrawSegmentFcn( b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context )
{
	 DrawLineV(v2(p1), v2(p2), GetColor(color));
}

void DrawTransformFcn( b2Transform transform, void* context )
{
	 Vector2 rX, rY;
	 rX = v2(b2Rot_GetXAxis(transform.q));
	 rY = v2(b2Rot_GetYAxis(transform.q));

	 Vector2 p, q1, q2;
	 p =  v2(transform.p);
	 q1.x = p.x + drawTransformAxisScale * rX.x;
	 q1.y = p.y + drawTransformAxisScale * rX.y;
	 DrawLineV(p, q1, RED);

	 q2.x = p.x + drawTransformAxisScale * rY.x;
	 q2.y = p.y + drawTransformAxisScale * rY.y;
	 DrawLineV(p, q2, GREEN);
}

void DrawPointFcn( b2Vec2 p, float size, b2HexColor color, void* context )
{
	 DrawCircleV(v2(p), size, GetColor(color));
//	 cout << "draw point " << p.x << " " << p.y << " " << size << endl;
}

void DrawStringFcn( b2Vec2 p, const char* s, b2HexColor color, void* context )
{
	 int fontSize = 2;
	 DrawText(s, p.x, p.y, fontSize, GetColor(color)); 
}


b2AABB bounds = { { -FLT_MAX, -FLT_MAX }, { FLT_MAX, FLT_MAX } };

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
	
	//m_debugDraw.drawingBounds = bounds;

	m_debugDraw.useDrawingBounds = false;
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

void buildMaze(b2WorldId worldId, float wallLength, float wallThickness, float startX, float startY)
{
	 b2BodyDef wallBodyDef = b2DefaultBodyDef();
	 b2ShapeDef wallShapeDef = b2DefaultShapeDef();

	 b2Vec2 wallHExtent = { wallLength/2, wallThickness/2};
	 b2Polygon wallH = b2MakeBox(wallHExtent.x, wallHExtent.y);
	 b2Polygon wallV = b2MakeBox(wallHExtent.y, wallHExtent.x);

	 for (int i = 0 ; i < maze_width ; i++)
	 {
		  for (int j = 0 ; j < maze_height ; j++)
		  {
			   if (maze[i][j] == EMPTY)
					continue;

			   wallBodyDef = b2DefaultBodyDef();
			   wallBodyDef.position = (b2Vec2){startX + i * wallLength, startY + j * wallLength};
			   wallBodyDef.enableSleep = false;
			   wallBodyDef.type = b2_staticBody;

			   wallShapeDef = b2DefaultShapeDef();
			   
			   b2BodyId wallId = b2CreateBody(worldId, &wallBodyDef);

			   if (maze[i][j] == HORWALL)
			   {
					b2CreatePolygonShape(wallId, &wallShapeDef, &wallH);
			   }
			   else if (maze[i][j] == VERTWALL)
			   {
					b2CreatePolygonShape(wallId, &wallShapeDef, &wallV);
			   }
			   else
			   {
					cout << "Error in maze construction!" << endl;
					exit(1);
			   }
		  }
	 }
}

int main(void)
{
	 initMaze();
#ifdef SENSOR
	 initSensor();
#endif
	 InitWindow(screen_width, screen_height, "Maze");

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
	 
	 b2Vec2 boxHExtent = { wallLengthH/2, wallWidth/2};
	 b2Vec2 boxVExtent = { wallLengthV/2, wallWidth/2};

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

	 buildMaze(worldId, mazeWallLength, mazeWallThickness, mazeX, mazeY);
	 
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
		  float deltaTime = GetFrameTime();
		  b2World_Step(worldId, deltaTime, 4);

		  BeginDrawing();
		  BeginMode2D(camera);
			
		  ClearBackground(DARKGRAY);

		  b2World_Draw(worldId, &m_debugDraw );
		  EndMode2D();
		  EndDrawing();
		  
	 }

	 /*
	   UnloadTexture(boxTexture);
	 */
	
	 CloseWindow();

	 return 0;
}
