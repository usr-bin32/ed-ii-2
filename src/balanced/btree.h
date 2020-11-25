#ifndef BTREE_H
#define BTREE_H

#include "btree_node.h"

template <typename T>
class btree {
  public:
    btree(int degree1);
    ~btree();

    T *search(long key, int &comparisons);
    void insert(long key, T value, int &comparisons);
    void walk();
    void remove(long key);

  private:
    int degree; //grau da árvore
    bnode<T> *root; //raiz da árvore

    void destroy(bnode<T> *node);
};

//Construtor
template <typename T>
btree<T>::btree(int degree1) {
    degree = degree1;
    root = nullptr;
}

//Destrutor
template <typename T>
btree<T>::~btree() {
    destroy(this->root);
}

//Pesquisa uma chave na árvore
template <typename T>
T *btree<T>::search(long key, int &comparisons) {
    comparisons++;
    return (root == nullptr) ? nullptr : root->search(key, comparisons);
};

//Função principal para inserir uma nova chave na árvore
template <typename T>
void btree<T>::insert(long key, T value, int &comparisons) {
    comparisons++;
    if (root == nullptr) {
        root = new bnode<T>(degree, true);
        root->keys[0] = key;
        root->values[0] = value;
        root->num_keys = 1;
    } else {
        if (root->num_keys == 2 * degree - 1) {
            bnode<T> *node = new bnode<T>(degree, false);
            node->children[0] = root;
            node->split(root, 0);

            int i = 0;
            comparisons++;
            if (node->keys[0] < key) {
                i++;
            }
            node->children[i]->insert_not_full(key, value, comparisons);

            root = node;
        } else {
            root->insert_not_full(key, value, comparisons);
        }
    }
}

//Percorre todos os nós de uma subárvore enraizada neste nó
template <typename T>
void btree<T>::walk() {
    if (root != nullptr)
        root->walk();
}

//Função principal para remover uma chave da árvore
template <typename T>
void btree<T>::remove(long key) {
    root->remove(key);

    if (root->num_keys == 0) {
        bnode<T> *aux = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];

        delete aux;
    }
}

template <typename T>
void btree<T>::destroy(bnode<T> *node) {
    if (node != nullptr) {
        for (int i = 0; i < node->num_children; i++) {
            bnode<T> *next = node->children[i];
            node->children[i] = nullptr;
            destroy(next);
        }
        delete node;
    }
}

#endif // BTREE_H
