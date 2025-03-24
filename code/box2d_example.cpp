#include <iostream>

#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/math_functions.h"

using namespace std;

int main () {
	 b2WorldDef worldDef = b2DefaultWorldDef();
	 worldDef.gravity = (b2Vec2){0.0f, -10.0f};
	 b2WorldId worldId = b2CreateWorld(&worldDef);

	 b2BodyDef groundBodyDef = b2DefaultBodyDef();
	 groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
	 b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

	 b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
	 b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	 b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
	 
}
