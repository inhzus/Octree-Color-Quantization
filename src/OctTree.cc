//
// Copyright [2019] <Zhi Sun>
//

#include <OctTree.h>

OctTree::OctTree(int maxColorNum) : maxColors(maxColorNum), root(nullptr), colors(0) {
  std::make_heap(heap.begin(), heap.end(), comparator);
  root = new OctNode;
}
OctTree::~OctTree() {
  root->~OctNode();
}
vector<uint8_t> OctTree::generateColorIndex(uint8_t blue, uint8_t green, uint8_t red) {
  vector<uint8_t> ret(8, 0);
  for (int i = OCT_NUM - 1; i >= 0; i--) {
    ret[i] = (uint8_t) (((blue & 0x1) << 2) + ((green & 0x1) << 1) + (red & 0x1));
    blue >>= 1;
    green >>= 1;
    red >>= 1;
  }
  return ret;
}

void OctTree::insertColor(uint8_t blue, uint8_t green, uint8_t red) {
  auto indexes = generateColorIndex(blue, green, red);
  OctNode *cur = root;
  cur->cnt++;
  bool isNewColor = false;
  for (int i = 0; i < OCT_NUM; i++) {
    const auto idx = indexes[i];
    auto &child = cur->children[idx];
    if (child == nullptr) {
      child = new OctNode(
          idx, i, shrinkColor(blue, i), shrinkColor(green, i), shrinkColor(red, i), cur);
      isNewColor = true;
      if (i == OCT_NUM - 1) {
        heap.push_back(child);
        std::push_heap(heap.begin(), heap.end(), comparator);
      }
    }
    child->cnt++;
    cur = child;
  }
  if (isNewColor) {
    colors++;
  }
}

void OctTree::generatePalette() {
  while (heap.size() > maxColors) {
    std::pop_heap(heap.begin(), heap.end(), comparator);
    auto &cur = heap.back();
    heap.pop_back();

    const auto idx = cur->idx;
    auto *parent = cur->parent;
    mixColor(parent, cur);
    if (parent->mix == 0) {
      heap.push_back(parent);
      std::push_heap(heap.begin(), heap.end(), comparator);
    }
    parent->mix += cur->cnt;
    parent->children[idx] = nullptr;

  }
}

bool OctTree::comparator(const OctNode *lon, const OctNode *ron) {
  return lon->cnt > ron->cnt || (lon->cnt == ron->cnt && lon->depth < ron->depth);
}

inline uint8_t OctTree::shrinkColor(uint8_t color, int remain) {
//  return (color >> (OCT_NUM - remain - 1));
  return color & (uint8_t) (CHAR_MAX << (OCT_NUM - remain - 1));
}

void OctTree::mixColor(OctNode *parent, OctNode *child) {

#define __OTQ_OCT_TREE_MIX_COLOR(color) \
parent->color = static_cast<uint8_t>( \
(parent->mix * parent->color + child->cnt * child->color) \
/ (parent->mix + child->cnt) \
);
  /*
  parent->blue = static_cast<uint8_t>(
      (parent->mix * parent->blue + child->cnt * child->blue)
          / (parent->mix + child->cnt)
  );
  parent->green = static_cast<uint8_t>(
      (parent->mix * parent->green + child->cnt * child->green)
          / (parent->mix + child->cnt)
  );
  parent->red = static_cast<uint8_t>(
      (parent->mix * parent->red + child->cnt * child->red)
          / (parent->mix + child->cnt)
  );*/
  __OTQ_OCT_TREE_MIX_COLOR(blue);
  __OTQ_OCT_TREE_MIX_COLOR(green);
  __OTQ_OCT_TREE_MIX_COLOR(red);
}
void OctTree::insertBitmap(const vector<vector<Pixel>> &bitmap) {
  for (int i = static_cast<int>(bitmap.size() - 1); i >= 0; i--) {
    for (auto pixel : bitmap[i]) {
      insertColor(pixel.blue, pixel.green, pixel.red);
    }
  }
}
const vector<OctNode *> &OctTree::getHeap() const {
  return heap;
}
OctNode *OctTree::getRoot() const {
  return root;
}

OctNode::OctNode(uint8_t idx, int depth, uint8_t blue, uint8_t green, uint8_t red, OctNode *parent) :
    idx(idx), depth(depth), blue(blue), green(green), red(red), parent(parent), cnt(0), mix(0) {
  for (auto &item : children) {
    item = nullptr;
  }
}
OctNode::~OctNode() = default;

OctNode::OctNode() : parent(nullptr) {
  mix = cnt = idx = blue = green = red = 0;
  depth = -1;
  for (auto &item : children) {
    item = nullptr;
  }
}