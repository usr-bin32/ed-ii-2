#ifndef BTREE_H
#define BTREE_H

#include "btree_node.h"

template <typename T>
class btree {
  public:
    btree(int degree1);
    ~btree();

    T *search(long key, int &comparisons);
    void insert(long key, T data, int &comparisons);
    void walk();
    void remove(long key);

  private:
    int degree;
    bnode<T> *root;
};

template <typename T>
btree<T>::btree(int degree1) {
    degree = degree1;
    root = nullptr;
}

template <typename T>
btree<T>::~btree() {
    //...
}

template <typename T>
T *btree<T>::search(long key, int &comparisons) {
    comparisons++;
    return (root == nullptr) ? nullptr : root->search(key, comparisons);
};

template <typename T>
void btree<T>::insert(long key, T data, int &comparisons) {
    comparisons++;
    if (root == nullptr) {
        root = new bnode<T>(degree, true);
        root->keys[0] = key;
        root->datas[0] = data;
        root->key_numbers = 1;
    } else {
        comparisons++;
        if (root->key_numbers == 2 * degree - 1) {
            bnode<T> *node = new bnode<T>(degree, false);
            node->child[0] = root;
            node->split(root, 0);

            int i = 0;
            if (node->keys[0] < key)
                i++;
            node->child[i]->insert_not_full(key, data, comparisons);

            root = node;
        } else {
            comparisons++;
            root->insert_not_full(key, data, comparisons);
        }
    }
}

template <typename T>
void btree<T>::walk() {
    if (root != nullptr)
        root->walk();
}

template <typename T>
void btree<T>::remove(long key) {
    if (!root) {
        std::cout << "Árvore vazia" << std::endl;
    }

    root->remove(key);

    if (root->key_numbers == 0) {
        bnode<T> *aux = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->child[0];

        delete aux;
    }
}

#endif // BTREE_H
