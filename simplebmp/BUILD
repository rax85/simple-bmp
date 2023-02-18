cc_library(
  name = "simplebmp",
  srcs = ["simplebmp.cc"],
  hdrs = ["simplebmp.h"],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "simplebmp_test",
  srcs = ["simplebmp_test.cc"],
  data = glob(["testdata/**"]),
  deps = [
    ":simplebmp",
    "@bazel_tools//tools/cpp/runfiles",
    "@com_google_googletest//:gtest_main",
  ],
)