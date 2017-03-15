#include <iostream>
#include "lib/tgaimage.h"
#include "common/drawer.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

int main(){

    TGAImage image(100, 100, TGAImage::RGB);
    Drawer::line(13, 20, 80, 40, image, white);
    Drawer::line(20, 13, 40, 80, image, red);
    Drawer::line(0, 0, 53, 40, image, blue);
    image.flip_vertically();
    image.write_tga_file("output.tga");




}
