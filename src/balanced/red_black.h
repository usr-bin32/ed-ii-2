#ifndef RED_BLACK_H
#define RED_BLACK_H

#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>

#include "red_black_node.h"

template <typename T>
class red_black_tree {
  public:
    red_black_tree() { root = nullptr; };
    ~red_black_tree() { destroy(root); }

    void insert(int key, T data, int &comparisons);
    T *search(int key, int &comparisons);
    void print();
    void aux_print(rbnode<T> *&, int key);

  private:
    rbnode<T> *root;

    void rotate_left(rbnode<T> *&node, rbnode<T> *&pt);
    void rotate_right(rbnode<T> *&node, rbnode<T> *&pt);
    void fix_violation(rbnode<T> *&node, rbnode<T> *&pt);
    void destroy(rbnode<T> *&node);
};

template <typename T>
rbnode<T> *aux_insert(rbnode<T> *node, rbnode<T> *pt, int &comparisons) {
    /* Se a árvore estiver vazia, retorna um novo nó */
    comparisons++;
    if (node == nullptr) {
        return pt;
    } else {
        /* Caso contrário, volte para baixo na árvore */
        if (pt->key < node->key) {
            node->left = aux_insert(node->left, pt, comparisons);
            node->left->parent = node;
        } else {
            node->right = aux_insert(node->right, pt, comparisons);
            node->right->parent = node;
        }
    }

    /* retornar o ponteiro do nó (inalterado) */
    return node;
};

template <typename T>
void red_black_tree<T>::print() {
    std::cout << std::endl << std::endl;
    std::cout << "******************** ARVORE **************************"
              << std::endl
              << std::endl;
    aux_print(root, root->key);
    std::cout << std::endl << std::endl;
    std::cout << "******************************************************";
    std::cout << std::endl << std::endl;
}

template <typename T>
void red_black_tree<T>::aux_print(rbnode<T> *&p, int key) {
    if (p != nullptr) {
        aux_print(p->right, key + 1);
        for (int i = 0; i < key; i++)
            std::cout << '\t';
        std::cout << p->key << std::endl;
        aux_print(p->left, key + 1);
    }
}

template <typename T>
T *red_black_tree<T>::search(int key, int &comparisons) {
    if (this->root->key == key) {
        return &root->data;
    } else {
        return aux_search(this->root, key, comparisons);
    }
}

template <typename T>
T *aux_search(rbnode<T> *node, int key, int &comparisons) {
    comparisons++;
    if (node->key == key) {
        return &node->data;
    } else {
        if (node->key > key) {
            return aux_search(node->left, key, comparisons);
        } else {
            return aux_search(node->right, key, comparisons);
        }
    }
    return nullptr;
}

template <typename T>
void red_black_tree<T>::rotate_left(rbnode<T> *&node, rbnode<T> *&pt) {
    rbnode<T> *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != nullptr)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
        node = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
};

template <typename T>
void red_black_tree<T>::rotate_right(rbnode<T> *&node, rbnode<T> *&pt) {
    rbnode<T> *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != nullptr)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
        node = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
};

// verifica as condições ao se adicionar um novo nó
template <typename T>
void red_black_tree<T>::fix_violation(rbnode<T> *&node, rbnode<T> *&pt) {
    rbnode<T> *parent_pt = nullptr;
    rbnode<T> *grand_parent_pt = nullptr;

    while ((pt != node) && (pt->color != BLACK) && (pt->parent->color == RED)) {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
            O pai do pt é filho esquerdo do avô do pt
        */
        if (parent_pt == grand_parent_pt->left) {
            rbnode<T> *uncle_pt = grand_parent_pt->right;

            /* Case : 1
               O tio de pt também é vermelho. Somente recoloração necessária
            */
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else {
                /* Case : 2
                   pt é filho direito de seu pai - rotação para a esquerda
                   necessária
                */
                if (pt == parent_pt->right) {
                    rotate_left(node, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt é filho esquerdo de seu pai. Rotação à direita necessária
                */
                rotate_right(node, grand_parent_pt);
                bool aux_color = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = aux_color;
                pt = parent_pt;
            }
        }

        /* Case : B
           O pai do pt é filho certo do avô do pt
        */
        else {
            rbnode<T> *uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                O tio de pt também é vermelho. Somente recoloração necessária
            */
            if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                /* Case : 2
                pt é filho esquerdo de seu pai. Rotação à direita necessária
                */
                if (pt == parent_pt->left) {
                    rotate_right(node, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                pt é filho direito de seu pai - rotação para a esquerda
                necessária
                */
                rotate_left(node, grand_parent_pt);
                bool aux_color = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = aux_color;

                pt = parent_pt;
            }
        }
    }
    node->color = BLACK;
};

// insere um novo valor na arvore
template <typename T>
void red_black_tree<T>::insert(int key, T data, int &comparisons) {
    rbnode<T> *pt = new rbnode<T>(data, key);

    // insere o nó
    this->root = aux_insert(this->root, pt, comparisons);

    // verifica as condições para inserção do novo nó
    fix_violation(root, pt);
};

template <typename T>
void red_black_tree<T>::destroy(rbnode<T> *&node) {
    if (node != nullptr) {
        destroy(node->left);
        destroy(node->right);

        delete node;
        node = nullptr;
    }
}

#endif
