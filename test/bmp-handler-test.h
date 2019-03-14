//
// Copyright [2019] <Zhi Sun>
//

#ifndef OCTQUANT_BMP_HANDLER_TEST_H
#define OCTQUANT_BMP_HANDLER_TEST_H

#include "bmp-handler.h"

class BmpHandlerTest {
 private:
  void readImageToMemory() {
    BmpHandler handler("asset/image/lena.bmp");
  }
 public:
  void run() {
    readImageToMemory();
  }
};

#endif //OCTQUANT_BMP_HANDLER_TEST_H
