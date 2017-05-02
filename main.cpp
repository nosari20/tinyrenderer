#include <iostream>
#include <stdio.h>

#include <iomanip>
#include <sstream>
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


void transform(Vec3f *pts, int size = 3){
   //Vec4f p = Vec4f();
   //p[3] = 1;

   float c = 3;

   for (int i=0; i<size; i++) {

       pts[i][0] = pts[i][0] / (1 - pts[i][2]/c);
       pts[i][1] = pts[i][1] / (1 - pts[i][2]/c);
       pts[i][2] = pts[i][2] / (1 - pts[i][2]/c);


   }
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("african_head/african_head.obj");
    }

    float *zbuffer = new float[width*height];
    Vec3f light_dir(0,0,-1);

    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());


    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::ostringstream foo;
        foo << "output_" << std::setfill('0') << std::setw(5) << i << ".tga";
        std::vector<int> face = model->face(i);
        Vec3f pts_s[3];
        Vec3f pts_w[3];

        for (int i=0; i<3; i++){
            pts_w[i] = model->vert(face[i]);

        }
        transform(pts_w,3);
        for (int i=0; i<3; i++){
            pts_s[i] = world2screen(pts_w[i]);

        }
        Vec2f uv[3];
        for (int k=0; k<3; k++) {
            uv[k] = model->uv(i, k);
        }

        transform(pts_s,3);

        Vec3f n = cross((pts_w[2]-pts_w[0]),(pts_w[1]-pts_w[0]));
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0){
            Drawer::triangle(pts_s, uv, zbuffer, image, model, intensity);
            //if(i%100 == 0)
                //image.write_tga_file(foo.str().c_str());
        }
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");

    { // dump z-buffer (debugging purposes only)
        TGAImage zbimage(width, height, TGAImage::RGBA);
        float zbmin = 1e10, zbmax = -1e10;
        for (int i=0; i<width*height; i++) {
          if (zbuffer[i]<-1e10) continue;
          zbmin = std::min(zbmin, zbuffer[i]);
          zbmax = std::max(zbmax, zbuffer[i]);
        }

        for (int i=0; i<width; i++) {
            for (int j=0; j<height; j++) {
          float z = zbuffer[i+j*width];
          if (z<-1e10) continue;
          z = 255*(z-zbmin)/(zbmax-zbmin);
          zbimage.set(i, j, TGAColor(z,z,z,255 ));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("zbuffer.tga");
    }



    delete model;
    return 0;
}
