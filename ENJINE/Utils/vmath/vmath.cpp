#include "vmath.h"

float vmath::distance(Vector2f a, Vector2f b) {
	Vector2f c = a - b;
	return sqrt((c.x * c.x) + (c.y + c.y));
}
Vector2f vmath::tovec2f(ImVec2 a) {
	return Vector2f(a.x, a.y);
}