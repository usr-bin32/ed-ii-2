#ifndef BTREE_NODE_H
#define BTREE_NODE_H

#include <iostream>

template <typename T>
class bnode {
  public:
    bnode(int degree1, bool leaf1);
    ~bnode();

    int num_children;
    bnode **children;
    int num_keys;
    long *keys;
    T *values;
    bool leaf;

    int get_key(bnode *node, int index) { return node->keys[index]; };

    T *search(long key, int &comparisons);
    void insert_not_full(long key, T value, int &comparisons);
    void walk();
    void split(bnode *node, int index);
    void remove(long key);

  private:
    int degree;

    int search_key(long key);
    void remove_leaf(int i);
    void remove_not_leaf(int i);
    long get_predecessor(int i);
    long get_successor(int i);
    void fill_child(int i);
    void borrow_key_before(int i);
    void borrow_key_after(int i);
    void merge(int i);
    void delete_children();
};

template <typename T>
bnode<T>::bnode(int degree1, bool leaf1) {
    degree = degree1;
    leaf = leaf1;

    num_keys = 0;
    keys = new long[2 * degree - 1];
    values = new T[2 * degree - 1];

    num_children = 2 * degree;
    children = new bnode<T> *[num_children];
    for (int i = 0; i < num_children; i++) {
        children[i] = nullptr;
    }
}

template <typename T>
bnode<T>::~bnode() {
    delete[] keys;
    delete[] values;
    delete[] children;
}

template <typename T>
T *bnode<T>::search(long key, int &comparisons) {
    int i = 0;

    comparisons++;
    while (i < num_keys && key > keys[i]) {
        i++;
        comparisons++;
    }

    if (keys[i] == key) {
        return &values[i];
    }

    if (leaf == true)
        return nullptr;

    return children[i]->search(key, comparisons);
}

template <typename T>
void bnode<T>::walk() {
    int i;
    for (i = 0; i < num_keys; i++) {
        if (leaf == false && children[i] != nullptr)
            children[i]->walk();
    }
    if (leaf == false && children[i] != nullptr)
        children[i]->walk();
}

template <typename T>
void bnode<T>::insert_not_full(long key, T value, int &comparisons) {
    int i = num_keys - 1;

    if (leaf == true) {
        comparisons++;
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            values[i + 1] = values[i];
            i--;
            comparisons++;
        }
        keys[i + 1] = key;
        values[i + 1] = value;
        num_keys = num_keys + 1;
    }

    else {
        comparisons++;
        while (i >= 0 && keys[i] > key) {
            i--;
            comparisons++;
        }
        if (children[i + 1]->num_keys == 2 * degree - 1) {
            split(children[i + 1], i + 1);

            comparisons++;
            if (keys[i + 1] < key) {
                i++;
            }
        }
        children[i + 1]->insert_not_full(key, value, comparisons);
    }
}

template <typename T>
void bnode<T>::split(bnode *node, int index) {
    bnode<T> *node1 = new bnode(node->degree, node->leaf);
    node1->num_keys = degree - 1;

    for (int j = 0; j < degree - 1; j++) {
        node1->keys[j] = node->keys[j + degree];
        node1->values[j] = node->values[j + degree];
    }

    if (node->leaf == false) {
        for (int j = 0; j < degree; j++) {
            node1->children[j] = node->children[j + degree];
            node->children[j + degree] = nullptr;
        }
    }

    node->num_keys = degree - 1;

    for (int j = num_keys; j >= index + 1; j--) {
        children[j + 1] = children[j];
    }

    children[index + 1] = node1;

    for (int j = num_keys - 1; j >= index; j--) {
        keys[j + 1] = keys[j];
        values[j + 1] = values[j];
    }

    keys[index] = node->keys[degree - 1];
    values[index] = node->values[degree - 1];
    num_keys = num_keys + 1;
}

template <typename T>
int bnode<T>::search_key(long key) {
    int i = 0;
    for (i = 0; i < num_keys && keys[i] < key; ++i)
        ;
    return i;
}

template <typename T>
long bnode<T>::get_predecessor(int i) {
    bnode<T> *node = children[i];
    while (!node->leaf)
        node = node->children[node->num_keys];

    return node->keys[node->num_keys - 1];
}

template <typename T>
long bnode<T>::get_successor(int i) {
    bnode<T> *node = children[i + 1];
    while (!node->leaf)
        node = node->children[0];

    return node->keys[0];
}

template <typename T>
void bnode<T>::fill_child(int i) {
    if (i != num_keys && children[i + 1]->num_keys >= degree)
        borrow_key_before(i);

    else if (i != 0 && children[i - 1]->num_keys >= degree)
        borrow_key_after(i);

    else {
        if (i != num_keys)
            merge(i);
        else
            merge(i - 1);
    }
}

template <typename T>
void bnode<T>::borrow_key_before(int i) {
    bnode<T> *c = children[i];
    bnode<T> *sib = children[i - 1];

    for (int j = c->num_keys - 1; j >= 0; --j) {
        c->keys[j + 1] = c->keys[j];
        c->values[j + 1] = c->values[j];
    }

    if (!c->leaf) {
        for (int j = c->num_keys; j >= 0; --j)
            c->children[j + 1] = c->children[j];
    }

    c->keys[0] = keys[i - 1];
    c->values[0] = values[i - 1];

    if (!c->leaf)
        c->children[0] = sib->children[sib->num_keys];

    keys[i - 1] = sib->keys[sib->num_keys - 1];
    values[i - 1] = sib->values[sib->num_keys - 1];

    sib->num_keys = sib->num_keys - 1;
    c->num_keys = c->num_keys + 1;
}

template <typename T>
void bnode<T>::borrow_key_after(int i) {
    bnode<T> *c = children[i];
    bnode<T> *sib = children[i + 1];

    c->keys[(c->num_keys)] = keys[i];
    c->values[(c->num_keys)] = values[i];

    if (!c->leaf) {
        c->children[(c->num_keys) + 1] = sib->children[0];
    }

    keys[i] = sib->keys[0];
    values[i] = sib->values[0];

    for (int j = 1; j < sib->num_keys; ++j) {
        sib->keys[j - 1] = sib->keys[j];
        sib->values[j - 1] = sib->values[j];
    }

    if (!sib->leaf) {
        for (int j = 1; j <= sib->num_keys; ++j)
            sib->children[j - 1] = sib->children[j];
    }

    sib->num_keys = sib->num_keys - 1;
    c->num_keys = c->num_keys + 1;
}

template <typename T>
void bnode<T>::merge(int i) {
    bnode<T> *c = children[i];
    bnode<T> *sib = children[i + 1];

    c->keys[degree - 1] = keys[i];
    c->values[degree - 1] = values[i];

    for (int j = 0; j < sib->num_keys; ++j) {
        c->keys[j + degree] = sib->keys[j];
        c->values[j + degree] = sib->values[j];
    }

    if (!c->leaf) {
        for (int j = 0; j < sib->num_keys; ++j)
            c->children[degree + j] = sib->children[j];
    }

    for (int k = i + 1; k < num_keys; ++k) {
        keys[k - 1] = keys[k];
        values[k - 1] = values[k];
    }

    for (int z = i + 2; z <= num_keys; ++z)
        children[z - 1] = children[z];

    c->num_keys = c->num_keys + sib->num_keys + 1;
    num_keys--;

    delete sib;
    children[i + 1] = nullptr;
}

template <typename T>
void bnode<T>::remove(long key) {
    int i = search_key(key);
    if (i < num_keys && keys[i] == key) {
        if (!leaf)
            remove_not_leaf(i);
        else
            remove_leaf(i);
    } else {
        if (leaf)
            return;

        bool is_present = ((i == num_keys) ? true : false);

        if (children[i]->num_keys < degree)
            fill_child(i);

        if (is_present && i > num_keys)
            children[i - 1]->remove(key);

        else
            children[i]->remove(key);
    }
}

template <typename T>
void bnode<T>::remove_leaf(int i) {
    for (int j = i + 1; j < num_keys; ++j) {
        keys[j - 1] = keys[j];
        values[j - 1] = values[j];
    }

    num_keys--;
}

template <typename T>
void bnode<T>::remove_not_leaf(int i) {
    long key = keys[i];

    if (children[i]->num_keys >= degree) {
        long aux = get_predecessor(i);
        keys[i] = aux;
        values[i] = aux;
        children[i]->remove(aux);
    } else if (children[i + 1]->num_keys >= degree) {
        long aux = get_successor(i);
        keys[i] = aux;
        values[i] = aux;
        children[i + 1]->remove(aux);
    } else {
        merge(i);
        children[i]->remove(key);
    }
}

#endif // BTREE_NODE_H
