#include "simplebmp/simplebmp.h"

#include <cstdio>
#include <iostream>

namespace simplebmp {
namespace {

struct __attribute__((__packed__)) Header {
  // Bitmap header.
  uint8_t magic[2] = {'B', 'M'};
  uint32_t file_size;
  uint16_t reserved1 = 0;
  uint16_t reserved2 = 0;
  uint32_t pixel_array_offset = sizeof(Header);

  // Dib header.
  uint32_t dib_header_size = sizeof(Header) - offsetof(Header, dib_header_size);
  int32_t image_width;
  int32_t image_height;
  uint16_t planes = 1;
  uint16_t bits_per_pixel = 24;
  uint32_t compression = 0;  // BI_RGB
  uint32_t image_size = 0;
  int32_t x_pixels_per_meter = 1000;
  int32_t y_pixels_per_meter = 1000;
  uint32_t num_colors = 0;  // defaults to 2^n
  uint32_t num_important_colors = 0; // all.

  // Everything else is ignored for 24bit bitmaps.
};

class Closer {
  public:
    explicit Closer(FILE* fp) : fp_(fp) {}
    ~Closer() { fclose(fp_); }
  private:
    FILE* fp_;
};

}

std::optional<Image> Image::Load(const std::string& path) {
  FILE* fp = fopen(path.c_str(), "rb");
  if (fp == nullptr) {
    return std::nullopt;
  }
  Closer closer(fp);

  Header header;
  if (fread(&header, sizeof(Header), 1, fp) != 1) {
    return std::nullopt;
  }

  // Compatibility checks.
  if ((header.magic[0] != 'B' && header.magic[1] != 'M') || header.bits_per_pixel != 24 || header.compression != 0) {
    return std::nullopt;
  }

  Image image(header.image_width, header.image_height);
  fseek(fp, header.pixel_array_offset, SEEK_SET);

  size_t image_bytes = header.image_size ? header.image_size : header.image_width * header.image_height * sizeof(Color);
  if (fread(image.Data(), 1, image_bytes, fp) != image_bytes) {
    return std::nullopt;
  }

  return image;
}

bool Image::Write(const std::string& path) {
  FILE* fp = fopen(path.c_str(), "wb");
  if (fp == nullptr) {
    return false;
  }
  Closer closer(fp);

  Header header;
  header.file_size = sizeof(Header) + image_.size() * sizeof(Color);
  header.image_width = x_;
  header.image_height = y_;
  header.image_size = image_.size() * sizeof(Color);

  if (fwrite(&header, sizeof(header), 1, fp) != 1) {
    return false;
  }
  return fwrite(image_.data(), sizeof(Color), image_.size(), fp) == image_.size();
}

}