#include "raylib.h"
#include "box2d/box2d.h"

#include <assert.h>
#include <stdio.h>
#include <iostream>

using namespace std;

// Forked from Erin's box2d-raylib example

const int screen_width = 800;
const int screen_height = 480;

typedef struct Entity
{
	b2BodyId bodyId;
	b2Vec2 extent;
	Texture texture;
} Entity;

void DrawEntity(const Entity* entity)
{
	 /*
	   // Disabled for now
	// The boxes were created centered on the bodies, but raylib draws textures starting at the top left corner.
	// b2Body_GetWorldPoint gets the top left corner of the box accounting for rotation.
	b2Vec2 p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2) { -entity->extent.x, -entity->extent.y });
	b2Rot rotation = b2Body_GetRotation(entity->bodyId);
	float radians = b2Rot_GetAngle(rotation);

	Vector2 ps = {p.x, p.y};
	DrawTextureEx(entity->texture, ps, RAD2DEG * radians, 1.0f, WHITE);
	 */
}

b2DebugDraw m_debugDraw;


void drawPoly (const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context)
{
	 printf("Draw poly\n");
	 for (int i = 0 ; i < vertexCount ; i++)
	 {
		  //
	 }
	 	 
}

void drawCircle (b2Vec2 center, float radius, b2HexColor color, void *context)
{
//void DrawCircle(int centerX, int centerY, float radius, Color color);
	 DrawCircle(center.x, center.y, radius, GetColor(color));    
}

void drawSolidCircle (b2Transform transform, float radius, b2HexColor color, void *context)
{
	 printf("Draw solid circle\n");
	 cout << transform.p.x << " " << transform.p.y << " " << radius << endl;
//	 DrawCircle(transform.p.x, transform.p.y, radius, GetColor(color));
	 DrawCircle(transform.p.x, transform.p.y, radius, RED);
//	 DrawCircle(100, 100, 100, RED);
}

void drawSolidPoly (b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius, b2HexColor color, void *context)
{
	 printf("Draw solid poly\n");
//	 drawPoly(vertices, vertexCount, color, context);
}


void DrawSolidCapsuleFcn( b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context )
{
	 printf("Draw solid capsule\n");

}

void DrawSegmentFcn( b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context )
{
	 printf("Draw segment\n");

}

void DrawTransformFcn( b2Transform transform, void* context )
{
	 printf("Draw transform\n");

}

void DrawPointFcn( b2Vec2 p, float size, b2HexColor color, void* context )
{
	 printf("Draw point\n");

}

void DrawStringFcn( b2Vec2 p, const char* s, b2HexColor color, void* context )
{
	 printf("Draw string\n");

}


b2AABB bounds = { { -FLT_MAX, -FLT_MAX }, { FLT_MAX, FLT_MAX } };

void init_debug() {
	m_debugDraw = {};
	m_debugDraw.DrawCircle = drawCircle;
	m_debugDraw.DrawPolygon = drawPoly;
//	m_debugDraw.DrawSegment = DrawSegmentFcn;
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
	m_debugDraw.drawAABBs = false;
	m_debugDraw.drawMass = false;
	m_debugDraw.drawContacts = false;
	m_debugDraw.drawGraphColors = false;
	m_debugDraw.drawContactNormals = false;
	m_debugDraw.drawContactImpulses = false;
	m_debugDraw.drawFrictionImpulses = false;

	m_debugDraw.context = nullptr;

}

int main(void)
{
	 init_debug();
	 InitWindow(screen_width, screen_height, "Maze");
	 SetTargetFPS(60);
	
	 float lengthUnitsPerMeter = 10.0f;
	 b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

	 b2WorldDef worldDef = b2DefaultWorldDef();
	 worldDef.gravity.y = 9.8f * lengthUnitsPerMeter;
	 //worldDef.gravity.y = -9.8f;
	 b2WorldId worldId = b2CreateWorld(&worldDef);

	 /*
	   Texture boxTexture = LoadTexture("box.png");
	 */

	 b2BodyDef groundBodyDef = b2DefaultBodyDef();
	 groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
	 groundBodyDef.type = b2_staticBody;
	 b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

	 b2Vec2 boxExtent = { 50.0f, 10.0f};
	 b2Polygon groundBox = b2MakeBox(boxExtent.x, boxExtent.y);
	 b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	 b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

	 b2BodyDef ballBodyDef = b2DefaultBodyDef();
	 ballBodyDef.type = b2_dynamicBody;
	 ballBodyDef.position = (b2Vec2){0.0f, 40.0f};
	 b2BodyId ballBodyId = b2CreateBody(worldId, &ballBodyDef);

	 b2ShapeDef ballShapeDef = b2DefaultShapeDef();
	 ballShapeDef.density = 1.0f;
	 ballShapeDef.friction = 0.3f;
	 b2Circle circle;
	 circle.center = (b2Vec2){2.0f, 3.0f};
	 circle.radius = 2.0f;
	 b2CreateCircleShape(ballBodyId, &ballShapeDef, &circle);


	 bool pause = false;

	 while (!WindowShouldClose())
	 {
		  float deltaTime = GetFrameTime();
		  b2World_Step(worldId, deltaTime, 4);

		  BeginDrawing();
			
		  ClearBackground(DARKGRAY);
		  b2World_Draw(worldId, &m_debugDraw );
		  /*
			for (int i = 0; i < BOX_COUNT; ++i)
			{
			DrawEntity(boxEntities + i);
			}
		  */
		  EndDrawing();
		  
	 }

	 /*
	   UnloadTexture(boxTexture);
	 */
	
	 CloseWindow();

	 return 0;
}
