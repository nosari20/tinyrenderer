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
const int depth = 255;

Vec3f light_dir(0,0,-1);
Vec3f eye(1,1,3);
Vec3f center(0,0,0);



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



Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye-center).normalize();
    Vec3f x = cross(up,z).normalize();
    Vec3f y = cross(z,x).normalize();
    Matrix res = Matrix::identity(4);
    for (int i=0; i<3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}


Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}

Matrix Vec3f2Matrix(Vec3f v){

}



int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("african_head/african_head.obj");
    }

    float *zbuffer = new float[width*height];
    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());


    Matrix ModelView  = lookat(eye, center, Vec3f(0,1,0));
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
    Projection[3][2] = -1.f/(eye-center).norm();


    std::cerr << ModelView << std::endl;
    std::cerr << Projection << std::endl;
    std::cerr << ViewPort << std::endl;
    Matrix z = (ViewPort*Projection*ModelView);
    std::cerr << z << std::endl;



    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::ostringstream foo;
        foo << "output_" << std::setfill('0') << std::setw(5) << i << ".tga";
        std::vector<int> face = model->face(i);
        Vec3f pts_s[3];
        Vec3f pts_w[3];

        for (int i=0; i<3; i++){
            pts_w[i] = model->vert(face[i]);

            /*
            Matrix m = ViewPort*Projection*ModelView*Matrix(pts_w[i]);

            pts_w[i] =  Vec3f((m[0][0]/m[3][0]), (m[1][0]/m[3][0]), (m[2][0]/m[3][0]));

            */

        }
        transform(pts_w,3);
        for (int i=0; i<3; i++){

            pts_s[i] = world2screen(pts_w[i]);


        }
        Vec2f uv[3];
        for (int k=0; k<3; k++) {
            uv[k] = model->uv(i, k);
        }


        Vec3f n = cross((pts_w[2]-pts_w[0]),(pts_w[1]-pts_w[0]));
        n.normalize();
        float intensity = n*light_dir;
        if (intensity>0){
            Drawer::triangle(pts_s, uv, zbuffer, image, model, light_dir);
            //Drawer::triangle(pts_s, uv, zbuffer, image,model, white, light_dir);
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
