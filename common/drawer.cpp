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
    //Vec2i ct0 = t0, ct1 = t1, ct2 = t2;

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

    auto in_triangle = [](Vec2i p, Vec2i p0, Vec2i p1, Vec2i p2) -> bool{

        Vec3f u = Vec3f(p1.x-p0.x, p1.y-p0.y, 0);
        Vec3f v = Vec3f(p2.x-p0.x, p2.y-p0.y, 0);
        Vec3f w = Vec3f(p.x-p0.x, p.y-p0.y, 0);

        Vec3f vw = cross(v,w);
        Vec3f vu = cross(v,u);

        if (dot(vw, vu) < 0)
            return false;


        Vec3f uw = cross(u,w);
        Vec3f uv = cross(u,v);
        if (dot(uw, uv) < 0)
            return false;


        double luv = sqrt(pow(uv.x,2)+pow(uv.y,2)+pow(uv.z,2));
        double lvw = sqrt(pow(vw.x,2)+pow(vw.y,2)+pow(vw.z,2));
        double luw = sqrt(pow(uw.x,2)+pow(uw.y,2)+pow(uw.z,2));

        float denom = luv;
        float r = lvw / denom;
        float t = luw / denom;


        return (r + t <= 1);
    };


    Vec2i P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            if (in_triangle(P,t0,t1,t2))
                image.set(P.x, P.y, color);
        }
    }

}


void Drawer::triangle(const Vec2f pts, float *z_buffer, TGAImage &image, const TGAColor color){
    Vec2i bboxmin(image.get_width()-1,  image.get_height()-1);
    Vec2i bboxmax(0, 0);;
    for (int i=0; i<3; i++) {
        bboxmin.x = std::min(bboxmin.x, pts[i].x);
        bboxmin.x = std::min(bboxmin.y, pts[i].y);

        bboxmax.x = std::max(bboxmax.x, pts[i].x);
        bboxmax.x = std::min(bboxmax.y, pts[i].y);
    }

    auto in_triangle = [](Vec2i p, Vec2i p0, Vec2i p1, Vec2i p2) -> bool{

        Vec3f u = Vec3f(p1.x-p0.x, p1.y-p0.y, 0);
        Vec3f v = Vec3f(p2.x-p0.x, p2.y-p0.y, 0);
        Vec3f w = Vec3f(p.x-p0.x, p.y-p0.y, 0);

        Vec3f vw = cross(v,w);
        Vec3f vu = cross(v,u);

        if (dot(vw, vu) < 0)
            return false;


        Vec3f uw = cross(u,w);
        Vec3f uv = cross(u,v);
        if (dot(uw, uv) < 0)
            return false;


        double luv = sqrt(pow(uv.x,2)+pow(uv.y,2)+pow(uv.z,2));
        double lvw = sqrt(pow(vw.x,2)+pow(vw.y,2)+pow(vw.z,2));
        double luw = sqrt(pow(uw.x,2)+pow(uw.y,2)+pow(uw.z,2));

        float denom = luv;
        float r = lvw / denom;
        float t = luw / denom;


        return (r + t <= 1);
    };


    Vec3f P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            if (in_triangle(P,t0,t1,t2)){
                image.set(P.x, P.y, color);
            }

        }
    }
}
