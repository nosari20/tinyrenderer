#include "drawer.h"

Drawer::Drawer()
{
}

void Drawer::line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, TGAColor color){
    for (int x=x0; x<=x1; x++) {
            float t = (x-x0)/(float)(x1-x0);
            int y = y0*(1.-t) + y1*t;
            image.set(x, y, color);
        }
}
