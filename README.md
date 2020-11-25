# Trabalho de Estrutura de Dados II: Parte 2

Segunda parte do trabalho da disciplina de Esturutra de Dados II (2020.1-B).

## Compilação

O projeto foi testado com o compilador GCC (G++) tanto no Linux quanto sua versão no Windows (MinGW) e é compatível com C++ 11 ou mais novo. Para compilar o projeto, basta executar o makefile na pasta raiz do projeto (comando `make`) ou rodar o seguinte comando:

```sh
g++ src/main.cpp src/tests/hashing.cpp src/tests/common.cpp src/tests/balanced.cpp src/structures/author.cpp src/parsing/csv_parser.cpp -Ofast -std=c++11 -o ./project.out
```

## Modo de Uso

Antes de executar o programa, certifique-se de que o arquivo .csv de autores seja colocado no diretório `./res/authors.csv` e que o arquivo de livros esteja em `./res/books.csv`.

O programa possui dois modos: `hashing` e `balanced`. O primeiro modo corresponde aos testes de tabela hash, enquanto o segundo é responsável por executar os testes das árvores balanceadas. O usuário deve selecionar o modo desejado através de um argumento de linha de comando, como nos seguintes exemplos: `./program.out hashing` e `./program.out balanced`.

### Tabela Hash
Ao selecionar a opção `hashing`, o usuário deve informar os valores de N e M através da entrada padrão. Após o processamento, os resultados são impressos na saída padrão. A interação é exemplificada a seguir.

```sh
Insira quantos livros devem ser lidos: 100000
Insira quantos autores devem ser impressos: 15

Martha Day Zschock: 337
My Unique Books: 286
Janice Singer: 260
Yourdinonotes Publishing: 202
Lauren Lopez: 122
Vipul Gupta: 105
Colin Watson: 104
Desconhecido: 101
H. Boyesen H.: 91
Emily Christie: 86
Michael Pöschl: 81
Desconhecido: 81
Distinctive Journals: 80
Robert Pritchard: 80
Ying Zhu: 79
```

### Estruturas Balanceadas
Antes de selecionar a opção `balanced`, o usuário deve verificar se o arquivo de entrada `entrada.txt` está devidamente configurado com os valores de N a serem testados. Ao executar o programa nesse modo, o usuário também deve informar a fração das buscas que será feita com chaves aleatórias através da entrada padrão. Os resultados do processamento são impressos tanto na saída padrão, quanto nos arquivos de saída solicitados na descrição do trabalho (`saidaBusca.txt` e `saidaInsercao.txt`). Veja o exemplo de uma interação a seguir.

```sh
Insira a fração das chaves buscadas que será aleatória (0 a 1): 0.5

N = 1000
Árvore Vermelho-Preto:
    Comparações nas Inserções: 9800
    Comparações nas Buscas:    9487
    Tempo nas Inserções (s):   0.0002536
    Tempo nas Buscas (s):      3.2e-05
Árvore B (d = 2):
    Comparações nas Inserções: 12683
    Comparações nas Buscas:    13524
    Tempo nas Inserções (s):   0.0002916
    Tempo nas Buscas (s):      8.54e-05
Árvore B (d = 20):
    Comparações nas Inserções: 25037
    Comparações nas Buscas:    32685
    Tempo nas Inserções (s):   0.0002024
    Tempo nas Buscas (s):      6.52e-05

N = 500000
Árvore Vermelho-Preto:
    Comparações nas Inserções: 9450678
    Comparações nas Buscas:    9254218
    Tempo nas Inserções (s):   0.409461
    Tempo nas Buscas (s):      0.154003
Árvore B (d = 2):
    Comparações nas Inserções: 12713747
    Comparações nas Buscas:    12942937
    Tempo nas Inserções (s):   0.440096
    Tempo nas Buscas (s):      0.340126
Árvore B (d = 20):
    Comparações nas Inserções: 25808258
    Comparações nas Buscas:    28206599
    Tempo nas Inserções (s):   0.18015
    Tempo nas Buscas (s):      0.118686
```
