#include "raylib.h"
#include "box2d/box2d.h"
#include <iostream>

const float drawTransformAxisScale = .05f;

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
	 std::cout << "Draw solid capsule" << std::endl;
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
//	 DrawCircleV(v2(p), size, GetColor(color));
//	 cout << "draw point " << p.x << " " << p.y << " " << size << endl;
}

void DrawStringFcn( b2Vec2 p, const char* s, b2HexColor color, void* context )
{
//	 int fontSize = 2;
//	 DrawText(s, p.x, p.y, fontSize, GetColor(color)); 
}

