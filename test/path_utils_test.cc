#include <cassert>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

namespace {

std::optional<std::string> lexically_relative_path(const std::string &file_path,
                                                   const std::string &dir_path) {
  auto normalized_file = std::filesystem::path(file_path).lexically_normal();
  auto normalized_dir = std::filesystem::path(dir_path).lexically_normal();
  auto relative = normalized_file.lexically_relative(normalized_dir);
  if (relative.empty()) {
    return std::nullopt;
  }
  return relative.generic_string();
}

void expect_relative(const std::string &file_path, const std::string &dir_path,
                     const std::optional<std::string> &expected) {
  auto actual = lexically_relative_path(file_path, dir_path);
  if (actual != expected) {
    std::cerr << "Expected relative path from \"" << dir_path << "\" to \""
              << file_path << "\" to be ";
    if (expected) {
      std::cerr << "\"" << *expected << "\"";
    } else {
      std::cerr << "<nullopt>";
    }
    std::cerr << ", but got ";
    if (actual) {
      std::cerr << "\"" << *actual << "\"";
    } else {
      std::cerr << "<nullopt>";
    }
    std::cerr << std::endl;
    assert(false);
  }
}

} // namespace

int main() {
  expect_relative("/home/a/dir1/game/../obj_model.h", "/home/a/dir1/game",
                  "../obj_model.h");

  expect_relative("/home/a/dir1/engine/../game/../obj_model.h",
                  "/home/a/dir1/game", "../obj_model.h");

  expect_relative("/home/a/prog/dir1/engine/../game/../obj_model.h",
                  "/home/a/dir1/game", "../../prog/dir1/obj_model.h");

  expect_relative("/home/a/dir1/game/obj_model.h", "/home/a/dir1/game",
                  "obj_model.h");

  expect_relative("game/../obj_model.h", "game", "../obj_model.h");

  expect_relative("/home/a/dir1/game", "home/a/dir1/game", std::nullopt);

  expect_relative("/home/a/prog/dir1/engine/utils/quaternion.h",
                  "/home/a/prog/dir1/engine/..",
                  "engine/utils/quaternion.h");

  std::cout << "All path utility tests passed." << std::endl;
  return 0;
}
