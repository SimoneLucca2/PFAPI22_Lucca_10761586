#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <time.h>

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

int n, k, legal_word_counter;
bool missing_letters[64];

int max(int n1, int n2){
    if(n1 > n2) return n1;
    else return n2;
}

bool is_string_valid(struct Node *Root, char *word){
    struct Node *iterate = Root;
    while(iterate != NULL && strcmp(iterate -> key, word) != 0){
        if(strcmp(word, iterate->key) > 0){
            iterate = iterate -> right_son;
        }
        else{
            iterate = iterate -> left_son;
        }
    }
    if(iterate == NULL || iterate -> valid == false) return false;
    return true;
}


//look for the string in the tree
bool is_string_present(struct Node *Root, char *word){
    struct Node *iterate = Root;
    while(iterate != NULL && strcmp(iterate -> key, word) != 0){
        if(strcmp(word, iterate->key) > 0){
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

void create_leaf(struct Node *new_node) {
    new_node->key = "";
    new_node->right_son = NULL;
    new_node->left_son = NULL;
    new_node->parent = NULL;
    new_node->color = black;
    new_node->valid = true;
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
            if(has_right_son(pivot -> parent) && strcmp(pivot -> key, pivot->parent->right_son->key) == 0) {
                pivot->parent->right_son = b;
            }
                //if pivot is the left son
            else if(pivot->parent->left_son != NULL && strcmp(pivot -> key, pivot->parent->left_son->key) == 0) {
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
            if(has_left_son(pivot -> parent) && strcmp(pivot -> key, pivot->parent->left_son->key) == 0) {
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
                        if (strcmp((node_parent->parent)->key, (*Root)->key) != 0) {
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
                    if (strcmp(node_parent->key, (*Root)->key) != 0) {
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

void stampa_filtrate(struct Node *Root){
    if(Root != NULL) {
        stampa_filtrate(Root->left_son);
        if(Root -> valid == true){
            printf("%s\n", Root -> key);
        }
        stampa_filtrate(Root->right_son);
    }
}

// Visit the tree
void inorder_tree_walk_counter(struct Node *Root) {
    if(Root != NULL) {
        inorder_tree_walk_counter(Root->left_son);
        if(Root -> valid == true){
            legal_word_counter += 1;
        }
        inorder_tree_walk_counter(Root->right_son);
    }
}

int hash_function_c(char c){
    if(c == '-')
        return 0;
    if('0' <= c && c <= '9')
        return (int)c - (int)'0' + 1;
    else if('A' <= c && c <= 'Z')
        return (int)c - (int)'A' + (int)'9' - (int)'0' + 2;
    else if('a' <= c && c <= 'z')
        return (int)c - (int)'a' + 37;
    else if(c == '_')
        return 63;
    else return -1;
}

void reset_valid(struct Node *Root){
    if(Root != NULL) {
        reset_valid(Root->left_son);
        Root -> valid = true;
        reset_valid(Root->right_son);
    }
}

//inorder tree walk to apply the + constraint
void ITW_constraints_plus(struct Node *Root, const char c, int i){
    if(Root != NULL) {
        ITW_constraints_plus(Root->left_son, c, i);
        if(Root -> valid == true) {
            if (c != (Root->key)[i]) {
                Root->valid = false;
            }
        }
        ITW_constraints_plus(Root->right_son, c, i);
    }
}

void ITW_constraints_bar(struct Node *Root, char c, int i){
    if(Root != NULL) {
        ITW_constraints_bar(Root->left_son, c,i);
        if(Root->valid == true) {
            if((Root -> key)[i] == c){
                Root -> valid = false;
            }
        }
        ITW_constraints_bar(Root->right_son, c, i);
    }
}

void ITW_constraints_slash(struct Node *Root, char c, int i){
    if(Root != NULL) {
        ITW_constraints_slash(Root->left_son, c, i);
        if (Root->valid == true && (Root -> key)[i] == c) {
            Root -> valid = false;
        }
        ITW_constraints_slash(Root->right_son, c, i);
    }
}

void ITW_letter_counter(struct Node *Root, char c, int missing, int present, const char *result){
    if(Root != NULL){
        ITW_letter_counter(Root -> left_son, c, missing, present, result);
        if(Root -> valid == true) {
            int count = 0;

            for (int i = 0; i < k; i++) {
                if ((Root->key)[i] == c && result[i] != '+') {
                    count++;
                }
            }

            if (missing == 0) {
                if (count < present) {
                    Root->valid = false;
                }
            }
            else{
                if(count != present){
                    Root -> valid = false;
                }
            }
        }
        ITW_letter_counter(Root -> right_son, c, missing, present, result);
    }
}

void ITW_constraints(struct Node *Root, int *C_hash_table_missing, int *C_hash_table_present, const char *result){
    if(Root != NULL) {
        ITW_constraints(Root->left_son, C_hash_table_missing, C_hash_table_present, result);
        if((Root -> valid) == true) {

            for(int i = 0; i < k; i++){
                if(result[i] != '+') {
                    int missing = C_hash_table_missing[hash_function_c((Root->key)[i])];
                    int present = C_hash_table_present[hash_function_c((Root->key)[i])];

                    if (missing > 0 && present == 0) {
                        Root->valid = false;
                    }
                }
            }
        }
        ITW_constraints(Root->right_son, C_hash_table_missing, C_hash_table_present, result);
    }
}

// Given the Tree and the new node, insert the node in the Tree
void RB_insert(struct Node **Root, struct Node* node_ptr) {

    // variable used to go through nodes in RB tree
    struct Node *iterate = *Root;

    if ((**Root).key[0] == '\0') {
        **Root = *node_ptr;
        return;
    }

    // find the right place to insert the word
    while (1) {
        if (strcmp(node_ptr -> key, iterate -> key) > 0) {
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
    if (strcmp(node_ptr -> key, iterate -> key) > 0)
        iterate -> right_son = node_ptr;

    // insert left if smaller
    else
        iterate -> left_son = node_ptr;

    // balance the tree
    RB_insert_fixup(Root, node_ptr);

}

//turns off the nodes that do not respect the constraints
void apply_constraints(struct Node *Root, const char *word, const char *result){
    int C_hash_table_missing[64];
    int C_hash_table_present[64];


    for(int i = 0; i < 64; i++) {
        C_hash_table_missing[i] = 0;
        C_hash_table_present[i] = 0;
    }

    for(int i = k - 1; i >= 0; i--){
        if(result[i] == '+'){
            ITW_constraints_plus(Root, word[i], i);
        }
        else if(result[i] == '|'){
            C_hash_table_present[hash_function_c(word[i])] += 1;
            missing_letters[hash_function_c(word[i])] = false;
            ITW_constraints_bar(Root, word[i], i);
        }
        else if(result[i] == '/'){
            C_hash_table_missing[hash_function_c(word[i])] += 1;
            missing_letters[hash_function_c(word[i])] = true;
            ITW_constraints_slash(Root, word[i], i);
        }
    }

    for(int i = 0; i < k; i++){
        int missing = C_hash_table_missing[hash_function_c(word[i])];
        int present =  C_hash_table_present[hash_function_c(word[i])];
        if(result[i] == '|'){
            ITW_letter_counter(Root, word[i], missing, present, result);
        }
        ITW_constraints(Root, C_hash_table_missing, C_hash_table_present, result);
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
    struct R_Node *iterate = &hash[hash_function(hash_dimension, c)];
    while(iterate -> key != c) {
        iterate = iterate -> next;
    }
    return iterate;
}


void inserisci_inizio(struct Node **Root1, struct Node **Root2, char *last_test, char* last_result, bool need_constraints){

    //take a random valid word in the tree, it contains already most information about the selection

    if(Root2 == NULL) {
        while (1) {
            char *word = malloc(sizeof(char) * max(k, 17));
            //read the word
            if (scanf("%s", word) != 1) return;
            //end the function if true
            if (strcmp(word, "+inserisci_fine") == 0) return;

            struct Node *new_node = malloc(sizeof(struct Node));
            create_leaf(new_node);

            new_node->key = word;

            if(need_constraints) {
                apply_constraints(new_node, last_test, last_result);

                for (int i = 0; i < k; i++) {
                    if (missing_letters[hash_function_c(word[i])] == true) {
                        new_node->valid = false;
                    }
                }
            }

            RB_insert(Root1, new_node);
        }
    }
    else{
        while (1) {
            char *word = malloc(sizeof(char) * max(k, 17));
            //read the word
            if (scanf("%s", word) != 1) return;
            //end the function if true
            if (strcmp(word, "+inserisci_fine") == 0) return;

            struct Node *new_node1 = malloc(sizeof(struct Node));
            struct Node *new_node2 = malloc(sizeof(struct Node));

            create_leaf(new_node1);
            create_leaf(new_node2);

            new_node1->key = word;
            new_node2->key = word;

            if(need_constraints) {
                apply_constraints(new_node1, last_test, last_result);
                apply_constraints(new_node2, last_test, last_result);


                for (int i = 0; i < k; i++) {
                    if (missing_letters[hash_function_c(word[i])] == true) {
                        new_node1->valid = false;
                        new_node2->valid = false;
                    }
                }
            }

            RB_insert(Root1, new_node1);
            RB_insert(Root2, new_node2);
        }
    }
}

void build_selection_tree(struct Node *Root, struct Node **New_Tree){
    if(Root != NULL) {
        if (Root->valid == true) {
            struct Node *new_node = malloc(sizeof(struct Node));
            create_leaf(new_node);
            new_node -> key = Root -> key;
            RB_insert(New_Tree, new_node);
        }
        build_selection_tree(Root->left_son, New_Tree);
        build_selection_tree(Root->right_son, New_Tree);
    }
}

void tree_destroy(struct Node *Root){
    if(Root != NULL){
        tree_destroy(Root -> left_son);
        tree_destroy(Root -> right_son);
        free(Root);
    }
}

//Print the output of the word
void word_result(const char* reference_word, struct R_Node* hash, int hash_dimension, char *word, char* result){
    struct R_Node hash_copy[hash_dimension];

    //initialize result and copy the hash table
    for(int i=0; i<hash_dimension;i++){
        struct R_Node *iterate_hash = &hash[i];
        struct R_Node *iterate_copy = &hash_copy[i];

        while(iterate_hash != NULL) {
            *iterate_copy = *iterate_hash;
            iterate_copy -> next = NULL;
            iterate_hash = iterate_hash -> next;
            if(iterate_hash != NULL){
                iterate_copy -> next = malloc(sizeof(struct R_Node));
                iterate_copy = iterate_copy -> next;
            }
        }
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

}


int main() {

    //time_t now1 = time(NULL);

    //reading and storing part
    //read k
    if(scanf("%d", &k) != 1) return 0;

    char last_test[k];
    char last_result[k];

    for(int i = 0; i < k; i++){
        last_test[i] = '$';
        last_result[i] = '$';
    }


    //read and store words

    //the tree is empty
    struct Node* ROOT = (struct Node*)malloc(sizeof(struct Node));
    create_leaf(ROOT);

    //IT IS ASSUMED THAT THE INSERTED WORDS ARE VALID
    while(1){
        char *word1 = malloc(sizeof(char) * max(k, 17));
        char *word = malloc(sizeof(char) * k);

        if(scanf("%s", word1) != 1) return 0;

        //start the game if true
        if(strcmp(word1, "+nuova_partita") == 0) break;

        strcpy(word, word1);
        free(word1);

        struct Node *new_node_ptr = (struct Node*)malloc(sizeof(struct Node));
        create_leaf(new_node_ptr);

        new_node_ptr -> key = word;
        RB_insert(&ROOT, new_node_ptr);

    }

    //time_t then1 = time(NULL);
    //printf("-------------------%ld-------------", then1 - now1);
    //game part


    while(1) {


        //this tree is used when the words are excluded
        struct Node* Selection_TREE = (struct Node*)malloc(sizeof(struct Node));
        create_leaf(Selection_TREE);

        //read reference word
        char *R_word = malloc(sizeof(char) * k);
        if (scanf("%s", R_word) != 1) return 0;

        //create the structure that contains reference word information
        int hash_dimension = k + 1;
        struct R_Node R_Hash_table[hash_dimension];

        //initialize
        for (int j = 0; j < hash_dimension; j++) {
            R_Hash_table[j].key = '$';
            R_Hash_table[j].count = 0;
            R_Hash_table[j].next = NULL;
        }

        //build the hash table that contains information about the reference word
        int i = 0;
        while (R_word[i] != 0) {
            int current_index = hash_function(hash_dimension, R_word[i]);
            //pointer to the current node
            struct R_Node *iterate = &R_Hash_table[current_index];

            if (iterate->key == '$') {
                iterate->key = R_word[i];
                iterate->count += 1;
            } else if (iterate->key == R_word[i]) {
                iterate->count += 1;
            } else {
                while (iterate->next != NULL && iterate->key != R_word[i]) {
                    iterate = iterate->next;
                }
                if (iterate->key == R_word[i]) {
                    iterate->count += 1;
                } else {
                    iterate->next = malloc(sizeof(struct R_Node));
                    iterate->next->key = R_word[i];
                    iterate->next->count = 1;
                    iterate->next->next = NULL;
                }
            }

            i += 1;
        }

        //read n
        if (scanf("%d", &n) != 1) return 0;

        //read word p - note exceptions +inserisci_inizio +stampa_filtrate
        i = 0;
        bool found = true;
        while (i < n) {

            char *word1 = malloc(sizeof(char) * max(k, 17));
            char result[k];
            if (scanf("%s", word1) != 1) return 0;

            if (i == 0) {

                //exception +inserisci_inizio
                if (strcmp(word1, "+inserisci_inizio") == 0) {
                    inserisci_inizio(&ROOT, NULL, last_test, last_result, true);
                    continue;
                }
                //exception +stampa_filtrate
                else if (strcmp(word1, "+stampa_filtrate") == 0) {
                    stampa_filtrate(ROOT);
                    continue;
                } else {

                    char *word = malloc(sizeof(char)*k);
                    strcpy(word, word1);
                    free(word1);

                    if (is_string_present(ROOT, word)) {

                        //word result returns true if the word was found
                        word_result(R_word, R_Hash_table, hash_dimension, word, result);

                        if(is_string_valid(Selection_TREE, word)) {
                            strcpy(last_test,word);
                            strcpy(last_result, result);
                        }

                        found = true;
                        for(int j = 0; j < k; j++){
                            if(result[j] != '+') found = false;
                        }
                        if(!found) {

                            //print the result
                            for(int j = 0; j < k; j++) {
                                printf("%c", result[j]);
                            }

                            apply_constraints(ROOT, word, result);

                            legal_word_counter = 0;
                            inorder_tree_walk_counter(ROOT);

                            printf("\n%d", legal_word_counter);
                        }else{
                            printf("ok");
                        }
                        printf("\n");

                        if (found) break;
                    } else {
                        printf("not_exists\n");
                        continue;
                    }
                }

                build_selection_tree(ROOT, &Selection_TREE);
            }
            else {

                //exception +inserisci_inizio
                if (strcmp(word1, "+inserisci_inizio") == 0) {
                    inserisci_inizio(&ROOT, &Selection_TREE, last_test, last_result, true);
                    continue;
                }
                //exception +stampa_filtrate
                if (strcmp(word1, "+stampa_filtrate") == 0) {

                    stampa_filtrate(Selection_TREE);



                    continue;
                } else {

                    char *word = malloc(sizeof(char)*k);
                    strcpy(word, word1);
                    free(word1);

                    if (is_string_present(ROOT, word)) {
                        if(!is_string_present(Selection_TREE, word)){
                            struct Node *new_node = malloc(sizeof(struct Node));
                            create_leaf(new_node);
                            new_node -> key = word;
                            RB_insert(&Selection_TREE, new_node);
                        }
                        //word result returns true if the word was found

                        word_result(R_word, R_Hash_table, hash_dimension, word, result);

                        if(is_string_valid(Selection_TREE, word)) {
                            strcpy(last_test,word);
                            strcpy(last_result, result);
                        }

                        found = true;
                        for(int j = 0; j < k; j++){
                            if(result[j] != '+') found = false;
                        }
                        if(!found) {

                            //print the result
                            for(int j = 0; j < k; j++) {
                                printf("%c", result[j]);
                            }

                            apply_constraints(Selection_TREE, word, result);

                            legal_word_counter = 0;
                            inorder_tree_walk_counter(Selection_TREE);

                            printf("\n%d", legal_word_counter);
                        }else{
                            printf("ok");
                        }
                        printf("\n");

                        if (found) break;

                    } else {
                        printf("not_exists\n");
                        continue;
                    }
                }
            }
            i += 1;
        }
        //print ko
        if (!found) {
            printf("ko\n");
        }


        while(1){
            char *game_starter = malloc(sizeof(char) * 17);
            if (scanf("%s", game_starter) != 1) return 0;
            if(strcmp(game_starter, "+inserisci_inizio") == 0){
                inserisci_inizio(&ROOT, NULL, last_test, last_result, false);
            }
            else if(strcmp(game_starter, "+nuova_partita") == 0) {
                reset_valid(ROOT);
                tree_destroy(Selection_TREE);

                for(int j = 0; j < 64; j++) {
                    missing_letters[j] = false;
                }

                break;
            }
            else{
                //time_t then2 = time(NULL);
                //printf("-------------------%ld-------------", then2 - now1);
                return 0;
            }
        }


    }
}

