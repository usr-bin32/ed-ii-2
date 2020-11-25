#include <iostream>
#include <string>

#include "tests/balanced.h"
#include "tests/hashing.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Parâmetro não encontrado! Execute o programa como: "
                  << std::endl;
        std::cout << "./program.out <hashing ou balanced>" << std::endl;

        return 0;
    }

    if (argv[1] == std::string("hashing")) {
        test_hashing();
    } else if (argv[1] == std::string("balanced")) {
        test_balanced();
    } else {
        std::cout << "Parâmetro inválido! Execute o programa como: "
                  << std::endl;
        std::cout << "./program.out <hashing ou balanced>" << std::endl;
    }

    return 0;
}
