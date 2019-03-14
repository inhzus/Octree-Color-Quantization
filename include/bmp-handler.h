//
// Copyright [2019] <Zhi Sun>
//

#ifndef OCTQUANT_BMP_HANDLER_H
#define OCTQUANT_BMP_HANDLER_H
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
using std::string;
using std::vector;

struct BmpFileHeader {
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
  friend std::istream &operator>>(std::istream &in, BmpFileHeader &header);
};

struct BmpInfoHeader {
  uint32_t biSize;
  uint32_t biWidth;
  uint32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint32_t biXPixelsPerMeter;
  uint32_t biYPixelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
  friend std::istream &operator>>(std::istream &in, BmpInfoHeader &header);
};

class BmpHandler {
 private:
  vector<vector<int>> pixels[3];
 public:
  explicit BmpHandler(string filename);
};

#endif //OCTQUANT_BMP_HANDLER_H
