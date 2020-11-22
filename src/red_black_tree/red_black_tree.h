#include <bits/stdc++.h>
#include "structures/book.h"

using namespace std;

enum Color
{
    RED,
    BLACK
};

template <typename T>
struct Node
{
    T data;
    int key;
    bool color;
    Node *left, *right, *parent;

    // Constructor
    Node(T data, int key)
    {
        this->data = data;
        this->key = key;
        left = right = parent = NULL;
        this->color = RED;
    }
};
template <typename T>
class red_black_tree
{
private:
    Node<T> *root;

protected:
    void rotate_left(Node<T> *&, Node<T> *&);
    void rotate_right(Node<T> *&, Node<T> *&);
    void fix_violation(Node<T> *&, Node<T> *&);

public:
    // Constructor
    red_black_tree() { root = NULL; };
    void insert(T data, int key, int &comparisonsCounter);
    T search(int key, int &comparisonsCounter);
};

template <typename T>
Node<T> *aux_insert(Node<T> *root, Node<T> *pt, int &comparisonsCounter)
{

    /* Se a árvore estiver vazia, retorna um novo nó */
    comparisonsCounter++;
    if (root == NULL)
        return pt;

    /* Caso contrário, volte para baixo na árvore */
    comparisonsCounter++;
    if (pt->key < root->key)
    {
        root->left = aux_insert(root->left, pt);
        root->left->parent = root;
    }
    else
    {
        comparisonsCounter++;
        if (pt->key > root->key)
        {
            root->right = aux_insert(root->right, pt);
            root->right->parent = root;
        }
    }

    /* retornar o ponteiro do nó (inalterado) */
    return root;
};
template <typename T>
T red_black_tree<T>::search(int key, int &comparisonsCounter)
{
    comparisonsCounter++;
    if (this->root->key == key)
    {
        return root->data;
    }
    else
    {
        return aux_search(this->root, key, comparisonsCounter);
    }
}

template <typename T>
T aux_search(Node<T> *aux, int key, int &comparisonsCounter)
{
    comparisonsCounter++;
    if (aux->key > key)
    {
        return aux_search(aux->left, key);
    }
    else
    {
        comparisonsCounter++;
        if (aux->key < key)
        {
            return aux_search(aux->right, key);
        }
        else
        {
            return aux;
        }
    }
}

template <typename T>
void red_black_tree<T>::rotate_left(Node<T> *&root, Node<T> *&pt)
{
    Node<T> *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
};
template <typename T>
void red_black_tree<T>::rotate_right(Node<T> *&root, Node<T> *&pt)
{
    Node<T> *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
};
template <typename T>
//verifica as condições ao se adicionar um novo nó
void red_black_tree<T>::fix_violation(Node<T> *&root, Node<T> *&pt)
{
    Node<T> *parent_pt = NULL;
    Node<T> *grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A            
            O pai do pt é filho esquerdo do avô do pt 
        */
        if (parent_pt == grand_parent_pt->left)
        {

            Node<T> *uncle_pt = grand_parent_pt->right;

            /* Case : 1 
               O tio de pt também é vermelho. Somente recoloração necessária
            */
            if (uncle_pt != NULL && uncle_pt->color ==
                                        RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2 
                   pt é filho direito de seu pai - rotação para a esquerda necessária
                */
                if (pt == parent_pt->right)
                {
                    rotate_left(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3 
                   pt é filho esquerdo de seu pai. Rotação à direita necessária
                */
                rotate_right(root, grand_parent_pt);
                swap(parent_pt->color,
                     grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        /* Case : B 
           O pai do pt é filho certo do avô do pt
        */
        else
        {
            Node<T> *uncle_pt = grand_parent_pt->left;

            /*  Case : 1 
                O tio de pt também é vermelho. Somente recoloração necessária
            */
            if ((uncle_pt != NULL) && (uncle_pt->color ==
                                       RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2            
                pt é filho esquerdo de seu pai. Rotação à direita necessária
                */
                if (pt == parent_pt->left)
                {
                    rotate_right(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                pt é filho direito de seu pai - rotação para a esquerda necessária 
                */
                rotate_left(root, grand_parent_pt);
                swap(parent_pt->color,
                     grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
};

template <typename T>
// inseri um novo valor na arvore
void red_black_tree<T>::insert(T data, int key, int &comparisonsCounter)
{
    Node<T> *pt = new Node<T>(data, key);

    // insere o nó
    root = aux_insert(root, pt, comparisonsCounter);

    // verifica as condições para inserção do novo nó
    fix_violation(root, pt);
};
