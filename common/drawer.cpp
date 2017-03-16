#include "drawer.h"
#include <cmath>
#include <utility>

Drawer::Drawer()
{
}

void Drawer::line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, const TGAColor color){
    int cx0 = x0, cy0 = y0, cx1 = x1, cy1 = y1 ;
    bool steep = false;
    if (std::abs(cx0 - cx1)<std::abs(cy0 - cy1)) {
        std::swap(cx0, cy0);
        std::swap(cx1, cy1);
        steep = true;
    }
    if (cx0 > cx1) {
        std::swap(cx0, cx1);
        std::swap(cy0, cy1);
    }
    int dx = cx1 - cx0;
    int dy = cy1 - cy0;
    float derror = std::abs(dy)*2;
    float error = 0;
    int y = cy0;
    for (int x = cx0; x <= cx1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx) {
            y += (cy1 > cy0 ? 1 : -1);
            error -= dx * 2;
        }
    }
}

void Drawer::line(const Vec2i t0, const Vec2i t1, TGAImage &image, const TGAColor color){
    Drawer::line(t0[0], t0[1], t1[0], t1[1], image, color);
}

void Drawer::triangle(const Vec2i t0, const Vec2i t1, const Vec2i t2, TGAImage &image, const TGAColor color) {
    Drawer::line(t0, t1, image, color);
    Drawer::line(t1, t2, image, color);
    Drawer::line(t2, t0, image, color);
}
