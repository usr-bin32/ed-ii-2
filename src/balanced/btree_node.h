#ifndef BTREE_NODE_H
#define BTREE_NODE_H

#include <iostream>

template <typename T>
class bnode {
  public:
    bnode(int degree1, bool leaf1);
    bnode **child;
    int key_numbers = 0;
    bool leaf;
    long *keys;
    T *values;

    int get_key(bnode *node, int index) { return node->keys[index]; };

    T *search(long key, int &comparisons);
    void insert_not_full(long key, T value, int &comparisons);
    void walk();
    void split(bnode *node, int index);
    void remove(long key);

  private:
    int degree;

    int search_key(long key, int &comparisons);
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

    keys = new long[2 * degree - 1];
    values = new T[2 * degree - 1];
    child = new bnode<T> *[2 * degree];

    key_numbers = 0;
}

template <typename T>
T *bnode<T>::search(long key, int &comparisons) {
    int i = 0;

    comparisons++;
    while (i < key_numbers && key > keys[i]) {
        i++;
        comparisons++;
    }

    if (keys[i] == key) {
        return &values[i];
    }

    if (leaf == true)
        return nullptr;

    return child[i]->search(key, comparisons);
}

template <typename T>
void bnode<T>::walk() {
    int i;
    for (i = 0; i < key_numbers; i++) {
        if (leaf == false)
            child[i]->walk();
    }
    if (leaf == false)
        child[i]->walk();
}

template <typename T>
void bnode<T>::insert_not_full(long key, T value, int &comparisons) {
    int i = key_numbers - 1;

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
        key_numbers = key_numbers + 1;
    }

    else {
        comparisons++;
        while (i >= 0 && keys[i] > key) {
            i--;
            comparisons++;
        }
        if (child[i + 1]->key_numbers == 2 * degree - 1) {
            split(child[i + 1], i + 1);

            comparisons++;
            if (keys[i + 1] < key) {
                i++;
            }
        }
        child[i + 1]->insert_not_full(key, value, comparisons);
    }
}

template <typename T>
void bnode<T>::split(bnode *node, int index) {
    bnode<T> *node1 = new bnode(node->degree, node->leaf);
    node1->key_numbers = degree - 1;

    for (int j = 0; j < degree - 1; j++) {
        node1->keys[j] = node->keys[j + degree];
        node1->values[j] = node->values[j + degree];
    }

    if (node->leaf == false) {
        for (int j = 0; j < degree; j++) {
            node1->child[j] = node->child[j + degree];
        }
    }

    node->key_numbers = degree - 1;

    for (int j = key_numbers; j >= index + 1; j--) {
        child[j + 1] = child[j];
    }

    child[index + 1] = node1;

    for (int j = key_numbers - 1; j >= index; j--) {
        keys[j + 1] = keys[j];
        values[j + 1] = values[j];
    }

    keys[index] = node->keys[degree - 1];
    values[index] = node->values[degree - 1];
    key_numbers = key_numbers + 1;
}

template <typename T>
int bnode<T>::search_key(long key, int &comparisons) {
    int i = 0;
    comparisons++;
    for (i = 0; i < key_numbers && keys[i] < key; ++i) {
        comparisons++;
    }
    return i;
}

template <typename T>
long bnode<T>::get_predecessor(int i) {
    bnode<T> *node = child[i];
    while (!node->leaf)
        node = node->child[node->key_numbers];

    return node->keys[node->key_numbers - 1];
}

template <typename T>
long bnode<T>::get_successor(int i) {
    bnode<T> *node = child[i + 1];
    while (!node->leaf)
        node = node->child[0];

    return node->keys[0];
}

template <typename T>
void bnode<T>::fill_child(int i) {
    if (i != key_numbers && child[i + 1]->key_numbers >= degree)
        borrow_key_before(i);

    else if (i != 0 && child[i - 1]->key_numbers >= degree)
        borrow_key_after(i);

    else {
        if (i != key_numbers)
            merge(i);
        else
            merge(i - 1);
    }
}

template <typename T>
void bnode<T>::borrow_key_before(int i) {
    bnode<T> *c = child[i];
    bnode<T> *sib = child[i - 1];

    for (int j = c->key_numbers - 1; j >= 0; --j) {
        c->keys[j + 1] = c->keys[j];
        c->values[j + 1] = c->values[j];
    }

    if (!c->leaf) {
        for (int j = c->key_numbers; j >= 0; --j)
            c->child[j + 1] = c->child[j];
    }

    c->keys[0] = keys[i - 1];
    c->values[0] = values[i - 1];

    if (!c->leaf)
        c->child[0] = sib->child[sib->key_numbers];

    keys[i - 1] = sib->keys[sib->key_numbers - 1];
    values[i - 1] = sib->values[sib->key_numbers - 1];

    sib->key_numbers = sib->key_numbers - 1;
    c->key_numbers = c->key_numbers + 1;
}

template <typename T>
void bnode<T>::borrow_key_after(int i) {
    bnode<T> *c = child[i];
    bnode<T> *sib = child[i + 1];

    c->keys[(c->key_numbers)] = keys[i];
    c->values[(c->key_numbers)] = values[i];

    if (!c->leaf) {
        c->child[(c->key_numbers) + 1] = sib->child[0];
    }

    keys[i] = sib->keys[0];
    values[i] = sib->values[0];

    for (int j = 1; j < sib->key_numbers; ++j) {
        sib->keys[j - 1] = sib->keys[j];
        sib->values[j - 1] = sib->values[j];
    }

    if (!sib->leaf) {
        for (int j = 1; j <= sib->key_numbers; ++j)
            sib->child[j - 1] = sib->child[j];
    }

    sib->key_numbers = sib->key_numbers - 1;
    c->key_numbers = c->key_numbers + 1;
}

template <typename T>
void bnode<T>::merge(int i) {
    bnode<T> *c = child[i];
    bnode<T> *sib = child[i + 1];

    c->keys[degree - 1] = keys[i];
    c->values[degree - 1] = values[i];

    for (int j = 0; j < sib->key_numbers; ++j) {
        c->keys[j + degree] = sib->keys[j];
        c->values[j + degree] = sib->values[j];
    }

    if (!c->leaf) {
        for (int j = 0; j < sib->key_numbers; ++j)
            c->child[degree + j] = sib->child[j];
    }

    for (int k = i + 1; k < key_numbers; ++k) {
        keys[k - 1] = keys[k];
        values[k - 1] = values[k];
    }

    for (int z = i + 2; z <= key_numbers; ++z)
        child[z - 1] = child[z];

    c->key_numbers = c->key_numbers + sib->key_numbers + 1;
    key_numbers--;

    delete sib;
}

template <typename T>
void bnode<T>::remove(long key) {
    int i = search_key(key);
    if (i < key_numbers && keys[i] == key) {
        if (!leaf)
            remove_not_leaf(i);
        else
            remove_leaf(i);
    } else {
        if (leaf)
            return;

        bool is_present = ((i == key_numbers) ? true : false);

        if (child[i]->key_numbers < degree)
            fill_child(i);

        if (is_present && i > key_numbers)
            child[i - 1]->remove(key);

        else
            child[i]->remove(key);
    }
}

template <typename T>
void bnode<T>::remove_leaf(int i) {
    for (int j = i + 1; j < key_numbers; ++j) {
        keys[j - 1] = keys[j];
        values[j - 1] = values[j];
    }

    key_numbers--;
}

template <typename T>
void bnode<T>::remove_not_leaf(int i) {
    long key = keys[i];

    if (child[i]->key_numbers >= degree) {
        long aux = get_predecessor(i);
        keys[i] = aux;
        values[i] = aux;
        child[i]->remove(aux);
    } else if (child[i + 1]->key_numbers >= degree) {
        long aux = get_successor(i);
        keys[i] = aux;
        values[i] = aux;
        child[i + 1]->remove(aux);
    } else {
        merge(i);
        child[i]->remove(key);
    }
}

#endif // BTREE_NODE_H
