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
const int width  = 800;
const int height = 500;

void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color, int ybuffer[]) {
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }
    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (ybuffer[x]<y) {
            ybuffer[x] = y;
            image.set(x, 0, color);
        }
    }
}



int main(int argc, char** argv){

    { // just dumping the 2d scene (yay we have enough dimensions!)
        TGAImage scene(width, height, TGAImage::RGB);

        int *zbuffer = new int[width*height];

        int ybuffer[width];
        for (int i=0; i<width; i++) {
            ybuffer[i] = std::numeric_limits<int>::min();
        }
        rasterize(Vec2i(20, 34),   Vec2i(744, 400), scene, red,   ybuffer);
        rasterize(Vec2i(120, 434), Vec2i(444, 400), scene, green, ybuffer);
        rasterize(Vec2i(330, 463), Vec2i(594, 200), scene, blue,  ybuffer);

        scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        scene.write_tga_file("scene.tga");
    }
    return 0;
}
