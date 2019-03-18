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

using std::string;
using std::vector;

const unsigned RGB_CHANNELS = 3;
const unsigned BITS_PER_CHANNEL = 8;
const unsigned ROW_ALIGNMENT = 4;
const int OCT_NUM = 8;

struct OctNode;

struct Pixel {
  uint8_t blue;
  uint8_t green;
  uint8_t red;
  uint8_t alpha;
  Pixel() : blue(0), green(0), red(0), alpha(0) {};
  Pixel(const OctNode *node);
};

struct OctNode {
  uint8_t blue, green, red, idx;
  uint64_t cnt, mix;
  int ptr, depth;
  OctNode *children[OCT_NUM];
  OctNode *parent;
  OctNode();
  OctNode(uint8_t idx, int depth, uint8_t blue, uint8_t green, uint8_t red, OctNode *parent);
  ~OctNode();
};

class OctTree {
 private:
  OctNode *root;
  int colors;
  int maxColors;
  vector<OctNode *> heap;
  static bool comparator(const OctNode *lon, const OctNode *ron);
  inline uint8_t shrinkColor(uint8_t color, int remain);
  inline void mixColor(OctNode *parent, OctNode *child);
 public:
  explicit OctTree(int maxColorNum);
  ~OctTree();
  OctNode *getRoot() const;
  const vector<OctNode *> &getHeap() const;
  static vector<uint8_t> generateColorIndex(uint8_t blue, uint8_t green, uint8_t red);
  void insertColor(uint8_t blue, uint8_t green, uint8_t red);
  void insertBitmap(const vector<vector<Pixel>> &bitmap);
  void generatePalette();
};

#endif //OCTQUANT_OCTTREE_H
