#include "drawer.h"
#include <math.h>
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

    Vec2i bboxmin(image.get_width()-1,  image.get_height()-1);
    Vec2i bboxmax(0, 0);

    bboxmin[0] = std::min(
                bboxmin[0],
            std::min(
                t0[0],
            std::min(
                t1[0],
            t2[0]
            )
            )
            );

    bboxmin[1] = std::min(
                bboxmin[1],
            std::min(
                t0[1],
            std::min(
                t1[1],
            t2[1]
            )
            )
            );

    bboxmax[0] = std::max(
                bboxmax[0],
            std::max(
                t0[0],
            std::max(
                t1[0],
            t2[0]
            )
            )
            );

    bboxmax[1] = std::max(
                bboxmax[1],
            std::max(
                t0[1],
            std::max(
                t1[1],
            t2[1]
            )
            )
            );

    auto barycentric = [](Vec2i p, Vec2i p0, Vec2i p1, Vec2i p2) -> Vec3f{

        double d = (p0.x-p2.x)*(p1.y-p2.y) - (p1.x-p2.x)*(p0.y-p2.y);

        double a = (1/d)*((p1.y-p2.y)*(p.x-p2.x) + (p2.x-p1.x)*(p.y-p2.y));
        double b = (1/d)*((p2.y-p0.y)*(p.x-p2.x) + (p0.x-p2.x)*(p.y-p2.y));

        return Vec3f(a,b,1-a-b);
    };


    Vec2i P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            Vec3f bc_screen  = barycentric(P, ct0, ct1, ct2);
            if (bc_screen.x>=0 && bc_screen.y>=0 && bc_screen.z>=0)
                image.set(P.x, P.y, color);
        }
    }

}
