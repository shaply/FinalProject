#include "../include/CollisionSolving.h"

Collision::Collision(Block* b1, Block* b2, Point p1, Point p2, Vect2 normal)
        : block1(b1), block2(b2), point1(p1), point2(p2), normal(normal) {}

double Collision::calc_impulse(Point) { // TODO
    return 0;
}

void Collision::solve() {
    for (const auto& point : { point1, point2 }) {

        Block* block = point == point1 ? block1 : block2;
        auto& impulse = point == point1 ? p1_total_impulse : p2_total_impulse;
        auto prev_impulse = impulse;
        auto delta_impulse = calc_impulse(point);
        impulse += delta_impulse;
        if (impulse < 0) impulse = 0;
        delta_impulse = impulse - prev_impulse;

        block->apply_accel(
                Vect2(normal.x / block->mass * delta_impulse, normal.y / block->mass * delta_impulse)
        );

        Vect2 r = block->distance(point);
        block->apply_angular_accel(
                r.magnitude() * sin(
                        r.direction() - normal.direction()
                ) / block->moment_of_inertia() * delta_impulse
        );

    }
}

CollisionGroup::CollisionGroup(Block *b1, Block *b2) : block1(b1), block2(b2) {}

std::vector<Collision> CollisionGroup::find_collisions() { // TODO
    return {};
}

Vect2 CollisionGroup::calc_normal() { // TODO
    return {};
}

void CollisionGroup::solve() { // TODO

}