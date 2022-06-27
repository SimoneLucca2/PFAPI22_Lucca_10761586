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

bool is_string_equal(const char *s1, const char *s2){
    int i = 0;
    //IT WORKS ONLY IF STRINGS HAVE THE SAME LENGTH
    while(s1[i] != 0 || s2[i] != 0){
        if(s1[i] != s2[i]) return false;
        i = i + 1;
    }
    return true;
}

// compares two strings in alphabetic order: 1 if s1 comes after s2
bool is_string_greater(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] != 0 || s2[i] != 0) {
        if (s1[i] > s2[i])
            return true;
        else if (s1[i] < s2[i])
            return false;
        i++;
    }
    return true;
}

bool is_leaf(struct Node n) {
    if (n.key[0] == '\0')
        return true;
    return false;
}

struct Node create_leaf() {
    struct Node new_node;
    new_node.key = "";
    new_node.right_son = NULL;
    new_node.left_son = NULL;
    new_node.parent = NULL;
    new_node.color = black;
    new_node.valid = false;

    return new_node;
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
// NOTE: Root has to point to a Leaf Node if the tree is empty
void RB_insert(struct Node *Root, struct Node* node_ptr) {

    // variable used to go through nodes in RB tree
    struct Node *iterate = Root;

    if (is_leaf(*Root)) {
        *Root = *node_ptr;
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
            iterate = (*iterate).left_son;
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
    // RB_insert_fixup(Root, new_node);
}


int main() {

    //the tree is empty
    struct Node* ROOT = (struct Node*)malloc(sizeof(struct Node));
    *ROOT = create_leaf();

    char *a[5] = {"are", "ere", "drr", "eix", "zud"};

    for (int i = 0; i < 5; i++) {
        struct Node *new_node_ptr = (struct Node*)malloc(sizeof(struct Node));
        *new_node_ptr = create_leaf();
        new_node_ptr -> key = a[i];
        RB_insert(ROOT, new_node_ptr);
    }
    inorder_tree_walk(ROOT);
}
