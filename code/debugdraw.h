#include "raylib.h"
#include "box2d/box2d.h"

void drawPoly (const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context);
void drawCircle (b2Vec2 center, float radius, b2HexColor color, void *context);
void drawSolidCircle (b2Transform transform, float radius, b2HexColor color, void *context);
void drawSolidPoly (b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius, b2HexColor color, void *context);
void DrawSolidCapsuleFcn( b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context );
void DrawSegmentFcn( b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context );
void DrawTransformFcn( b2Transform transform, void* context );
void DrawPointFcn( b2Vec2 p, float size, b2HexColor color, void* context);
void DrawStringFcn( b2Vec2 p, const char* s, b2HexColor color, void* context);







