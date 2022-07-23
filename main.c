#include <stdlib.h>
#include <stdio.h>


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

//Reference node, contains information about a letter in the reference word
struct R_Node{
    char key;
    int count;
    struct R_Node *next;
};


int n, k;

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

//look for the string in the tree
bool is_string_valid(struct Node *Root, char *word){
    struct Node *iterate = Root;
    while(iterate != NULL && !is_string_equal(iterate -> key, word)){
        if(is_string_greater(word, iterate->key)){
            iterate = iterate -> right_son;
        }
        else{
            iterate = iterate -> left_son;
        }
    }
    if(iterate == NULL) return false;
    return true;
}

bool has_right_son(struct Node *node){
    if(node -> right_son == NULL)
        return false;
    return true;
}

bool has_left_son(struct Node *node){
    if(node -> left_son == NULL)
        return false;
    return true;
}

bool is_leaf(struct Node node) {
    if (node.key[0] == '\0')
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

            //if pivot is the right son
            if(has_right_son(pivot -> parent) && is_string_equal(pivot -> key, pivot->parent->right_son->key)) {
                pivot->parent->right_son = b;
            }
                //if pivot is the left son
            else if(pivot->parent->left_son != NULL && is_string_equal(pivot -> key, pivot->parent->left_son->key)) {
                pivot->parent->left_son = b;
            }

            //point 3
            if(has_left_son(b)) {
                pivot->right_son = b->left_son;
                //point 4
                b->left_son = pivot;
                //point 5
                pivot->right_son -> parent = pivot;
            }
            else{
                b->left_son = pivot;
                pivot -> right_son = NULL;
            }
            pivot -> parent = b;


        }

    }
    else{

        if(has_right_son(pivot)){
            b = pivot->right_son;
            b->parent = NULL;
            if(has_left_son(b)) {
                pivot->right_son = b->left_son;
                b->left_son = pivot;
                pivot->right_son -> parent = pivot;
            }
            else{
                b -> left_son = pivot;
                pivot->right_son = NULL;
            }
            pivot -> parent = b;

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
            a->parent = pivot->parent;
            //if pivot is the left son
            if(has_left_son(pivot -> parent) && is_string_equal(pivot -> key, pivot->parent->left_son->key)) {
                pivot->parent->left_son = a;
            }

                //if pivot is the right son
            else{
                pivot->parent->right_son = a;
            }


            pivot->parent = a;
            if(has_right_son(a)) {
                pivot->left_son = a->right_son;
                a->right_son = pivot;
                pivot->left_son -> parent = pivot;
            }
            else{
                a->right_son = pivot;
                pivot -> left_son = NULL;
            }
            pivot -> parent = a;

        }

    }
        //here the pivot is the Root, that means the Root has to change
    else{

        if(has_left_son(pivot)){
            a = pivot->left_son;
            a->parent = NULL;

            if(has_right_son(a)) {
                pivot->left_son = a->right_son;
                a->right_son = pivot;
                pivot->left_son -> parent = pivot;
            }
            else{
                a -> right_son = pivot;
                pivot->left_son = NULL;
            }
            pivot -> parent = a;

            *Root = a;
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

            //if parent is the left son of the grandfather
            if ((node_parent->parent)->left_son != NULL && ((node_parent->parent)->left_son)->key == node_parent->key) {

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
                    //NOTE, here the uncle can exist black or not exist
                    //if the node is the right son, CASE 2
                else if ((node_parent->right_son != NULL) && (node_parent->right_son)->key == new_node_ptr->key) {
                    left_rotate(Root, node_parent);
                    RB_insert_fixup(Root, node_parent);
                }
                    //CASE 3
                else {
                    node_parent->color = black;
                    (node_parent->parent)->color = red;
                    right_rotate(Root, node_parent -> parent);
                }

            }

                //if parent is the right son of the grandfather
            else if ((node_parent->parent)->right_son != NULL && ((node_parent->parent)->right_son)->key == node_parent->key) {

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
                else if(node_parent->left_son != NULL && (node_parent->left_son)->key == new_node_ptr->key) {
                    right_rotate(Root, node_parent);
                    RB_insert_fixup(Root, node_parent);
                }

                    //CASE 3
                else {
                    node_parent->color = black;
                    (node_parent->parent)->color = red;
                    left_rotate(Root, node_parent -> parent);
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

void inserisci_inizio(struct Node **Root){
    while(1) {
        char *word = malloc(sizeof(char)*k);
        //read the word
        if(scanf("%s", word) != 1) return;
        //end the function if true
        if (is_string_equal(word, "+inserisci_fine")) return;

        struct Node *new_node = malloc(sizeof(struct Node));
        create_leaf(new_node);
        new_node -> key = word;

        RB_insert(Root, new_node);
    }

}

int hash_function(int dim, char c){
    return (int)c % dim;
}

bool is_in_hash_table(struct R_Node *hash, int hash_dimension, char c){
    struct R_Node *iterate = &hash[hash_function(hash_dimension, c)];

    while( iterate -> key != c && iterate -> next != NULL){
        iterate = iterate -> next;
    }
    if(iterate -> key == c) {
        return true;
    }
    else{
        return false;
    }
}

struct R_Node *get_hash_node(struct R_Node *hash, int hash_dimension, char c) {
    return &hash[hash_function(hash_dimension, c)];
}

//Print the output of the word
bool word_result(const char* reference_word, const struct R_Node* hash, int hash_dimension, char *word){
    char result[k];
    struct R_Node hash_copy[hash_dimension];

    //initialize result and copy the hash table
    for(int i=0; i<hash_dimension;i++){
        hash_copy[i] = hash[i];
        if(i<k)
            result[i] = '$';
    }

    //Fill first the letters that are not in reference_word
    //and the ones that are in the right place
    for(int i = 0; i < k; i++){
        if(!is_in_hash_table(hash_copy, hash_dimension, word[i])){
            result[i] = '/';
        }else if(word[i] == reference_word[i]){
            result[i] = '+';
            struct R_Node *current_node;
            current_node = get_hash_node(hash_copy, hash_dimension, word[i]);
            current_node -> count -= 1;
        }
    }

    struct R_Node *current_node;

    for(int i = 0; i < k; i++){
        if(result[i] == '$'){
            current_node = get_hash_node(hash_copy, hash_dimension,word[i]);
            if(current_node -> count > 0){
                result[i] = '|';
                current_node -> count -= 1;
            }else{
                result[i] = '/';
            }
        }
    }
    
    bool found = true;
    
    for(int i = 0; i < k; i++){
        if(result[i] != '+') found = false;
    }
    if(!found) {
        //print the result
        for (int i = 0; i < k; i++) {
            printf("%c", result[i]);
        }
    }else{
        printf("ok");
    }
    printf("\n");
    return found;

}

int main() {
    //reading and storing part
    //read k
    if(scanf("%d", &k) != 1) return 0;

    //read and store words

    //the tree is empty
    struct Node* ROOT = (struct Node*)malloc(sizeof(struct Node));
    create_leaf(ROOT);

    //IT IS ASSUMED THAT THE INSERTED WORDS ARE VALID
    while(1){
        char* word = malloc(sizeof(char)*k);
        if(scanf("%s", word) != 1) return 0;

        //start the game if true
        if(is_string_equal(word, "+nuova_partita")) break;

        struct Node *new_node_ptr = (struct Node*)malloc(sizeof(struct Node));
        create_leaf(new_node_ptr);

        new_node_ptr -> key = word;

        RB_insert(&ROOT, new_node_ptr);

    }

    //game part

    //read reference word
    char *R_word = malloc(sizeof(char)*k);
    if(scanf("%s", R_word) != 1) return 0;

    //create the structure that contains reference word information
    int hash_dimension = (6 * k)/5;
    struct R_Node R_Hash_table[hash_dimension];

    //initialize
    for(int j=0; j<hash_dimension; j++) {
        R_Hash_table[j].key = '$';
        R_Hash_table[j].count = 0;
        R_Hash_table[j].next = NULL;
    }

    //build the hash table that contains information about the reference word
    int i = 0;
    while(R_word[i] != 0){
        int current_index = hash_function(hash_dimension, R_word[i]);
        //pointer to the current node
        struct R_Node *iterate = &R_Hash_table[current_index];

        if (iterate->key == '$') {
            iterate->key = R_word[i];
            iterate->count += 1;
        } else if (iterate->key == R_word[i]) {
            iterate->count += 1;
        }else{
            while(iterate -> next != NULL && iterate -> key != R_word[i]){
                iterate = iterate -> next;
            }
            if(iterate -> key == R_word[i]) {
                iterate -> count += 1;
            }else{
                iterate->next = malloc(sizeof(struct R_Node));
                iterate->next->key = R_word[i];
                iterate->next->count = 1;
                iterate->next->next = NULL;
            }
        }

        i+=1;
    }

    //read n
    if(scanf("%d", &n) != 1) return 0;

    //read word p - note exceptions +inserisci_inizio +stampa_filtrate
    bool found = false;
    i = 0;
    while(i < n){
        char *word = malloc(sizeof(char)*k);
        if(scanf("%s", word) != 1) return 0;

        //exception +inserisci_inizio
        if(is_string_equal(word, "+inserisci_inizio")){
            inserisci_inizio(&ROOT);
            i-=1;
        }
        //exception +stampa_filtrate
        else if(is_string_equal(word, "+stampa_filtrate")){
            //stampa_filtrate(ROOT);
            i-=1;
        }
        else{
            if(is_string_valid(ROOT, word)) {
                //word result returns true if the word was found
                found = word_result(R_word, R_Hash_table, hash_dimension, word);
                if(found) break;
            }else{
                printf("not_exists");
                i-=1;
            }
        }

        i+=1;
    }
    //print ko
    if(!found){
        printf("ko");
    }
    //apply constrains
    //count every legal word remaining

}

