//
// Copyright [2019] <Zhi Sun>
//

#ifndef OCTQUANT_OCTTREETEST_H
#define OCTQUANT_OCTTREETEST_H

#include "OctTree.h"
#include <iomanip>

class OctTreeTest {
 private:
  void checkColorIndex() {
    // 37: 0010 0101
    // 107: 0110 1011
    // 219: 1101 1011
    // supposed to be: 1 3 6 1 3 4 3 7
    vector<uint8_t> rightRet = {1, 3, 6, 1, 3, 4, 3, 7};
    int i = 0;
    auto gen = OctTree::generateColorIndex(37, 107, 219);
    for (auto &ch : gen) {
      if (ch != rightRet[i]) {
        std::cout << i << ", right: " << (int) rightRet[i] <<
                  ", wrong: " << (int) ch << std::endl;
      }
      i++;
    }
  }

  void insertColorToTree() {
    BmpImage image("asset/image/shanghai.bmp");
    OctTree tree(256);
    tree.insertBitmap(image.getBitmap());
    tree.generatePalette();
    image.setPalette(tree);
  }
  void convert() {
    BmpImage image("asset/image/arch.bmp");
//    BmpImage image("test/out.bmp");
    OctTree tree(256);
    tree.insertBitmap(image.getBitmap());
    tree.generatePalette();
    image.setPalette(tree);
    std::ofstream out("test/out.bmp", std::ios::binary);
    out << image;
  }

 public:
  void run() {
    checkColorIndex();
//    insertColorToTree();
    convert();
  }
};

#endif //OCTQUANT_OCTTREETEST_H
