#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
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
    
    creat_all_authors();
}
 
hashing::~hashing ()
{

}

int hashing::function_hash(int id, int tam)
{
    double A = (sqrt(5) - 1)/2;   // valor sugerido por Knuth
    A =  A * id;
    A = (A - floor(A))*tam;     // A = (A mod 1)*N
    A = floor(A);
    return A;
}

void hashing::insert_book (int id_book, std::vector<int> ids_authors, std::string info)
{       
    books[function_hash(id_book, tam_books)].emplace_front(aux_insert<std::vector<int>>(id_book, ids_authors, info));
}

void hashing::insert_authors ()
{  
    std::string name;

    for(int i = 0; i < tam_books; i++)
    {
        for(hash_node<std::vector<int>> data : books[i])
        {   
            for (int k = 0; k < data.value.size(); k++)
            {   
                if(!is_exist_author(data.value[k]))
                {
                    name = search_author(data.value[k]);
                    authors[function_hash(data.value[k], tam_authors)].emplace_front(aux_insert<int>(data.value[k], 1, name));
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
    std::list<hash_node<int>> &l = authors[index];
 
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

std::string hashing::search_author(int id)
{
    int index = function_hash(id, tam_all_authors);
   
    for(hash_node<std::string> data : all_authors[index])
    {   
        if( data.key == id)
        {   
            return data.info;
        }
    } 

    return "not found";
}

void hashing::creat_all_authors()
{
    //..
}





///////////////////////////////////////////////////////////// 
//  teste 
////////////////////////////////////////////////////////////
void hashing::print(int tam)
{   
    std::cout << " \n == Imprimindo == \n";

    if( tam == tam_authors)
    {
        for(int i = 0; i < tam; i++)
        {
            for(hash_node<int> data : authors[i])
            {
                std::cout << " \n Indice: " << i <<" / id: " << data.key<< " / nome : " << data.info << " / frequencia: " << data.value;
            }
        }
    }
    else if(tam == tam_books){
        for(int i = 0; i < tam; i++)
        {
            for(hash_node<std::vector<int>> data : books[i])
            {
                std::cout << " \n Indice: "  << i <<" / id: " << data.key<< " / titulo : " << data.info; 
            }
        }
    }else{
        for(int i = 0; i < 100; i++)
        {  
            for(hash_node<std::string> data : all_authors[i])
            {
                std::cout << " \n Indice: "  << i <<" / id: " << data.key<< " / nome: " << data.value; 
            }
        }
    }

    std::cout << "\n";
}

