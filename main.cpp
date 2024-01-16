#include "bst.hpp"

#include <iostream>

int main() {
  auto tree = put(put(put(empty<int, char>(), 42, 'a'), 10, 'b'), 101, 'd');

  auto demo = [&](auto name, auto order) {
    std::cout << name << ':' << std::endl;
    for_each(
        [](auto kv) {
          std::cout << std::get<0>(kv) << ", " << std::get<1>(kv) << std::endl;
        },
        order(tree));
    std::cout << std::endl;
  };

  demo("pre_order", pre_order<int, char>);
  demo("in_order", in_order<int, char>);
  demo("post_order", post_order<int, char>);

  std::cout << "Bye!" << std::endl;

  return 0;
}
