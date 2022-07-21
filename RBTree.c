#include <stdio.h>
#include <stdlib.h>

typedef enum { red, black } node_color;
typedef enum { false, true } bool;

struct Node {
    char *key;
    struct Node *right_son;
    struct Node *left_son;
    struct Node *parent;
    node_color color;
    bool valid;
};

// compares two strings in alphabetic order: 1 if s1 comes after s2
bool is_string_greater(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] != 0 || s2[i] != 0) {
        if (s1[i] > s2[i])
            return true;
        else if (s1[i] < s2[i])
            return false;
        i += 1;
    }
    return true;
}

bool is_string_equal(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] != 0 || s2[i] != 0) {
        if (s1[i] != s2[i])
            return false;
        i += 1;
    }
    return true;
}

void swap_nodes_parent_son(struct Node *a, struct Node *b){
    struct Node *a_temp = malloc(sizeof(struct Node));
    struct Node *b_temp = malloc(sizeof(struct Node));
    *a_temp = *a;
    *b_temp = *b;

    //if b is the left son
    if(a -> left_son == b || a -> right_son == b){
        a -> parent = b_temp -> parent;
        b -> parent = a_temp -> parent;
        a -> left_son = b_temp -> left_son;
        a -> right_son = b_temp -> right_son;
        b -> left_son = a_temp -> left_son;
        b -> right_son = a_temp -> right_son;
    }
    else printf("error: trying to swap two nodes with swap_nodes_parent_son that are not parent and son");

    free(a_temp);
    free(b_temp);

}


bool has_right_son(struct Node *n){
    if(n -> right_son == NULL)
        return false;
    return true;
}

bool has_left_son(struct Node *n){
    if(n -> left_son == NULL)
        return false;
    return true;
}

bool is_leaf(struct Node n) {
    if (n.key[0] == '\0')
        return true;
    return false;
}

void create_leaf(struct Node *new_node) {
    new_node->key = "";
    new_node->right_son = NULL;
    new_node->left_son = NULL;
    new_node->parent = NULL;
    new_node->color = black;
    new_node->valid = false;
}

void left_rotate(struct Node **Root, struct Node* pivot){
    struct Node* b;
    //if the pivot is not the root
    if(pivot->parent != NULL){

        if(has_right_son(pivot)){
            b = pivot->right_son;
            //point 1
            b->parent = pivot->parent;

            //point 2
            pivot->parent->left_son = b;
            pivot->parent = b;
            //point 3
            if(has_left_son(b)) {
                pivot->right_son = b->left_son;
                //point 4
                b->left_son = pivot;
                //point 5
                pivot->right_son -> parent = pivot;
            }

        }

    }
    else{

        if(has_right_son(pivot)){
            b = pivot->right_son;
            //point 1
            b->parent = NULL;
            //point 2
            pivot -> parent = b;
            //point 3
            if(has_left_son(b)) {
                pivot->right_son = b->left_son;
                //point 4
                b->left_son = pivot;
                //point 5
                pivot->right_son -> parent = pivot;
            }
            else{
                b -> left_son = pivot;
                pivot->right_son = NULL;
            }

            *Root = b;

        }

    }
}

void right_rotate(struct Node **Root, struct Node* pivot){
    struct Node* a;
    //if the pivot is not the root
    if(pivot->parent != NULL){

        if(has_left_son(pivot)){
            a = pivot->left_son;
            //point 1
            a->parent = pivot->parent;

            //point 2
            pivot->parent->left_son = a;
            pivot->parent = a;
            //point 3
            if(has_right_son(a)) {
                pivot->left_son = a->right_son;
                //point 4
                a->right_son = pivot;
                //point 5
                pivot->left_son -> parent = pivot;
            }

        }

    }
    //here the pivot is the Root, that means the Root has to change
    else{

        if(has_left_son(pivot)){
            a = pivot->left_son;
            //point 1
            a->parent = NULL;
            //point 2
            pivot -> parent = a;
            //point 3
            if(has_right_son(a)) {
                pivot->left_son = a->right_son;
                //point 4
                a->right_son = pivot;
                //point 5
                pivot->left_son -> parent = pivot;
            }
            else{
                a -> right_son = pivot;
                pivot->left_son = NULL;
            }

            *Root = a;
            printf("hello");
        }

    }
}


//Used to maintain a RB tree
void RB_insert_fixup(struct Node **Root, struct Node *new_node_ptr) {
    struct Node *node_parent;
    struct Node *node_uncle;

    //if the tree was empty before
    if ((*Root)->key == new_node_ptr->key) {
        (*Root)->color = black;
    } else {
        new_node_ptr->color = red;
        node_parent = new_node_ptr->parent;
        //if the parent is red that means that he also has a parent
        if (node_parent->color == red) {
            //if grandparent has a left son
            if ((node_parent->parent)->left_son != NULL) {
                //if parent is the left son of the grandfather
                if (((node_parent->parent)->left_son)->key == node_parent->key) {
                    //initialization
                    bool uncle_exists = false;

                    if (( node_parent->parent)->right_son != NULL) {
                        uncle_exists = true;
                        node_uncle = (node_parent->parent)->right_son;
                    }

                    if (uncle_exists) {
                        //if the uncle is red, CASE 1
                        if (node_uncle->color == red) {
                            node_parent->color = black;
                            node_uncle->color = black;
                            //if the grandparent is not the root
                            if (!is_string_equal((node_parent->parent)->key, (*Root)->key)) {
                                (node_parent->parent)->color = red;
                                //repeat on the grandparent
                                RB_insert_fixup(Root, node_parent->parent);
                            }
                        }
                    }
                    //if the node is the right son, CASE 2
                    else if ((node_parent->right_son != NULL)) {
                        if((node_parent->right_son)->key == new_node_ptr->key)
                            left_rotate(Root, node_parent);
                    }
                    //CASE 3
                    else {
                        node_parent->color = black;
                        (node_parent->parent)->color = red;
                        right_rotate(Root, node_parent->parent);
                    }
                }
            }
            //if grandparent has a right son
            else if ((node_parent->parent)->right_son != NULL) {
                //if parent is the right son of the grandfather
                if (((node_parent->parent)->right_son)->key == node_parent->key) {


                    //initialization
                    bool uncle_exists = false;

                    if (( node_parent->parent)->left_son != NULL) {
                        uncle_exists = true;
                        node_uncle = (node_parent->parent)->left_son;
                    }
                    if(uncle_exists && node_uncle->color == red) {
                        //if the uncle is red, CASE 1
                        node_parent->color = black;
                        node_uncle->color = black;
                        if (!is_string_equal(node_parent->key, (*Root)->key)) {
                            (node_parent->parent)->color = red;
                            //repeat on the grandparent
                            RB_insert_fixup(Root, node_parent->parent);
                        }
                    }

                    //if the node is the left son, CASE 2
                    else if ((node_parent->left_son)->key == new_node_ptr->key) {
                        right_rotate(Root, node_parent);
                    }
                        //CASE 3
                    else {
                        node_parent->color = black;
                        (node_parent->parent)->color = red;
                        right_rotate(Root, node_parent->parent);
                    }
                }

            }
        }
    }
}

// Visit the tree
void inorder_tree_walk(struct Node *Root) {
    if(Root != NULL) {
        if (Root->key != NULL) {
            inorder_tree_walk((*Root).left_son);
            printf("%s\n", (*Root).key);
            inorder_tree_walk((*Root).right_son);
        }
    }
}

// Given the Tree and the new node, insert the node in the Tree
void RB_insert(struct Node **Root, struct Node* node_ptr) {

    // variable used to go through nodes in RB tree
    struct Node *iterate = *Root;

    if (is_leaf(**Root)) {
        **Root = *node_ptr;
        return;
    }

    // find the right place to insert the word
    while (1) {

        if (is_string_greater(node_ptr -> key, iterate -> key)) {
            if (iterate -> right_son == NULL)
                break;
            iterate = iterate -> right_son;
        } else {
            if (iterate -> left_son == NULL)
                break;
            iterate = iterate->left_son;
        }
    }

    // insert the word in Tree
    node_ptr -> parent = iterate;

    // insert right if larger
    if (is_string_greater(node_ptr -> key, iterate -> key))
        iterate -> right_son = node_ptr;

    // insert right if larger
    else
        iterate -> left_son = node_ptr;

    // balance the tree
    RB_insert_fixup(Root, node_ptr);
}


int main() {

    //the tree is empty
    struct Node* ROOT = (struct Node*)malloc(sizeof(struct Node));
    create_leaf(ROOT);

    char *a[10] = {"ote", "ere", "drr", "bgx", "zud", "rad", "ulm", "not", "dpr", "ask"};

    for (int i = 0; i < 10; i++) {
        struct Node *new_node_ptr = (struct Node*)malloc(sizeof(struct Node));
        create_leaf(new_node_ptr);
        new_node_ptr -> key = a[i];
        RB_insert(&ROOT, new_node_ptr);
    }
    inorder_tree_walk(ROOT);
}
