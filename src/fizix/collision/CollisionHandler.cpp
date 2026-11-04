#include "CollisionHandler.h"

bool CollisionHandler::intersects(Hitbox a, Hitbox b)
{
    vec3d maxa = a.position+a.size, maxb= b.position + b.size;
    return (maxa.x > b.position.x && maxa.y > b.position.y && maxa.z > b.position.z)
        || (maxb.x > a.position.x && maxb.y > a.position.y && maxb.z > a.position.z);
}
