//
// Copyright [2019] <Zhi Sun>
//

#include <OctTree.h>

OctTree::OctTree(int maxColorNum) : maxColors(maxColorNum), root(nullptr) {
  std::make_heap(heap.begin(), heap.end(), comparator);
  root = new OctNode;
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
  for (int i = 0; i < OCT_NUM; i++) {
    const auto idx = indexes[i];
    auto &child = cur->children[idx];
    uint8_t ib = shrinkColor(blue, i),
        ig = shrinkColor(green, i),
        ir = shrinkColor(red, i);
    if (child == nullptr) {
      child = new OctNode(
          idx, i, ib, ig, ir, cur);
#ifdef CLUSTER
      nodes[i].push_back(child);
#else
      if (i == OCT_NUM - 1) {
        heap.push_back(child);
        std::push_heap(heap.begin(), heap.end(), comparator);
      }
#endif
    }
    child->cnt++;
    cur = child;
  }
}

void OctTree::generatePalette() {
#ifdef CLUSTER
  for (auto *node : nodes[OCT_NUM - 1]) {
    node->mix = node->cnt;
  }
  auto leavesNum = getLeaves(root).size();
  for (int i = OCT_NUM - 2; i >= 0; i--) {
    vector<OctNode *> &line = nodes[i];
    std::sort(line.begin(), line.end());
    int k;
    for (k = 0; k < line.size(); k++) {
      leavesNum -= (line[k]->removeChildren() - 1);
      if (leavesNum <= maxColors) {
        break;
      }
    }
    if (leavesNum <= maxColors) {
      break;
    }
  }
  heap = getLeaves(root);
#else
  for (auto *node : heap) {
    node->mix = node->cnt;
  }
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
#endif
}

bool OctTree::comparator(const OctNode *lon, const OctNode *ron) {
  return lon->cnt > ron->cnt || (lon->cnt == ron->cnt && lon->depth < ron->depth);
}

inline uint8_t OctTree::shrinkColor(uint8_t color, int remain) {
//  return (color >> (OCT_NUM - remain - 1));
#ifdef CLUSTER
  return color & (uint8_t) (UCHAR_MAX << (OCT_NUM - remain - 1));
#else
  return color & (uint8_t) (UCHAR_MAX << (OCT_NUM - remain - 1));
#endif
}

void OctTree::mixColor(OctNode *parent, OctNode *child) {

#define __OTQ_OCT_TREE_MIX_COLOR(color) \
parent->color = static_cast<uint8_t>( \
(parent->mix * parent->color + child->cnt * child->color) \
/ (parent->mix + child->cnt) \
);
  __OTQ_OCT_TREE_MIX_COLOR(blue);
  __OTQ_OCT_TREE_MIX_COLOR(green);
  __OTQ_OCT_TREE_MIX_COLOR(red);
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
#ifdef CLUSTER
vector<OctNode *> OctTree::getLeaves(OctNode *node) {
  vector<OctNode *> ret;
  for (auto ptr : node->children) {
    if (ptr != nullptr) {
      if (ptr->isLeaf()) {
//        std::cout << *ptr << std::endl;
        ret.push_back(ptr);
      } else {
        vector<OctNode *> t = getLeaves(ptr);
        ret.insert(ret.begin(), t.begin(), t.end());
      }
    }
  }
  return ret;
}
#endif
OctNode::OctNode(uint8_t idx, int depth, uint8_t blue, uint8_t green, uint8_t red, OctNode *parent) :
    idx(idx), depth(depth), blue(blue), green(green), red(red), parent(parent), cnt(0), mix(0) {
  for (auto &item : children) {
    item = nullptr;
  }
}

OctNode::OctNode() : parent(nullptr) {
  mix = cnt = idx = blue = green = red = 0;
  depth = -1;
  for (auto &item : children) {
    item = nullptr;
  }
}
std::ostream &operator<<(std::ostream &os, const OctNode &node) {
  os << "blue: " << (uint32_t)node.blue << " green: " << (uint32_t)node.green << " red: " << (uint32_t)node.red << " cnt: " << node.cnt << " depth: "
     << node.depth;
  return os;
}
WidePixel::WidePixel(int32_t blue, int32_t green, int32_t red, int32_t alpha)
    : blue(blue), green(green), red(red), alpha(alpha) {}
WidePixel::WidePixel(OctNode *node) :
    blue(node->blue), green(node->green), red(node->red), alpha(0) {
}
WidePixel::WidePixel(const Pixel &pixel) :
    blue(pixel.blue), green(pixel.green), red(pixel.red), alpha(pixel.red) {}
WidePixel WidePixel::operator*(const int &val) const {
  return {blue * val, green * val, red * val, alpha * val};
}
WidePixel WidePixel::operator/(const int &val) const {
  return {blue / val, green / val, red / val, alpha / val};
}
WidePixel WidePixel::operator-(const Pixel &pixel) const {
  return {blue - pixel.blue, green - pixel.green, red - pixel.red, alpha - pixel.alpha};
}
std::ostream &operator<<(std::ostream &os, const WidePixel &pixel) {
  os << "blue: " << pixel.blue << " green: " << pixel.green << " red: " << pixel.red;
  return os;
}
