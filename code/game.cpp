#include "raylib.h"
#include "box2d/box2d.h"

#include <assert.h>
#include <iostream>

using namespace std;

// Forked from Erin's box2d-raylib example

const int screen_width = 800;
const int screen_height = 480;
const float pixelsPerMeter = 5.0f;
const float ballDensity = 15.0f;
const float wallLength = 100.0f;
const float wallWidth = 5.0f;
b2Vec2 gravity = {0, -9.8f};

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
	 const float axisScale = 3.5f;
	 Vector2 rX, rY;
	 rX = v2(b2Rot_GetXAxis(transform.q));
	 rY = v2(b2Rot_GetYAxis(transform.q));

	 Vector2 p, q1, q2;
	 p =  v2(transform.p);
	 q1.x = p.x + axisScale * rX.x;
	 q1.y = p.y + axisScale * rX.y;
	 DrawLineV(p, q1, RED);

	 q2.x = p.x + axisScale * rY.x;
	 q2.y = p.y + axisScale * rY.y;
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

void init_debug() {
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

int main(void)
{
	 InitWindow(screen_width, screen_height, "Maze");

	 Camera2D camera = { 0 };
	 camera.target = (Vector2){ 0.0f, 0.0f };
	 camera.offset = (Vector2){ screen_width/2.0f, screen_height/2.0f };
	 camera.rotation = 0.0f;
	 camera.zoom = pixelsPerMeter;
	
	 SetTargetFPS(60);
	
//	 float lengthUnitsPerMeter = 10.0f;
//	 b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

	 init_debug();

	 b2WorldDef worldDef = b2DefaultWorldDef();
	 worldDef.gravity.x = gravity.x;
	 worldDef.gravity.y = gravity.y;
	 b2WorldId worldId = b2CreateWorld(&worldDef);

	 b2BodyDef wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){0.0f, -wallLength/2 - wallWidth/2};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall0Id = b2CreateBody(worldId, &wallBodyDef);

	 wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){-wallLength/2 - wallWidth/2, 0};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall1Id = b2CreateBody(worldId, &wallBodyDef);

	 wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){0.0f, wallLength/2 + wallWidth/2};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall2Id = b2CreateBody(worldId, &wallBodyDef);

	 wallBodyDef = b2DefaultBodyDef();
	 wallBodyDef.position = (b2Vec2){wallLength/2 + wallWidth/2, 0};
	 wallBodyDef.enableSleep = false;
	 wallBodyDef.type = b2_staticBody;
	 b2BodyId wall3Id = b2CreateBody(worldId, &wallBodyDef);
	 
	 b2Vec2 boxExtent = { wallLength/2, wallWidth/2};
	 b2Polygon wallHBox = b2MakeBox(boxExtent.x, boxExtent.y);
	 b2Polygon wallVBox = b2MakeBox(boxExtent.y, boxExtent.x);
	 b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	 b2CreatePolygonShape(wall0Id, &groundShapeDef, &wallHBox);
	 b2CreatePolygonShape(wall1Id, &groundShapeDef, &wallVBox);
	 b2CreatePolygonShape(wall2Id, &groundShapeDef, &wallHBox);
	 b2CreatePolygonShape(wall3Id, &groundShapeDef, &wallVBox);
	 
	 b2BodyDef ballBodyDef = b2DefaultBodyDef();
	 ballBodyDef.type = b2_dynamicBody;
	 ballBodyDef.position = (b2Vec2){0.0f, 40.0f};
	 ballBodyDef.enableSleep = false;
	 b2BodyId ballBodyId = b2CreateBody(worldId, &ballBodyDef);

	 b2ShapeDef ballShapeDef = b2DefaultShapeDef();
	 ballShapeDef.density = ballDensity;
	 ballShapeDef.friction = 0.3f;
	 b2Circle circle;
	 circle.center = (b2Vec2){0.0f, 0.0f};
	 circle.radius = 2.0f;
	 b2CreateCircleShape(ballBodyId, &ballShapeDef, &circle);

	 bool pause = false;

	 while (!WindowShouldClose())
	 {
		  if (IsKeyDown(KEY_UP))
		  {
			   gravity.y -= 1.3f;
			   b2World_SetGravity (worldId, gravity);
		  }
		  else if (IsKeyDown(KEY_DOWN))
		  {
			   gravity.y += 1.3f;
			   b2World_SetGravity (worldId, gravity);
		  }
		  else if (IsKeyDown(KEY_LEFT))
		  {
			   gravity.x -= 1.3f;
			   b2World_SetGravity (worldId, gravity);
		  }
		  else if (IsKeyDown(KEY_RIGHT))
		  {
			   gravity.x += 1.3f;
			   b2World_SetGravity (worldId, gravity);
		  }
		  
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
