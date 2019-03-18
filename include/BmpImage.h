//
// Copyright [2019] <Zhi Sun>
//

#ifndef OCTQUANT_BMP_HANDLER_H
#define OCTQUANT_BMP_HANDLER_H
#include "OctTree.h"
#include <sstream>
#include <iomanip>

#pragma pack(push)  // push current alignment to stack
#pragma pack(1)  // set alignment to 1 byte boundary
struct BmpFileHeader {
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t bfOffBits;
  BmpFileHeader() = default;
  explicit BmpFileHeader(uint32_t size);
  size_t size() const;
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
  BmpInfoHeader() = default;
  BmpInfoHeader(uint32_t width, uint32_t height);
  inline size_t size() const;
};
#pragma pack(pop)  // restore original alignment from stack

class BmpImage {
 private:
  vector<Pixel> palette;
  vector<uint8_t> storage;
  vector<vector<Pixel>> bitmap;
  BmpInfoHeader infoHeader;
  BmpFileHeader fileHeader;
  OctNode *getClosestColor(OctNode *root, uint8_t blue, uint8_t green, uint8_t red) const;
 public:
  explicit BmpImage(string filename);
  const vector<vector<Pixel>> &getBitmap() const;
  void setPalette(const OctTree &tree);
  friend std::istream &operator>>(std::istream &in, BmpImage &image);
  friend std::ostream &operator<<(std::ostream &out, BmpImage &image);
};

namespace std {
template<>
struct hash<OctNode *> {
  std::size_t operator()(OctNode *node) const {
    std::size_t ret = hash<int>()((node->blue << 16) | (node->green << 8) | node->red);
    return ret;
  }
};
template<>
struct equal_to<OctNode *> {
  bool operator()(OctNode *lhs, OctNode *rhs) const {
    bool ret = lhs->red == rhs->red && lhs->green == rhs->green && lhs->blue == rhs->blue;
    return ret;
  }
};
}

#endif //OCTQUANT_BMP_HANDLER_H
