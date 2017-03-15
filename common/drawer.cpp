#include "drawer.h"
#include <cmath>
#include <utility>

Drawer::Drawer()
{
}

void Drawer::line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, TGAColor color){
        bool steep = false;
        int cx0 = x0, cy0 = y0, cx1 = x1, cy1 = y1 ;
        if (std::abs(cx0-cx1)<std::abs(cy0-cy1)) { // if the line is steep, we transpose the image
            std::swap(cx0, cy0);
            std::swap(cx1, cy1);
            steep = true;
        }
        if (x0>x1) { // make it left−to−right
            std::swap(cx0, cx1);
            std::swap(cy0, cy1);
        }
        for (int x=cx0; x<=cx1; x++) {
            float t = (x-cx0)/(float)(cx1-cx0);
            int y = (int)round(cy0*(1.-t) + cy1*t);
            if (steep) {
                image.set(y, x, color); // if transposed, de−transpose
            } else {
                image.set(x, y, color);
            }
        }
}
