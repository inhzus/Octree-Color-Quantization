//
// Copyright [2019] <Zhi Sun>
//

#include "BmpImage.h"
#include <map>
#include <fstream>
#include <cassert>

using std::ifstream;
using std::ofstream;
using std::map;

inline uint32_t getPaddingBytesPerRow(uint32_t rowBytes) {
  return (ROW_ALIGNMENT - (rowBytes) % ROW_ALIGNMENT) % ROW_ALIGNMENT;
}

size_t BmpFileHeader::size() const {
  return 2 + 4 + 2 + 2 + 4;  // Cos of c++ `struct` alignment
}
BmpFileHeader::BmpFileHeader(uint32_t size) {
  bfType = 0x4D42;
  bfSize = size;
  bfReserved1 = 0;
  bfReserved2 = 0;
  bfOffBits = 54 + 256 * 4;
}

size_t BmpInfoHeader::size() const {
  return biSize;
}
BmpInfoHeader::BmpInfoHeader(uint32_t width, uint32_t height) {
  biSize = 40;
  biWidth = width;
  biHeight = height;
  biPlanes = 1;
  biBitCount = 0;
  biCompression = 0;
  biSizeImage = (getPaddingBytesPerRow(width) + width) * height;
  biXPixelsPerMeter = biYPixelsPerMeter = biClrUsed = biClrImportant = 0;
}

Pixel::Pixel(const OctNode *node) {
  blue = node->blue;
  green = node->green;
  red = node->red;
  alpha = 0;
}

BmpImage::BmpImage(string filename) {
  ifstream fin(filename, std::ios::binary);
  fin >> *this;
  fin.close();
}
std::istream &operator>>(std::istream &in, BmpImage &image) {
  in.read((char *) &image.fileHeader, sizeof(BmpFileHeader));
  in.read((char *) &image.infoHeader, sizeof(BmpInfoHeader));
  assert(image.fileHeader.bfType == 0x4D42);
  const BmpFileHeader &fileHeader = image.fileHeader;
  const BmpInfoHeader &infoHeader = image.infoHeader;

  if (infoHeader.biBitCount == RGB_CHANNELS * BITS_PER_CHANNEL) {
    image.bitmap = vector<vector<Pixel>>(infoHeader.biHeight, vector<Pixel>(infoHeader.biWidth, Pixel()));
    in.seekg(fileHeader.bfOffBits, std::ios::beg);
    const uint32_t paddingBytesPerRow = getPaddingBytesPerRow(
        infoHeader.biWidth * RGB_CHANNELS * BITS_PER_CHANNEL);

    for (int i = infoHeader.biHeight - 1; i >= 0; i--) {
      for (int j = 0; j < infoHeader.biWidth; j++) {
        in.read((char *) &image.bitmap[i][j], RGB_CHANNELS);
      }
      in.seekg(paddingBytesPerRow, std::ios::cur);
    }
  }
  return in;
}
std::ostream &operator<<(std::ostream &out, BmpImage &image) {
  out.write((char *) &image.fileHeader, sizeof(BmpFileHeader));
  out.write((char *) &image.infoHeader, sizeof(BmpInfoHeader));
  out.flush();
  if (image.palette.empty()) {
    const uint32_t
        paddingBytesPerRow = getPaddingBytesPerRow(
        image.infoHeader.biWidth * RGB_CHANNELS * BITS_PER_CHANNEL);
    uint8_t placeholder = 0;
    for (int i = image.infoHeader.biHeight - 1; i >= 0; i--) {
      for (int j = 0; j < image.infoHeader.biWidth; j++) {
        out.write((char *) &image.bitmap[i][j], RGB_CHANNELS);
      }
      for (int t = 0; t < paddingBytesPerRow; t++) {
        out.write((char *) &placeholder, sizeof(uint8_t));
      }
    }
  } else {
    out.write((char *) &image.palette, sizeof(Pixel) * image.palette.size());
    out.write((char *) &image.storage, sizeof(uint8_t) * image.storage.size());
  }
  return out;
}

const vector<vector<Pixel>> &BmpImage::getBitmap() const {
  return bitmap;
}

void BmpImage::setPalette(const OctTree &tree) {
  const auto &nodeList = tree.getHeap();
  OctNode *root = tree.getRoot();

  palette.reserve(nodeList.size());
  for (int i = 0; i < nodeList.size(); i++) {
    palette[i] = nodeList[i];
//    map[nodeList[i]] = i;
    nodeList[i]->ptr = i;
  }

  storage.reserve(nodeList.size());
  for (int i = infoHeader.biHeight - 1; i >= 0; i--) {
    for (int j = 0; j < infoHeader.biWidth; j++) {
      Pixel pixel = bitmap[i][j];
      OctNode *altNode = getClosestColor(root, pixel.blue, pixel.green, pixel.red);
      storage.push_back((uint8_t) altNode->ptr);
//      storage.push_back(tmp);
    }
  }
}

OctNode *BmpImage::getClosestColor(OctNode *root, uint8_t blue, uint8_t green, uint8_t red) const {
  vector<uint8_t> indexes = OctTree::generateColorIndex(blue, green, red);
  for (int i = 0; i < OCT_NUM; i++) {
    OctNode *child = root->children[indexes[i]];
    if (child == nullptr) {
      return root;
    } else {
      root = child;
    }
  }
  return root;
}
