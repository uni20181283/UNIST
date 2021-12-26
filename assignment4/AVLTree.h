#include "Tree.h"

template <class keyT, class valT>
class AVLTree_t : public Tree_t<keyT, valT> {
    long check_bf(Node_t<keyT, valT>* n) {
        // Checks balance factor values in tree.
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = check_bf(n->right);
        hl = check_bf(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        long bf = hl - hr;
        assert(bf == n->meta);
        assert(bf < 2);
        assert(bf > -2);
        return h;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    Node_t<keyT, valT>* balance(Node_t<keyT, valT>* a) {
        // Make tree balanced
    }

    Node_t<keyT, valT>* insert_internal(keyT key, valT value) {
        // Helper function for insert()
    }

    void remove_node(Node_t<keyT, valT>* n) {
        // Helper function for remove()
    }

public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.
        Node_t<keyT, valT>* A_node = Tree_t<keyT, valT>::insert_internal(key, value);
        while(A_node){
            A_node->meta = Node_bf(A_node);
            if(A_node->meta == 2){
                if(Node_bf(A_node->left) == -1){ // LR
                    Tree_t<keyT, valT>::rotate(A_node->left, false);
                    Tree_t<keyT, valT>::rotate(A_node, true);
                    pre_order_SET_BF(this->root);
                }
                else if(Node_bf(A_node->right) == 1){   // LL
                    Tree_t<keyT, valT>::rotate(A_node, true);
                    A_node->meta = Node_bf(A_node);
                }
                A_node = search_subtree(this->root, key);
            }
            else if(A_node->meta == -2){
                if(Node_bf(A_node->right) == -1){  //  RR
                    Tree_t<keyT, valT>::rotate(A_node, false);
                    A_node->meta = Node_bf(A_node);
                }
                else if(Node_bf(A_node->right) == 1){   // RL
                    Tree_t<keyT, valT>::rotate(A_node->right, true);
                    Tree_t<keyT, valT>::rotate(A_node, false);
                    pre_order_SET_BF(this->root);
                }
                A_node = search_subtree(this->root, key);
            }
            else{
                A_node = A_node->parent;
            }
        }
        pre_order_SET_BF(this->root);
    }

    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        bool rt = Tree_t<keyT, valT>::remove(key);
        pre_order_SET_BF(this->root);
        return rt;
    }

    short Node_bf(Node_t<keyT, valT>* n){
        // Checks balance factor values in tree.
        short hr, hl;
        hr = hl = 0;
        if(!n) return 0;
        hr = height(n->right);
        hl = height(n->left);
        short bf = hl - hr;
        return bf;
    }
    short height(Node_t<keyT, valT>* n){
        // Checks balance factor values in tree.
        short h, hr, hl;
        hr = hl = 0;
        if(!n) return 0;
        hr = height(n->right);
        hl = height(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        return h;
    }
    Node_t<keyT, valT>* search_subtree(Node_t<keyT, valT>* root, keyT key) const {
        if(key == root->key) return root;
        else if (key < root->key) {
            if(root->left)
                return search_subtree(root->left, key);
            else
                return root;
        }
        else {
            if(root->right) return search_subtree(root->right, key);
            else return root;
        }
    }
    void pre_order_SET_BF(Node_t<keyT, valT>* node) {
        node->meta = Node_bf(node);
        if(node->left) pre_order_SET_BF(node->left);
        if(node->right) pre_order_SET_BF(node->right);
    }
};

//  g++ -o avl AVLtest.cpp -Wall -std=c++11 -O3
