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
   int key;               // recebe o id de autor ou id de um livro
   T value;              // recebe os id's dos autores ou frequencia de um autor ou nome do autor
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

      int tam_books, tam_authors, tam_all_authors;
      std::vector<std::list<hash_node<int>>> authors;
      std::vector<std::list<hash_node<std::string>>> all_authors;
      std::vector<std::list<hash_node<std::vector<int>>>> books;

      template <typename T> hash_node<T> aux_insert(int key, T value, std::string info);
      template <typename T> void destroy (int tam, std::vector<std::list<hash_node<T>>> &table);
      void creat_all_authors();  // tabela hash contendo todos os autores do authors.csv
      bool is_exist_author (int id);  //verifica se o id do autor já foi inserido, se sim soma 1 na frequencia 
      std::string search_author(int id);  // recupera o nome do autor
      int function_hash(int id, int tam); 
      //A função hash utilizada foi a da multiplicação
      //motivo da escolha se da por não se ter o conhecimento do valor N de entrada
      //utilizando esse método não teremos problemas se tamanho da hash for potência de 2 por exemplo.
         
};

#endif // HASHING_H