//
// Copyright [2019] <inhzus>
//

#include "BmpImage.h"
#include <fstream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage:\n"
           "  ./src.exe <input-bmp-filename> <output-bmp-filename>\n");
  }
  BmpImage image(argv[1]);
  OctTree tree(256);
  tree.insertBitmap(image.getBitmap());
  tree.generatePalette();
  image.setPalette(tree);
  std::ofstream out(argv[2], std::ios::binary);
  out << image;
  return 0;
}
