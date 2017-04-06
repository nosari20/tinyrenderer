#ifndef DRAWER_H
#define DRAWER_H

#include "../lib/tgaimage.h"
#include "../lib/geometry.h"
#include "../lib/model.h"

struct Shader {
    Vec2i varying_uv[3];
    Model *model;
    float varying_inty[3];
    float intensity;
    Vec3f light_dir;
    TGAColor fragment(Vec3f bar) {
        Vec2i uv   = varying_uv[0]*bar.x + varying_uv[1]*bar.y + varying_uv[2]*bar.z;
        float inty = model->norm(uv)*light_dir;
         return model->diffuse(uv)*inty;
        //return model->diffuse(uv)*intensity;
        //return TGAColor(255,255,255)*intensity;

        /*
        Vec2i uv   = varying_uv[0]*bar.x + varying_uv[1]*bar.y + varying_uv[2]*bar.z;
        float inty = varying_inty[0]*bar.x + varying_inty[1]*bar.y + varying_inty[2]*bar.z;
        inty = std::max(0.f, std::min(1.f, inty));
        return model->diffuse(uv)*inty;
        */
    }
};

class Drawer
{
public:
    Drawer();
    static void line(const int x0, const int y0, const int x1, const int y1, TGAImage &image, const TGAColor color);
    static void line(const Vec2i t0, const Vec2i t1, TGAImage &image, const TGAColor color);
    static void triangle(const Vec2i t0, const Vec2i t1, const Vec2i t2, TGAImage &image, const TGAColor color);
    static void triangle(const Vec2f *pts, TGAImage &image, const TGAColor color);
    static void triangle(const Vec3f *pts, float *zbuffer,TGAImage &image, const TGAColor color);
    static void triangle(const Vec3f *pts, float *zbuffer,TGAImage &image, Shader &shader);
};



#endif // DRAWER_H
