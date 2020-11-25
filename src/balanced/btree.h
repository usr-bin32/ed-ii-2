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
    int degree;
    bnode<T> *root;

    void destroy(bnode<T> *node);
};

template <typename T>
btree<T>::btree(int degree1) {
    degree = degree1;
    root = nullptr;
}

template <typename T>
btree<T>::~btree() {
    destroy(this->root);
}

template <typename T>
T *btree<T>::search(long key, int &comparisons) {
    comparisons++;
    return (root == nullptr) ? nullptr : root->search(key, comparisons);
};

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

template <typename T>
void btree<T>::walk() {
    if (root != nullptr)
        root->walk();
}

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
