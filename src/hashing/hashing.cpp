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
    destroy(tam_authors, authors);
  
    // all_authors e books são destruidos após seus dados serem inseridos em authors
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

    destroy(tam_books, books);
    destroy(tam_all_authors, all_authors);
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
    std::ifstream data("hashing/authors.csv");
    std::string line, auxLine;
    int lineId;

    if(data.is_open())
    {   
        tam_all_authors = std::count(std::istreambuf_iterator<char>(data), std::istreambuf_iterator<char>(), '\n'); // num de lines do csv
        tam_all_authors = tam_all_authors + int(tam_all_authors * 0.35);  // porcetagem de espaço adicional
        all_authors.resize(tam_all_authors);
        data.clear();
        data.seekg (0);

        getline(data,line); // pula o cabeçario da tabela

        while(getline(data,line))
        {   
            auxLine = line.substr(0, line.find("\",\""));   // separa o id do restante da string
            auxLine.erase(0, 1);
            
            try{lineId = stoll(auxLine);}catch(std::invalid_argument){lineId = 0;} 
            line.erase(0, line.find("\",\"") +3);   // separa o nome do autor do restante da string
            line.erase(line.size() - 1);

            all_authors[function_hash(lineId, tam_all_authors)].emplace_front(aux_insert<std::string>(lineId, line, line)); 
        }

    }else std::cout << "Error opening file 'authors.csv' ";

    data.close(); 
}

template<typename T> void hashing::destroy(int tam, std::vector<std::list<hash_node<T>>> &table)
{
    for (int i = 0; i < tam; i++)
    {
		table[i].clear();
	}

    table.clear();
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

