#ifndef DRAWER_H
#define DRAWER_H

#include "../lib/tgaimage.h"

class Drawer
{
public:
    Drawer();
    static void line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, TGAColor color);
};

#endif // DRAWER_H
