#include <iostream>
#include "lib/tgaimage.h"
#include "common/drawer.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

int main(){

    TGAImage image(100, 100, TGAImage::RGB);
    image.clear(white);

    Drawer::line(0, 100, 100, 0, image, green);
    Drawer::line(0, 0, 100, 80, image, red);
    Drawer::line(80, 40, 13, 20, image, blue);
    image.flip_vertically();
    image.write_tga_file("output.tga");




}
