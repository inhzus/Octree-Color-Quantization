//
// Copyright [2019] <Zhi Sun>
//

#ifndef OCTQUANT_OCTTREE_H
#define OCTQUANT_OCTTREE_H
//#include "BmpImage.h"

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <algorithm>

using std::string;
using std::vector;

const unsigned RGB_CHANNELS = 3;
const unsigned BITS_PER_CHANNEL = 1;
const unsigned ROW_ALIGNMENT = 4;
const int OCT_NUM = 8;
const int PALETTE_SPACE = 256;

#define DITHER
#define CLUSTER

struct OctNode;
struct WidePixel;

struct Pixel {
  uint8_t blue;
  uint8_t green;
  uint8_t red;
  uint8_t alpha;
  Pixel() : blue(0), green(0), red(0), alpha(0) {};
  Pixel(uint8_t blue, uint8_t green, uint8_t red, uint8_t alpha);
  Pixel &operator+=(const WidePixel &pixel);
  Pixel(const OctNode *node); // NOLINT(google-explicit-constructor)
};

struct WidePixel {
  int32_t blue, green, red, alpha;
  WidePixel(int32_t blue, int32_t green, int32_t red, int32_t alpha);
  WidePixel(const Pixel &pixel);
  explicit WidePixel(OctNode *node);
  WidePixel operator-(const Pixel &pixel) const;
  WidePixel operator*(const int &val) const;
  WidePixel operator/(const int &val) const;
  friend std::ostream &operator<<(std::ostream &os, const WidePixel &pixel);
};

struct OctNode {
  uint8_t blue, green, red, idx;
  uint64_t cnt, mix;
  int ptr, depth;
  OctNode *children[OCT_NUM];
  OctNode *parent;
  OctNode();
  OctNode(uint8_t idx, int depth, uint8_t blue, uint8_t green, uint8_t red, OctNode *parent);
  friend std::ostream &operator<<(std::ostream &os, const OctNode &node);
  inline bool isLeaf() const {
#ifdef CLUSTER
    bool ret = true;
    for (auto child : children) {
      if (child != nullptr) {
        ret = false;
      }
    }
    return ret;
#else
    return cnt == mix;
#endif
  }
  inline int removeChildren() {
    int ret = 0;
    for (auto & child : children) {
      if (child != nullptr) {
        ret++;
        child = nullptr;
      }
    }
    mix = cnt;
    return ret;
  }
};

class OctTree {
 private:
  OctNode *root;
  int maxColors;
  vector<OctNode *> heap;
#ifdef CLUSTER
  vector<OctNode *> nodes[8];
  static vector<OctNode *> getLeaves(OctNode *node);
#endif
  static bool comparator(const OctNode *lon, const OctNode *ron);
  inline uint8_t shrinkColor(uint8_t color, int remain);
  inline void mixColor(OctNode *parent, OctNode *child);
 public:
  explicit OctTree(int maxColorNum);
  OctNode *getRoot() const;
  const vector<OctNode *> &getHeap() const;
  static vector<uint8_t> generateColorIndex(uint8_t blue, uint8_t green, uint8_t red);
  void insertColor(uint8_t blue, uint8_t green, uint8_t red);
  void insertBitmap(const vector<vector<Pixel>> &bitmap);
  void generatePalette();
};

#endif //OCTQUANT_OCTTREE_H
