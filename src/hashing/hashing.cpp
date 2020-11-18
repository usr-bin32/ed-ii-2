#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "math.h"
#include "hashing.h"
#include <vector>
#include <list>

hashing::hashing (int N_book, int N_author)
{   
    tam_books = N_book;        
    tam_authors = N_author;   
    books.resize(tam_books);
    authors.resize(tam_authors);
}

hashing::~hashing ()
{
    for (int i = 0; i < tam_authors; i++)
    {
		authors[i].clear();
	}

    for (int i = 0; i < tam_books; i++)
    {
		books[i].clear();
	}

    authors.clear();
    books.clear();
}

int hashing::function_hash(int id, int tam)
{
    double A = (sqrt(5) - 1)/2;   // valor sugerido por Knuth
    A =  A * id;
    A = (A - floor(A))*tam;   // A = (A mod 1)*N
    A = floor(A);

    return A;
}

void hashing::insert_book (int id_book, std::vector<int> ids_authors, std::string info)
{       
    hash_node<std::vector<int>> aux;
    aux = aux_insert<std::vector<int>>(id_book, ids_authors, info);
    books[function_hash(id_book, tam_books)].emplace_front(aux);
}

void hashing::insert_authors ()
{  
    for(int i = 0; i < tam_books; i++)
    {
        for(hash_node<std::vector<int>> data : books[i])
        {   
            for (int k = 0; k < data.value.size(); k++)
            {   
                if(!is_exist_author(data.value[k]))
                {
                    hash_node<int> aux;
                    aux = aux_insert<int>(data.value[k], 1, "nome do autor");
                    authors[function_hash(data.value[k], tam_authors)].emplace_front(aux);
                }
            }
            
        }
    }
}
     
template <typename T> hash_node<T> hashing::aux_insert(int key, T value, std::string info)
{   
    hash_node<T> aux;

    aux.key = key;
    aux.info = info;
    aux.value = value;
   
    return aux;
}

bool hashing::is_exist_author (int id)
{  
    int index = function_hash(id, tam_authors);
  
    std::list<hash_node<int>> &l =authors[index];
 
    for (auto it = l.rbegin(); it != l.rend(); it++)
    {   
        if( it->key == id)
        {   

            it->value = it->value +1;

            return true;
        }
    } 
  
    return false;
}

void hashing::print(int tam)
{   
    std::cout << " \n == Imprimindo == \n";

    if( tam == tam_authors)
    {
        for(int i = 0; i < tam; i++)
        {
            for(hash_node<int> data : authors[i])
            {
                std::cout << " \n Indice: " << i <<" / Key: " << data.key<< " / Info : " << data.info << " / Valor: " << data.value;
            }
        }
    }
    else{
        for(int i = 0; i < tam; i++)
        {
            for(hash_node<std::vector<int>> data : books[i])
            {
                std::cout << " \n Indice: "  << i <<" / Key: " << data.key<< " / Info : " << data.info; 
            }
        }
    }

    std::cout << "\n";
}

