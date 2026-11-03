#pragma once
#include "Player.h"
inline std::optional<float> FirstRelativeCircleHit(
    Vector2 projectileStart, Vector2 projectileEnd,
    Vector2 enemyStart, Vector2 enemyEnd, float radius)
{
    const double sx = double(projectileStart.x) - enemyStart.x;
    const double sy = double(projectileStart.y) - enemyStart.y;
    const double dx = (double(projectileEnd.x) - projectileStart.x)
                    - (double(enemyEnd.x) - enemyStart.x);
    const double dy = (double(projectileEnd.y) - projectileStart.y)
                    - (double(enemyEnd.y) - enemyStart.y);
    const double c = sx*sx + sy*sy - double(radius)*radius;
    if (c <= 0.0) return 0.0f;
    const double a = dx*dx + dy*dy;
    if (a <= 1e-12) return std::nullopt;
    const double b = 2.0*(sx*dx + sy*dy);
    const double discriminant = b*b - 4.0*a*c;
    if (discriminant < 0.0) return std::nullopt;
    const double t = (-b - std::sqrt(discriminant))/(2.0*a);
    if (t < 0.0 || t > 1.0) return std::nullopt;
    return static_cast<float>(t);
}