#include "drawer.h"
#include <cmath>
#include <utility>
#include "../lib/tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

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
    Vec2i ct0 = t0, ct1 = t1, ct2 = t2;

    if (ct0.y>ct1.y) std::swap(ct0, ct1);
    if (ct0.y>ct2.y) std::swap(ct0, ct2);
    if (ct1.y>ct2.y) std::swap(ct1, ct2);

    int total_height = ct2.y-ct0.y;
    for (int y=ct0.y; y<=ct1.y; y++) {
        int segment_height = ct1.y-ct0.y+1;
        float alpha = (float)(y-ct0.y)/total_height;
        float beta  = (float)(y-ct0.y)/segment_height; // be careful with divisions by zero
        Vec2i A = ct0 + (ct2-ct0)*alpha;
        Vec2i B = ct0 + (ct1-ct0)*beta;
        image.set(A.x, y, red);
        image.set(B.x, y, green);
    }

    /*
    Drawer::line(ct0, ct1, image, color);
    Drawer::line(ct1, ct2, image, color);
    Drawer::line(ct2, ct0, image, color);
    */
}
