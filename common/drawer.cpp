#include "drawer.h"

Drawer::Drawer()
{
}

void Drawer::line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, TGAColor color){
    for (float t=0.; t<1.; t+=.01) {
        int x = x0*(1.-t) + x1*t;
        int y = y0*(1.-t) + y1*t;
        image.set(x, y, color);
    }
}
