//
// Copyright [2019] <Zhi Sun>
//

#ifndef OCTQUANT_BMP_HANDLER_TEST_H
#define OCTQUANT_BMP_HANDLER_TEST_H

#include "BmpImage.h"
#include <fstream>
class BmpImageTest {
 private:
  void readImageToMemory() {
    BmpImage handler("asset/image/flag.bmp");
  }
  void writeColorImage() {
    BmpImage image("asset/image/flag.bmp");
    std::ofstream out("test/out.bmp", std::ios::binary);
    out << image;
    out.close();
  }

  void writeImageToConsole() {
    BmpImage image("asset/image/flag.bmp");
    std::cout << image;
  }

  void findFirstDiffFromTwoImages() {
    std::ifstream sin("asset/image/flag.bmp", std::ios::binary);
    std::ifstream din("test/out.bmp", std::ios::binary);

    char left, right;
    for (int i = 0; i < 10000; i++) {
      sin.read(&left, sizeof(char));
      din.read(&right, sizeof(char));
      if (left != right) {
        std::cout << "difference: " << i << std::endl;
        break;
      }
    }
    sin.close();
    din.close();
  }

 public:
  void run() {
//    readImageToMemory();
//    writeColorImage();
//    findFirstDiffFromTwoImages();
//    writeImageToConsole();
  }
};

#endif //OCTQUANT_BMP_HANDLER_TEST_H
