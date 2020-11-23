#ifndef RED_BLACK_H
#define RED_BLACK_H

#include "red_black_node.h"

template <typename T>
class red_black_tree {
  private:
    rbnode<T> *root;

  protected:
    void rotate_left(rbnode<T> *&, rbnode<T> *&);
    void rotate_right(rbnode<T> *&, rbnode<T> *&);
    void fix_violation(rbnode<T> *&, rbnode<T> *&);

  public:
    // Constructor
    red_black_tree() {
        root = nullptr;
    };
    void insert(T data, int key, int &comparisons);
    T search(int key, int &comparisons);
};

template <typename T>
rbnode<T> *aux_insert(rbnode<T> *root, rbnode<T> *pt, int &comparisons) {
    /* Se a árvore estiver vazia, retorna um novo nó */
    comparisons++;
    if (root == nullptr)
        return pt;

    /* Caso contrário, volte para baixo na árvore */
    comparisons++;
    if (pt->key < root->key) {
        root->left = aux_insert(root->left, pt);
        root->left->parent = root;
    } else {
        comparisons++;
        if (pt->key > root->key) {
            root->right = aux_insert(root->right, pt);
            root->right->parent = root;
        }
    }

    /* retornar o ponteiro do nó (inalterado) */
    return root;
};

template <typename T>
T red_black_tree<T>::search(int key, int &comparisons) {
    comparisons++;
    if (this->root->key == key) {
        return root->data;
    } else {
        return aux_search(this->root, key, comparisons);
    }
}

template <typename T>
T aux_search(rbnode<T> *aux, int key, int &comparisons) {
    comparisons++;
    if (aux->key > key) {
        return aux_search(aux->left, key);
    } else {
        comparisons++;
        if (aux->key < key) {
            return aux_search(aux->right, key);
        } else {
            return aux;
        }
    }
}

template <typename T>
void red_black_tree<T>::rotate_left(rbnode<T> *&root, rbnode<T> *&pt) {
    rbnode<T> *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != nullptr)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
};

template <typename T>
void red_black_tree<T>::rotate_right(rbnode<T> *&root, rbnode<T> *&pt) {
    rbnode<T> *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != nullptr)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
};

// verifica as condições ao se adicionar um novo nó
template <typename T>
void red_black_tree<T>::fix_violation(rbnode<T> *&root, rbnode<T> *&pt) {
    rbnode<T> *parent_pt = nullptr;
    rbnode<T> *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
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
                    rotate_left(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt é filho esquerdo de seu pai. Rotação à direita necessária
                */
                rotate_right(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
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
                    rotate_right(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                pt é filho direito de seu pai - rotação para a esquerda
                necessária
                */
                rotate_left(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
};

template <typename T>
// inseri um novo valor na arvore
void red_black_tree<T>::insert(T data, int key, int &comparisons) {
    rbnode<T> *pt = new rbnode<T>(data, key);

    // insere o nó
    root = aux_insert(root, pt, comparisons);

    // verifica as condições para inserção do novo nó
    fix_violation(root, pt);
};

#endif