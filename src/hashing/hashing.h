#ifndef HASHING_H
#define HASHING_H
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "math.h"
#include <vector>
#include <list>

template <typename T>
struct hash_node{
   int key;     
   T value;              // recebe o id's de autores ou frequencia de um autor
   std::string info;    // recebe o nome de um autor ou informacoes de um livro
};

class hashing
{
   public:

      hashing (int N_book, int N_author);
      ~hashing ();
      void insert_book (int id_book, std::vector<int> ids_authors, std::string info);
      void insert_authors ();
      void print(int tam); //feita para testes, apagar dps
      
   private:

      int tam_books, tam_authors;
      std::vector<std::list<hash_node<int>>> authors;
      std::vector<std::list<hash_node<std::vector<int>>>> books;

      template <typename T> hash_node<T> aux_insert(int key, T value, std::string info);
      bool is_exist_author (int id); //verifica se o id do autor já foi inserido, se sim soma 1 na frequencia 

      //A função hash utilizada foi a da multiplicação
      //motivo da escolha se da por não se ter o conhecimento do valor N de entrada
      //utilizando esse método não teremos problemas se tamanho da hash for potência de 2 por exemplo.
      int function_hash(int id, int tam);

      //utlizando o id, recupera o nome do autor
     // void search_author();
};

#endif // HASHING_H