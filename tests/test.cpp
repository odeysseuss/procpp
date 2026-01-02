#include "lib.hpp"
#include <iostream>

int main() {
    auto cmd = Command("ls")
                   .arg("-lAvh")
                   .args({"--color=always", "--group-directories-first"})
                   .run();
    std::cout << cmd.stdout();
    std::cerr << cmd.stderr();
}
