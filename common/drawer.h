#ifndef DRAWER_H
#define DRAWER_H

#include "../lib/tgaimage.h"
#include "../lib/geometry.h"

class Drawer
{
public:
    Drawer();
    static void line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, const TGAColor color);
    static void line(const Vec2i t0, const Vec2i t1, TGAImage &image, const TGAColor color);
    static void triangle(const Vec2i t0, const Vec2i t1, const Vec2i t2, TGAImage &image, const TGAColor color);

private:
    static bool in_triangle(const Vec2i p,const Vec2i t0, const Vec2i t1, const Vec2i t2);
};

#endif // DRAWER_H
