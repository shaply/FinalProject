#pragma once

#include "Point.h"
#include "Color.h"
#include "Rect.h"
#include "Triangle.h"
#include "Vect2.h"
#include "Constants.h"

#include <stdexcept>
#include <vector>
#include <cmath>

#ifndef REPLIT
#include <GL/glew.h>
#endif

// This is all Microsoft's fault (this is required for compilation with Visual Studio)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline double normalize_angle(double angle) {
    return angle - (2.0 * M_PI) * std::floor(angle / (2.0 * M_PI));
}

class Block {
public:
    bool is_static = false;

    Point position;

    // Angle in radians between horizontal line in 2D space
    // and the ray extending from the center of the rectangle
    // to the right side of the rectangle (whichever side is
    // the right side when the rectangle is not rotated, of course)
    // Should always be less than 2*PI
    double rotation = 0;

    std::vector<Point> vertices; // block is a polygon, with no holes

    Vect2 velocity;
    double angular_velocity;

    double mass;

    Color color = Color::WHITE;

    // OpenGL objects
    static unsigned int gl_program;
    unsigned int gl_vao, gl_ebo, gl_triangles;

    std::vector<Point*> triangulation; // point to Points in the vertices field

    // place rotation between 0 and 2*PI
    void normalize_rotation();

    // divide shape into triangles
    // must be called before rendering if vertices have been modified since last render
    // returns triangle vertex indices
    std::vector<unsigned int> triangulate();

    // initialization
    Block(double cX, double cY, double width, double height, double xVel, double yVel, double mass);

#ifndef REPLIT
    static void init_static_render_cache();
#endif

    // setters

    void set_position(Point p);
    void set_position(double x, double y);

    void set_rotation(double r);

    void set_color(Color c);

    void add_vertex(Point p);
    void add_vertex(size_t index, Point p);

    // getters

#ifndef REPLIT

    static decltype(gl_program) get_gl_program();

    // rendering

    void update_render_cache();

    void render() const;

#endif

    std::vector<Triangle> get_triangulation(); // should take triangles from the "triangulation" field

    Rect bounding_box(); // return a rectangular bounding box containing all the block's vertices

    bool is_intersecting(Block& other);

    // finds every intersection point between two blocks
    // intended to be called if it is already known that this block and the other block are colliding
    // std::vector<Point> find_intersections(Block& other);

    Point center_of_mass();

    double moment_of_inertia(); // scary

    void apply_accel(const Vect2& accel); // does not account for fps when applying acceleration
    void apply_angular_accel(double accel);
    void apply_velocity();

    Vect2 distance(const Point&);

    Point to_world_coords(Point);

    bool operator==(const Block& other);

};