#include <iostream>
#include <string>

#include "tests/balanced.h"
#include "tests/hashing.h"

int main(int argc, char **argv) {
    // O programa possui dois modos: hashing e balanced. O primeiro modo executa
    // os testes relacionados à tabela hash e à contagem de frequência dos
    // autores. O segundo realiza os testes das árvores balanceadas com os
    // livros. O modo deve ser selecionado a partir dos argumentos da linha de
    // comando

    // Verifica a existência do parâmetro de modo
    if (argc < 2) {
        std::cout << "Parâmetro não encontrado! Execute o programa como: "
                  << std::endl;
        std::cout << "./program.out <hashing ou balanced>" << std::endl;

        return 0;
    }

    // Checa qual modo deve ser utilizado
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
