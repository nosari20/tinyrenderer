#include <iostream>
#include <stdio.h>

#include <stdlib.h>

#include <time.h>
#include <limits>
#include "lib/tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "common/drawer.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);
Model *model = NULL;
const int width  = 1024;
const int height = 1024;



Vec3f world2screen(Vec3f v) {
    return Vec3f(int((v.x+1.)*width/2.+.5), int((v.y+1.)*height/2.+.5), v.z);
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

    float *zbuffer = new float[width*height];
    Vec3f light_dir(0,0,-1);

    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());


    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec3f pts_s[3];
        Vec3f pts_w[3];
        for (int i=0; i<3; i++){
            pts_s[i] = world2screen(model->vert(face[i]));
            pts_w[i] = model->vert(face[i]);
        }

        Vec3f n = cross((pts_w[2]-pts_w[0]),(pts_w[1]-pts_w[0]));
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0)
            Drawer::triangle(pts_s, zbuffer, image, model, intensity);
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
