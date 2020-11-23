#ifndef BTREE_H
#define BTREE_H

#include "btree_node.h"

template <typename T>
class btree {
  public:
    int degree;
    node<T> *root;
    btree(int degree1);

    node<T> *search(int key);
    void insert(int key);
    void walk();
    void remove(int key);
};

template <typename T>
btree<T>::btree(int degree1) {
    degree = degree1;
    root = nullptr;
}

template <typename T>
node<T> *btree<T>::search(int key) {
    return (root == nullptr) ? nullptr : root->search(key);
};

template <typename T>
void btree<T>::insert(int key) {
    if (root == nullptr) {
        root = new node(degree, true);
        root->keys[0] = key;
        root->key_numbers = 1;
    } else {
        if (root->key_numbers == 2 * degree - 1) {
            node<T> *node = new node(degree, false);
            node->child[0] = root;
            node->split(root, 0);

            int i = 0;
            if (node->keys[0] < key)
                i++;
            node->child[0]->insert_not_full(key);

            root = node;
        } else
            root->insert_not_full(key);
    }
}

template <typename T>
void btree<T>::walk() {
    if (root != nullptr)
        root->walk();
}

template <typename T>
void btree<T>::remove(int key) {
    if (!root) {
        cout << "Arvore vazia" << endl;
    }

    root->remove(key);

    if (root->key_numbers == 0) {
        node<T> *aux = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->child[0];

        delete aux;
    }
}

#endif // BTREE_H
