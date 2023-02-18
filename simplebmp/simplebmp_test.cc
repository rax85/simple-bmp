#include "simplebmp/simplebmp.h"

#include <filesystem>
#include <iostream>

#include "gtest/gtest.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace simplebmp {
namespace {

using bazel::tools::cpp::runfiles::Runfiles;

std::string ResolvePath(const std::string& path) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest(&error));
  EXPECT_TRUE(std::filesystem::exists(runfiles->Rlocation(path)));
  return runfiles->Rlocation(path);
}

TEST(SimpleBmpTests, LoadBmp) {
  std::optional<Image> maybe_image = Image::Load(ResolvePath("__main__/simplebmp/testdata/test.bmp"));
  ASSERT_NE(maybe_image, std::nullopt);

  const Image& image = maybe_image.value();
  EXPECT_EQ(image.Width(), 320);
  EXPECT_EQ(image.Height(), 240);

  EXPECT_EQ(image.At(0, 0), Color(255, 255, 255));
  EXPECT_EQ(image.At(64, 64), Color(0, 0, 0));
  EXPECT_EQ(image.At(68, 68), Color(255, 0, 0));
  EXPECT_EQ(image.At(81, 95), Color(0, 255, 0));
  EXPECT_EQ(image.At(111, 96), Color(0, 0, 255));
}

TEST(SimpleBmpTests, SaveBmp) {
  Image image(8, 12);
  image.At(1, 0) = Color(255, 0, 0);
  image.At(3, 2) = Color(0, 255, 0);
  image.At(5, 4) = Color(0, 0, 255);
  image.At(7, 6) = Color(255, 255, 255);

  std::filesystem::path path = std::filesystem::temp_directory_path().append("out.bmp");
  std::cout << "Manually verify " << path << std::endl;
  EXPECT_TRUE(image.Write(path.c_str()));
}

}
}