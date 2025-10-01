#include <apiset/resolver.h>

#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <dllname-or-apisetname>" << std::endl;
    return 0;
  }

  apiset::Resolver resolver{};
  auto result = resolver.resolve(argv[1]);
  // Also available below:
  // auto result = resolver.resolve(std::string{argv[1]});

  if (result) {
    std::cout << argv[1] << " is ApisetName." << std::endl;
    std::cout << argv[1] << " is redirected below:" << std::endl;
    for (auto&& dllname : result.value()) {
      std::cout << "- " << dllname << std::endl;
    }
  } else {
    std::cout << argv[1] << " is DllName." << std::endl;
  }

  return 0;
}
