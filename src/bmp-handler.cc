//
// Copyright [2019] <Zhi Sun>
//

#include <bmp-handler.h>

#include <fstream>
using std::ifstream;
using std::ofstream;

BmpHandler::BmpHandler(string filename) {
  ifstream fin(filename, std::ios::binary);
  BmpFileHeader fileHeader;
  BmpInfoHeader infoHeader;
  bool isOpen = fin.is_open();
  fin >> fileHeader;
  fin.read((char *) &infoHeader, sizeof(BmpInfoHeader));
  fin.close();
}
std::istream &operator>>(std::istream &in, BmpFileHeader &header) {
  in.read((char *)&header.bfType, 2);
  in.read((char *)&header.bfSize, 4);
  in.read((char *)&header.bfReserved1, 2);
  in.read((char *)&header.bfReserved2, 2);
  in.read((char *)&header.bfOffBits, 4);
  return in;
}
std::istream &operator>>(std::istream &in, BmpInfoHeader &header) {
  in.read((char *)&header.biSize, 4);
  in.read((char *)&header.biWidth, 4);
  in.read((char *)&header.biHeight, 4);
  in.read((char *)&header.biPlanes, 2);
  in.read((char *)&header.biBitCount, 2);
  in.read((char *)&header.biCompression, 4);
  in.read((char *)&header.biSizeImage, 4);
  in.read((char *)&header.biXPixelsPerMeter, 4);
  in.read((char *)&header.biYPixelsPerMeter, 4);
  in.read((char *)&header.biClrUsed, 4);
  in.read((char *)&header.biClrImportant, 4);
  return in;
}
