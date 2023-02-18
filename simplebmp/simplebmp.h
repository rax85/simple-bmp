#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace simplebmp {

struct __attribute__((__packed__)) Color {
  uint8_t b;
  uint8_t g;
  uint8_t r;

  Color(uint8_t red, uint8_t green, uint8_t blue) : b(blue), g(green), r(red) {}
  Color() : b(0), g(0), r(0) {}

  bool operator ==(const Color& o) const {
    return r == o.r && g == o.g && b == o.b;
  }
};

/** @brief Reads and writes 24bit bmp files. */
class Image {
  public:
    Image(size_t width, size_t height) : x_(width), y_(height), image_(width * height) {}

    static std::optional<Image> Load(const std::string& path);
    bool Write(const std::string& path);

    const Color& At(size_t x, size_t y) const { return image_[(y_ - y - 1) * x_ + x]; }
    Color& At(size_t x, size_t y) { return image_[(y_ - y - 1) * x_ + x]; }

    size_t Width() const { return x_; }
    size_t Height() const { return y_; }
    Color* Data() { return image_.data(); }

  private:
    Image() = delete;

    size_t x_;
    size_t y_;
    std::vector<Color> image_;
};

}