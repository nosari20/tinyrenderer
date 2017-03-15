#include <iostream>
#include "lib/tgaimage.h"
#include "common/drawer.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(){

    TGAImage image(100, 100, TGAImage::RGB);
    Drawer::line(13, 20, 80, 40, image, white);
    image.flip_vertically();
    image.write_tga_file("output.tga");




}
