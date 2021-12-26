#include "Tree.h"

#define BLACK 0
#define RED 1

template <class keyT, class valT>
class RBTree_t : public Tree_t<keyT, valT> {
    size_t check_rb(Node_t<keyT, valT>* n) {
        // Checks RB tree property
        size_t b, br, bl;
        br = bl = b = 1;
        assert(n);
        if(n && n->right) {
            br = check_rb(n->right);
            assert(n->meta == BLACK || n->right->meta == BLACK);
        }
        if(n && n->left) {
            bl = check_rb(n->left);
            assert(n->meta == BLACK || n->left->meta == BLACK);
        }
        b = br;
        if(n == this->root && br != bl) {
            //dump_subtree(this->root);
        }
        assert(n != this->root || br == bl);
        b = br;
        if(n->meta == BLACK) b += 1;
        if(n == this->root) assert(n->meta == BLACK);

        return b;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    void balance(Node_t<keyT, valT>* n) {
    }

    void flip(Node_t<keyT, valT>* n) {
        // Flip the color of this node and all children
        if(n){
            if(n != this->root){
                if(n->meta == 0){
                    n->meta = 1;
                }
                else{
                    n->meta = 0;
                }
            }
            else{
                n->meta = 0;
            }
        }
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
        bool already_exist = Tree_t<keyT, valT>::search(key).valid;
        short meta;
        if(already_exist){
            meta = search_subtree(this->root, key)->meta;
        }
        Node_t<keyT, valT>* A_node = Tree_t<keyT, valT>::insert_internal(key, value);
        if(already_exist){
            A_node->meta = meta;
        }
        else if(A_node == this->root){   // if root
            A_node->meta = 0;
        }
        else if(A_node->parent->parent && A_node->parent->meta == 1){    // if it has gp
            A_node->meta = 1;
            insert_RB(A_node);
        }
        else{
            A_node->meta = 1;
        }
    }

    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        Node_t<keyT, valT>* A_node = search_subtree(this->root, key);
        Node_t<keyT, valT>* client = bring(A_node);
        // short meta = A_node->meta;
        bool rt = Tree_t<keyT, valT>::remove(key);
        if(rt){
            if(A_node == this->root){
                flip(A_node);
                return rt;
            }
            else if(A_node == client){
                if(A_node->parent){
                    std::cout << "asdad"<<std::endl;
                    start(client);
                }
                return rt;
            }
            else if(client->meta == 1){
                return rt;
            }
            else if(client->right){
                if(client->right->meta == 1){
                    flip(client->right);
                }
                else{
                    if(client->parent){
                    std::cout << "asdad"<<std::endl;
                        start(client);
                        return rt;
                    }                     
                }
            }
            return rt;
        }
        return rt;
    }
    Node_t<keyT, valT>* bring(Node_t<keyT, valT>* n){
        if(n->left && n->right) {
            return Tree_t<keyT, valT>::get_min(n->right);
        } else if (n->left) {
            return n->left;
        } else if (n->right) {
            return going_right(n->right);
        } else {
            return n;
        }
    }
    Node_t<keyT, valT>* going_right(Node_t<keyT, valT>* n){
        if(n->right){
            return Tree_t<keyT, valT>::get_min(n->right);
        }
        else{
            return n;
        }
    }

    void start(Node_t<keyT, valT>* n) {
        if(n->parent){
            if(n->parent->meta == 0){
                if(n->parent->right){
                    if(n->parent->right->meta == 1){
                        case_1(n);
                    }
                }
                else{
                    if(n->parent->right->right){
                        if(n->parent->right->right->meta == 1){
                            case_4(n);
                        }
                        else{
                            case_0(n);
                        }
                    }
                    else if(n->parent->right->left){
                        if(n->parent->right->left->meta == 0){
                            case_3(n);
                        }
                        else{
                            case_0(n);
                        }
                    }
                    else{
                        case_0(n);
                    }
                }
            }
            else{
                if(n->parent->right){
                    if(n->parent->right->meta == 0){
                        if(n->parent->right->left && n->parent->right->right){
                            if(n->parent->right->left->meta == 1){
                                case_3(n);
                            }
                            else if(n->parent->right->right->meta == 1){
                                case_4(n);
                            }
                        }
                        else if(n->parent->right->left){
                            if(n->parent->right->left->meta == 1){
                                case_3(n);
                            }
                        }
                        else if(n->parent->right->right){
                            if(n->parent->right->right->meta == 1){
                                case_4(n);
                            }
                            else{
                                case_3(n);
                            }
                        }
                    }
                    else{
                        case_1(n);
                    }
                }
            }
        }
    }
    void case_0(Node_t<keyT, valT>* n) { //  Case 0: v and its two children are black
        if(n->parent->right){
            n->parent->right->meta =1;
        }
        start(n);
    }
    void case_2(Node_t<keyT, valT>* n) { // Case 2: v and its two children are black
        if(n->parent){
            if(n->parent->meta == 1){
                flip(n->parent);
            }
            if(n->parent->right){
                flip(n->parent->right);
            }
        }
    }
    void case_4(Node_t<keyT, valT>* n) { // Case 4: v is black and its right child is red
        if(n->parent){
            flip(n->parent);
            flip(n->parent->right);
            Tree_t<keyT, valT>::rotate(n->parent, false);
        }
    }
    void case_1(Node_t<keyT, valT>* n) { // Case 1: y is black and v is red
        if(n->parent){
            if(n->parent->right){
                flip(n->parent);
                flip(n->parent->right);
                Tree_t<keyT, valT>::rotate(n, false);
            }
        }
        if(n->parent){
            if(n->parent->meta == 1) case_2(n);
            else if(n->parent->right){
                if(n->parent->right->right){
                    if(n->parent->right->right->meta == 1){
                        case_4(n);
                    }
                    else case_2(n);
                }
                else if(n->parent->right->left){
                    if(n->parent->right->left->meta == 1){
                        case_3(n);
                    }
                    else case_2(n);
                }
                else case_2(n);
            }
        }
    }

    void case_3(Node_t<keyT, valT>* n) { // Case 3: v and its right child are black while its left child is red
        if(n->parent){
            if(n->parent->right){
                if(n->parent->right->left){
                    flip(n->parent->right->left);
                    flip(n->parent->right);
                    Tree_t<keyT, valT>::rotate(n->parent->right, true);
                }
            }
        }
        case_4(n);
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

    void insert_RB(Node_t<keyT, valT>* n) {
        bool gp_right = false;
        bool pp_right = false;
        if(n->parent->right){
            if(n->parent->right->key == n->key){
                pp_right = true;
            }
        }
        if(n->parent->parent->right){
            if(n->parent->parent->right->key == n->parent->key){
                gp_right = true;
            }    
        }
        if(gp_right){
            if(!n->parent->parent->left){   // z = black
                if(pp_right){ //RRb
                    Tree_t<keyT, valT>::rotate(n->parent->parent, false);
                    flip(n->parent);
                    flip(n->parent->left);
                }
                else{ // RLb
                    Tree_t<keyT, valT>::rotate(n->parent, true);
                    Tree_t<keyT, valT>::rotate(n->parent, false); // Tree_t<keyT, valT>::rotate(n->parent->parent, false);
                    flip(n);
                    flip(n->left);
                }
            }
            else if(n->parent->parent->left->meta == 1){    // XYr
                flip(n->parent->parent->left);
                flip(n->parent);
                flip(n->parent->parent);
            }
            else{
                if(pp_right){ //RRb
                    Tree_t<keyT, valT>::rotate(n->parent->parent, false);
                    flip(n->parent);
                    flip(n->parent->left);
                }
                else{ // RLb
                    Tree_t<keyT, valT>::rotate(n->parent, true);
                    Tree_t<keyT, valT>::rotate(n->parent, false); // Tree_t<keyT, valT>::rotate(n->parent->parent, false);
                    flip(n);
                    flip(n->left);
                }
            }
        }
        else{
            if(!n->parent->parent->right){  // z = black
                if(pp_right){ //LRb_null
                    Tree_t<keyT, valT>::rotate(n->parent, false);
                    Tree_t<keyT, valT>::rotate(n->parent, true);
                    flip(n);
                    flip(n->right);
                }
                else{ // LLb_null
                    Tree_t<keyT, valT>::rotate(n->parent->parent, true);
                    flip(n->parent);
                    flip(n->parent->right);
                }
            }
            else if(n->parent->parent->right->meta == 1){   // XYr
                flip(n->parent->parent->right);
                flip(n->parent);
                flip(n->parent->parent);
            }
            else{
                if(pp_right){ //LRb
                    Tree_t<keyT, valT>::rotate(n->parent, false);
                    Tree_t<keyT, valT>::rotate(n->parent, true); // Tree_t<keyT, valT>::rotate(n->parent->parent, true);
                    flip(n);
                    flip(n->right);
                }
                else{ // LLb
                    Tree_t<keyT, valT>::rotate(n->parent->parent, true);
                    flip(n->parent);
                    flip(n->parent->right);
                }
            }
        }
    }

};
//  g++ -o rb RBtest.cpp -Wall -std=c++11 -O3
