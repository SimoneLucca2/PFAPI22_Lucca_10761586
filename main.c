#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

struct hashNode{
    struct Node *Tree;
    bool shadow_tree;
};

struct temporaryListNode{
    char *key;
    struct temporaryListNode *next;
};

struct iterationList{
    unsigned long index;
    struct iterationList *next;
};

//Reference node, contains information about a letter in the reference word
struct R_Node{
    char key;
    int count;
    struct R_Node *next;
};

int n, legal_word_counter;
unsigned char x, t;
int k;
unsigned long hash_dimension = 0;

//information that the user have about the number of letters in the reference word
int lett_count[64];
//information the user knows the exact number or only the minimum number
bool exact_num[64];

int max(int n1, int n2);
unsigned long hash_function(char* word);
void create_leaf(struct Node *new_node);
void RB_insert(struct hashNode *TreeBox, struct Node* node_ptr);
void RB_insert_fixup(struct hashNode *TreeBox, struct Node *new_node_ptr);
void right_rotate(struct hashNode *TreeBox, struct Node* pivot);
void left_rotate(struct hashNode *TreeBox, struct Node* pivot);
bool has_right_son(struct Node *node);
bool has_left_son(struct Node *node);
int lett_number(char c);
void append(unsigned long i, struct iterationList **Tail, struct iterationList **Head);
void stampa_filtrate(struct hashNode *HashTable, struct iterationList *Head);
void print_tree(struct Node *Tree);
bool is_word_in_tree(struct Node *Root, char *word);
bool is_string_present(struct hashNode *HashTable, char *word);
bool is_string_valid(struct hashNode *HashTable, char *word);
int R_hash_function(int dim, char c);
void word_result(const char* reference_word, struct R_Node* hash, int R_hash_dimension, char *word, char* result);
struct R_Node *get_hash_node(struct R_Node *hash, unsigned char hash_dimension, char c);
bool is_in_R_hash_table(struct R_Node *hash, int R_hash_dimension, char c);
void apply_constraints(struct hashNode *HashTable, struct iterationList **INodeHead, struct iterationList **INodeTail, char (*positionInformation)[64],char *word, char* result);
void validNodeCounter(struct hashNode *HashTable, struct iterationList **IListHead, struct iterationList **IListTail);
void countTreeNodes(struct Node *Root);
void nodeRemove(struct iterationList *nodeToBeDeleted, struct iterationList *prev, struct iterationList **Head, struct iterationList **Tail);
void TreeVisitPlusConstraint(struct Node *Root, char letter, int position);
void TreeVisitBarConstraint(struct Node *Root, char letter, int position);
void TreeVisitCounterConstraint(struct Node *Root, char letter, int position, bool isExact);
void TreeVisitSlashConstraint(struct Node *Root, char letter);
void reset_valid(struct hashNode *HashTable, struct iterationList **IListHead, struct iterationList **IListTail);
void reset_tree(struct Node *Root);
void destroyList(struct iterationList **IListHead, struct iterationList **IListTail);
void inserisciInizio(struct hashNode *HashTable, struct iterationList **IListHead, struct iterationList **IListTail, char (*positionInformation)[64], bool need_constraints);
void invalidateTree(struct Node *Root);
void listInsert(unsigned long index, struct iterationList **IListHead,struct iterationList **IListTail);

int max(int n1, int n2){
    if(n1 > n2) return n1;
    else return n2;
}

unsigned long hash_function(char* word){

    unsigned long index = 0;
    unsigned char i = 0;

    while(i < x){
        index += (unsigned long)(lett_number(word[i]) * hash_dimension / pow(64, i + 1));
        i++;
    }
    if(t > 0) {
        index = index + (unsigned long) floor(lett_number(word[x]) * pow(2, t)/64);
    }
    return index;
}

void create_leaf(struct Node *new_node) {
    new_node->key = "";
    new_node->right_son = NULL;
    new_node->left_son = NULL;
    new_node->parent = NULL;
    new_node->color = black;
    new_node->valid = true;
}

void RB_insert(struct hashNode *TreeBox, struct Node* node_ptr) {

    // variable used to go through nodes in RB tree
    struct Node *Root = TreeBox -> Tree;
    struct Node *iterate = Root;

    // find the right place to insert the word
    while(1) {
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
    RB_insert_fixup(TreeBox, node_ptr);

}

//Used to maintain a RB tree
void RB_insert_fixup(struct hashNode *TreeBox, struct Node *new_node_ptr) {
    struct Node *Root = TreeBox -> Tree;
    struct Node *node_parent;
    struct Node *node_uncle;

    if (Root -> key == new_node_ptr -> key) {
        Root -> color = black;
    }else {
        new_node_ptr->color = red;
        node_parent = new_node_ptr->parent;
        //if the parent is red that means that he also has a parent
        if (node_parent->color == red) {

            //if parent is the left son of the grandfather
            if ((node_parent->parent)->left_son != NULL && ((node_parent->parent)->left_son)->key == node_parent->key) {

                //initialization
                bool uncle_exists = false;

                if ((node_parent->parent)->right_son != NULL) {
                    uncle_exists = true;
                    node_uncle = (node_parent->parent)->right_son;
                }

                if (uncle_exists) {
                    //if the uncle is red, CASE 1
                    if (node_uncle->color == red) {

                        node_parent->color = black;
                        node_uncle->color = black;

                        //if the grandparent is not the root
                        if (strcmp((node_parent->parent)->key, Root->key) != 0) {
                            (node_parent->parent)->color = red;
                            //repeat on the grandparent
                            RB_insert_fixup(TreeBox, node_parent->parent);
                        }
                    }
                }
                    //NOTE, here the uncle can exist black or not exist
                    //if the node is the right son, CASE 2
                else if ((node_parent->right_son != NULL) && (node_parent->right_son)->key == new_node_ptr->key) {
                    left_rotate(TreeBox, node_parent);
                    RB_insert_fixup(TreeBox, node_parent);
                }
                    //CASE 3
                else {
                    node_parent->color = black;
                    (node_parent->parent)->color = red;
                    right_rotate(TreeBox, node_parent->parent);
                }

            }

                //if parent is the right son of the grandfather
            else if ((node_parent->parent)->right_son != NULL &&
                     ((node_parent->parent)->right_son)->key == node_parent->key) {

                //initialization
                bool uncle_exists = false;

                if ((node_parent->parent)->left_son != NULL) {
                    uncle_exists = true;
                    node_uncle = (node_parent->parent)->left_son;
                }
                if (uncle_exists && node_uncle->color == red) {
                    //if the uncle is red, CASE 1
                    node_parent->color = black;
                    node_uncle->color = black;
                    if (strcmp(node_parent->key, Root->key) != 0) {
                        (node_parent->parent)->color = red;
                        //repeat on the grandparent
                        RB_insert_fixup(TreeBox, node_parent->parent);
                    }
                }

                    //if the node is the left son, CASE 2
                else if (node_parent->left_son != NULL && (node_parent->left_son)->key == new_node_ptr->key) {
                    right_rotate(TreeBox, node_parent);
                    RB_insert_fixup(TreeBox, node_parent);
                }

                    //CASE 3
                else {
                    node_parent->color = black;
                    (node_parent->parent)->color = red;
                    left_rotate(TreeBox, node_parent->parent);
                }


            }

        }
    }
}

void right_rotate(struct hashNode *TreeBox, struct Node* pivot){
    struct Node* a;
    //if the pivot is not the root
    if(pivot != TreeBox -> Tree){

        if(has_left_son(pivot)){
            a = pivot->left_son;
            a->parent = pivot->parent;

            //if pivot is the left son
            if(has_left_son(pivot -> parent) && (pivot == pivot->parent->left_son)) {
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

            TreeBox -> Tree = a;
        }

    }
}

void left_rotate(struct hashNode *TreeBox, struct Node* pivot){
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

            TreeBox -> Tree = b;

        }

    }
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

int lett_number(char c){
    if(c == '-')
        return 0;
    else if('0' <= c && c <= '9')
        return (int)c - (int)'0' + 1;
    else if('A' <= c && c <= 'Z')
        return (int)c - (int)'A' + (int)'9' - (int)'0' + 2;
    else if('a' <= c && c <= 'z')
        return (int)c - (int)'a' + 37;
    else if(c == '_')
        return 63;
    else return -1;
}

void append(unsigned long i, struct iterationList **Tail, struct iterationList **Head){
    struct iterationList *new = malloc(sizeof(struct iterationList));
    new -> index = i;
    new -> next = NULL;
    if(*Tail == NULL){
        *Tail = new;
        *Head = new;
    }else{
        (*Tail) -> next = new;
        *Tail = new;
    }
}

void stampa_filtrate(struct hashNode *HashTable, struct iterationList *Head){
    struct iterationList *iterate = Head;
    while(iterate != NULL){
        print_tree(HashTable[iterate -> index].Tree);
        iterate = iterate -> next;
    }
}

void print_tree(struct Node *Tree){
    if(Tree != NULL) {
        print_tree(Tree->left_son);
        if (Tree->valid == true) {
            printf("%s\n", Tree->key);
        }
        print_tree(Tree->right_son);
    }
}

//look for the string in the tree
bool is_word_in_tree(struct Node *Root, char *word){
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

bool is_string_present(struct hashNode *HashTable, char *word){
    return is_word_in_tree(HashTable[hash_function(word)].Tree, word);
}

bool is_string_valid(struct hashNode *HashTable, char *word){
    if(HashTable[hash_function(word)].shadow_tree == true)
        return false;
    else{
        struct Node *iterate = HashTable[hash_function(word)].Tree;
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
}


int R_hash_function(int dim, char c){
    return (int)c % dim;
}

void word_result(const char* reference_word, struct R_Node* hash, int R_hash_dimension, char *word, char* result) {
    struct R_Node hash_copy[R_hash_dimension];

    //initialize result and copy the hash table
    for(int i=0; i<R_hash_dimension;i++){
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
        if(!is_in_R_hash_table(hash_copy, R_hash_dimension, word[i])){
            result[i] = '/';
        }else if(word[i] == reference_word[i]){
            result[i] = '+';
            struct R_Node *current_node;
            current_node = get_hash_node(hash_copy, R_hash_dimension, word[i]);
            current_node -> count -= 1;
        }
    }

    struct R_Node *current_node;

    for(int i = 0; i < k; i++){
        if(result[i] == '$'){
            current_node = get_hash_node(hash_copy, R_hash_dimension,word[i]);
            if(current_node -> count > 0){
                result[i] = '|';
                current_node -> count -= 1;
            }else{
                result[i] = '/';
            }
        }
    }
}

struct R_Node *get_hash_node(struct R_Node *hash, unsigned char R_hash_dimension, char c) {
    struct R_Node *iterate = &hash[R_hash_function(R_hash_dimension, c)];
    while(iterate -> key != c) {
        iterate = iterate -> next;
    }
    return iterate;
}

bool is_in_R_hash_table(struct R_Node *hash, int R_hash_dimension, char c){
    struct R_Node *iterate = &hash[R_hash_function(R_hash_dimension, c)];

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

void apply_constraints(struct hashNode *HashTable, struct iterationList **INodeHead, struct iterationList **INodeTail, char (*positionInformation)[64],char *word, char* result){

    int i;
    for(i = 0; i < x; i++) {

        char prev = positionInformation[i][lett_number(word[i])];

        if(result[i] == '+' && prev != '+'){

            //go through the IList and remove all the nodes that don't have word[i] in the i-th position
            struct iterationList *iterate = *INodeHead;
            struct iterationList *prevNode = NULL;

            while(iterate != NULL){
                if((HashTable[iterate -> index].Tree -> key)[i] != word[i]){
                    HashTable[iterate -> index].shadow_tree = true;
                    struct iterationList *next = iterate -> next;
                    nodeRemove(iterate, prevNode, INodeHead, INodeTail);
                    iterate = next;
                }else {
                    prevNode = iterate;
                    iterate = iterate->next;
                }
            }

            //set all the other char in positionInformation[i] as '/'
            for(int j = 0; j < 64; j++){
                positionInformation[i][j] = '/';
            }
            positionInformation[i][lett_number(word[i])] = '+';

        }else if(result[i] != '+' && prev == '$'){
            //go through the IList and remove all the nodes that have word[i] in the i-th position
            struct iterationList *iterate = *INodeHead;
            struct iterationList *prevNode = NULL;

            while(iterate != NULL){
                if((HashTable[iterate -> index].Tree -> key)[i] == word[i]){
                    HashTable[iterate -> index].shadow_tree = true;
                    struct iterationList *next = iterate -> next;
                    nodeRemove(iterate, prevNode, INodeHead, INodeTail);

                    iterate = next;
                }else {
                    prevNode = iterate;
                    iterate = iterate->next;
                }
            }

            positionInformation[i][lett_number(word[i])] = '/';
        }
    }

    if(i == k) return;

    i = x;
    char prev;

    if(t > 0) {
        prev = positionInformation[i][lett_number(word[i])];

        if (result[i] == '+' && prev != '+') {

            //set all the other char in positionInformation[i] as '/'
            for (int j = 0; j < 64; j++) {
                positionInformation[i][j] = '/';
            }
            positionInformation[i][lett_number(word[i])] = '+';

            //go through the IList and remove all the nodes that belong to other categories
            //if it is visiting the corresponding category go through the tree and invalidate the nodes
            //that don't have word[i] in the i-th place

            struct iterationList *iterate = *INodeHead;
            struct iterationList *prevNode = NULL;

            while (iterate != NULL) {
                //if they are not in the same place in the hash table

                short category = (short)((short)lett_number(word[i]) * (short) pow(2, t) / 64);
                if ((short)((iterate->index) % (short) pow(2, t)) != category) {
                    HashTable[iterate->index].shadow_tree = true;
                    struct iterationList *next = iterate->next;
                    nodeRemove(iterate, prevNode, INodeHead, INodeTail);
                    iterate = next;
                } else {
                    TreeVisitPlusConstraint(HashTable[iterate->index].Tree, word[i], i);
                    prevNode = iterate;
                    iterate = iterate->next;
                }
            }
        } else if (result[i] != '+' && prev == '$') {
            //in this case it is not possible to remove nodes from the IList for lack of information
            //it is still possible to save time by going only through the trees corresponding to the word[i] class
            struct iterationList *iterate = *INodeHead;

            short category = (short)((short)lett_number(word[i]) * (short) pow(2, t) / 64);
            while (iterate != NULL) {

                //if they are not in the same place in the hash table
                if ((short)((iterate->index) % (short) pow(2, t)) == category) {
                    TreeVisitBarConstraint(HashTable[iterate->index].Tree, word[i], i);
                }
                iterate = iterate->next;

            }

            positionInformation[i][lett_number(word[i])] = '/';

        }
        i++;
    }

    while(i < k){
        prev = positionInformation[i][lett_number(word[i])];
        if(result[i] == '+' && prev != '+'){

            //set all the other char in positionInformation[i] as '/'
            for(int j = 0; j < 64; j++){
                positionInformation[i][j] = '/';
            }
            positionInformation[i][lett_number(word[i])] = '+';

            //go through each valid Tree of the HashTable using the IList
            //invalid every word that doesn't have word[i] in the i-th place

            struct iterationList *iterate = *INodeHead;

            while(iterate != NULL){
                TreeVisitPlusConstraint(HashTable[iterate -> index].Tree, word[i], i);
                iterate = iterate -> next;
            }


        }else if(result[i] != '+' && prev == '$'){
            //set the matrix value as / in the corresponding place
            positionInformation[i][lett_number(word[i])] = '/';

            //go through each valid Tree of the HashTable using the IList
            //invalid every word that doesn't have word[i] in the i-th place

            struct iterationList *iterate = *INodeHead;

            while(iterate != NULL) {
                TreeVisitBarConstraint(HashTable[iterate->index].Tree, word[i], i);
                iterate = iterate -> next;
            }
        }
        i++;
    }

    //CONSTRAINTS REGARDING THE LETTER COUNT
    int temporaryCounter[64];
    for(int j = 0; j < 64; j++){
        temporaryCounter[j] = 0;
    }

    for(int j = 0; j < k; j++){
        if(result[j] == '|' || result[j] == '+'){
            temporaryCounter[lett_number(word[j])]++;
        }
    }

    bool prev_exact_num[64];
    for(short j = 0; j < 64; j++){
        prev_exact_num[j] = exact_num[j];
    }

    for(int j = 0; j < k; j++) {

        if (result[j] == '/' && temporaryCounter[lett_number(word[j])] > 0) {
            exact_num[lett_number(word[j])] = true;
        } else if (result[j] == '/' && temporaryCounter[lett_number(word[j])] == 0) {

            struct iterationList *iterate = *INodeHead;

            while (iterate != NULL) {
                TreeVisitSlashConstraint(HashTable[iterate->index].Tree, word[j]);
                iterate = iterate->next;
            }

        }
    }

    for(int j = 0; j < k; j++){
        int lett_num = lett_number(word[j]);
        if((result[j] == '|' || result[j] == '+') && (temporaryCounter[lett_num] > lett_count[lett_num] || exact_num[lett_num] != prev_exact_num[lett_num])){
            lett_count[lett_num] = temporaryCounter[lett_num];

            struct iterationList *iterate = *INodeHead;

            while(iterate != NULL){
                TreeVisitCounterConstraint(HashTable[iterate -> index].Tree, word[j], j, exact_num[lett_num]);
                iterate = iterate -> next;
            }

        }
    }

}

void validNodeCounter(struct hashNode *HashTable, struct iterationList **IListHead, struct iterationList **IListTail){
    struct iterationList *iterate = *IListHead;
    struct iterationList *prev = NULL;

    while(iterate != NULL){
        int temp = legal_word_counter;

        countTreeNodes(HashTable[iterate -> index].Tree);

        if(temp == legal_word_counter){
            struct iterationList *nextNode = iterate -> next;
            HashTable[iterate -> index].shadow_tree = true;
            nodeRemove(iterate, prev, IListHead, IListTail);
            iterate = nextNode;
            continue;
        }

        prev = iterate;
        iterate = iterate -> next;
    }
}

void countTreeNodes(struct Node *Root) {
    if (Root != NULL) {
        countTreeNodes(Root->left_son);
        if(Root -> valid == true){
            legal_word_counter++;
        }
        countTreeNodes(Root->right_son);
    }
}

void nodeRemove(struct iterationList *nodeToBeDeleted, struct iterationList *prev, struct iterationList **Head, struct iterationList **Tail){
    if(nodeToBeDeleted == *Head && nodeToBeDeleted == *Tail){
        free(nodeToBeDeleted);
        *Head = *Tail = NULL;
    }else if(nodeToBeDeleted == *Head){
        *Head = nodeToBeDeleted->next;
        free(nodeToBeDeleted);
    } else if(prev != NULL && nodeToBeDeleted == *Tail){
        *Tail = prev;
        prev -> next = NULL;
        free(nodeToBeDeleted);
    } else {
        prev->next = nodeToBeDeleted->next;
        free(nodeToBeDeleted);
    }
}

void TreeVisitPlusConstraint(struct Node *Root, char letter, int position){
    if(Root != NULL){
        TreeVisitPlusConstraint(Root -> left_son, letter, position);
        if(Root -> valid == true){
            if((Root -> key)[position] != letter){
                Root -> valid = false;
            }
        }
        TreeVisitPlusConstraint(Root -> right_son, letter, position);
    }
}

void TreeVisitBarConstraint(struct Node *Root, char letter, int position){
    if(Root != NULL){
        TreeVisitBarConstraint(Root -> left_son, letter, position);
        if(Root -> valid == true){
            if((Root -> key)[position] == letter){
                Root -> valid = false;
            }
        }
        TreeVisitBarConstraint(Root -> right_son, letter, position);
    }
}

void TreeVisitCounterConstraint(struct Node *Root, char letter, int position, bool isExact){
    if(Root != NULL){
        TreeVisitCounterConstraint(Root -> left_son, letter, position, isExact);
        if(Root -> valid == true){

            short count = 0;
            for(int i = 0; i < k; i++){
                if((Root -> key)[i] == letter){
                    count++;
                }
            }
            if(isExact){
                //if the node contains a different number of word[j] letter than lett_count[lett_num] => valid = false
                if(count != lett_count[lett_number(letter)]){
                    Root -> valid = false;
                }
            }else{
                //if the node contains less word[j] letter than lett_count[lett_num] => valid = false
                if(count < lett_count[lett_number(letter)]){
                    Root -> valid = false;
                }
            }
        }
        TreeVisitCounterConstraint(Root -> right_son, letter, position, isExact);
    }


}

void TreeVisitSlashConstraint(struct Node *Root, char letter){
    if(Root != NULL) {
        TreeVisitSlashConstraint(Root->left_son, letter);
        if(Root -> valid == true){
            for(int i = 0; i < k; i++){
                if((Root -> key)[i] == letter){
                    Root -> valid = false;
                    break;
                }
            }
        }
        TreeVisitSlashConstraint(Root->right_son, letter);
    }
}

void reset_valid(struct hashNode *HashTable, struct iterationList **IListHead, struct iterationList **IListTail){
    for(int i = 0; i < hash_dimension; i++){
        if(HashTable[i].Tree != NULL){
            append(i, IListTail, IListHead);
            HashTable[i].shadow_tree = false;
            reset_tree(HashTable[i].Tree);
        }
    }
}

void reset_tree(struct Node *Root){
    if(Root != NULL) {
        reset_tree(Root->left_son);
        Root -> valid = true;
        reset_tree(Root->right_son);
    }
}

void destroyList(struct iterationList **IListHead, struct iterationList **IListTail){
    struct iterationList *iterate = *IListHead;
    while(iterate != NULL){
        struct iterationList *next = iterate -> next;
        nodeRemove(iterate, NULL, IListHead, IListTail);
        iterate = next;
    }
}

void inserisciInizio(struct hashNode *HashTable, struct iterationList **IListHead, struct iterationList **IListTail, char (*positionInformation)[64], bool need_constraints){
    while (1) {
        char *word1 = malloc(sizeof(char) * max(k, 17));
        char *word = malloc(sizeof(char)*k);
        //read the word
        if (scanf("%s", word1) != 1) return;
        //end the function if true
        if (strcmp(word1, "+inserisci_fine") == 0) return;

        strcpy(word, word1);

        struct Node *new_node = malloc(sizeof(struct Node));
        create_leaf(new_node);
        new_node->key = word;

        bool isWordValid = true;
        short count[64];

        for(int i = 0; i < 64; i++){
            count[i] = 0;
        }

        if(need_constraints) {
            //apply_constraints
            for (int i = 0; i < k; i++) {
                if(positionInformation[i][lett_number(word[i])] == '/'){
                    new_node -> valid = false;
                    isWordValid = false;
                }else {
                    count[lett_number(word[i])]++;
                }
            }

            for (int i = 0; i < k; i++) {
                int num = lett_number(word[i]);
                if(count[num] < lett_count[num]){
                    new_node -> valid = false;
                    isWordValid = false;
                }else if(count[num] != lett_count[num] && exact_num[num] == true){
                    new_node -> valid = false;
                    isWordValid = false;
                }
            }

        }

        if(HashTable[hash_function(word)].shadow_tree == true && isWordValid){
            invalidateTree(HashTable[hash_function(word)].Tree);
            HashTable[hash_function(word)].shadow_tree = false;
            listInsert(hash_function(word), IListHead, IListTail);
        }
        RB_insert(&HashTable[hash_function(word)], new_node);
    }
}

void invalidateTree(struct Node *Root){
    if(Root != NULL){
        invalidateTree(Root -> left_son);
        Root -> valid = false;
        invalidateTree(Root -> right_son);
    }
}

void listInsert(unsigned long index, struct iterationList **IListHead,struct iterationList **IListTail){
    struct iterationList *iterate = *IListHead;
    struct iterationList *new_node = malloc(sizeof(struct iterationList));
    new_node -> index = index;
    new_node -> next = NULL;

    while(iterate != NULL){
        if(iterate == *IListHead && iterate -> index > index){
            *IListHead = new_node;
            new_node -> next = iterate;
            break;
        } else if(iterate == *IListTail && iterate -> index < index){
            *IListTail = new_node;
            iterate -> next = new_node;
        } else if(iterate -> index < index && iterate -> next -> index > index ){

            struct iterationList *nextTemp = iterate -> next;
            iterate -> next = new_node;
            new_node -> next = nextTemp;
            break;
        }
        iterate = iterate -> next;
    }
}




int main() {

    //reading and storing part
    //read k
    if(scanf("%d", &k) != 1) return 0;
    
    //read and store words

    //temporary list necessary to store words before inserting them in the hash table
    struct temporaryListNode *temporaryListHead = malloc(sizeof(struct temporaryListNode));
    struct temporaryListNode *temporaryListTail = temporaryListHead;
    temporaryListTail -> key = NULL;
    //count the starting words
    unsigned long n_starting_words = 0;

    while(true){
        char *word1 = malloc(sizeof(char) * max(k, 17));
        char *word = malloc(sizeof(char) * k);

        if(scanf("%s", word1) != 1) return 0;

        //start the game if true
        if(strcmp(word1, "+nuova_partita") == 0) break;

        n_starting_words++;

        strcpy(word, word1);
        free(word1);

        if(temporaryListTail -> key != NULL) {
            temporaryListTail->next = malloc(sizeof(struct temporaryListNode));
            temporaryListTail = temporaryListTail->next;
        }

        temporaryListTail -> key = word;
        temporaryListTail -> next = NULL;

    }

    //create a hash table of 64^x * 2^t elements
    unsigned char d = ((unsigned char)floor(log2((double)n_starting_words)));

    hash_dimension = (unsigned long)pow(2, d);
    x = d / 6;
    t = d % 6;

    if(x > k){
        x = k;
        t = 0;
    }

    struct hashNode HashTable[hash_dimension];

    //initialize
    for(int i = 0; i < hash_dimension; i++){
        HashTable[i].Tree = NULL;
        HashTable[i].shadow_tree = false;
    }

    //take every element of the list and insert it in the HashTable
    struct temporaryListNode *iterate = temporaryListHead;


    //insert every word from the temporary list in the hash table
    while(iterate != NULL){
        unsigned long hash_index = hash_function(iterate -> key);
        struct temporaryListNode *next_node = iterate -> next;

        if(HashTable[hash_index].Tree == NULL){
            HashTable[hash_index].Tree = malloc(sizeof(struct Node));
            create_leaf(HashTable[hash_index].Tree);
            HashTable[hash_index].Tree -> key = iterate -> key;

        } else{
            struct Node *node_ptr = malloc(sizeof(struct Node));
            create_leaf(node_ptr);
            node_ptr -> key = iterate -> key;

            RB_insert(&HashTable[hash_index], node_ptr);
        }
        free(iterate);
        iterate = next_node;
    }

    //list that contains only the indexes of the hash table that are valid (shadow_tree == false and Tree != NULL)
    struct iterationList *IListTail = NULL;
    struct iterationList *IListHead = IListTail;

    for(unsigned long i = 0; i < hash_dimension; i++){
        if(HashTable[i].Tree != NULL){
            append(i, &IListTail, &IListHead);
        }
    }

    //game part
    while(1) {

        //matrix that says what letters are/are not in a certain place
        char positionInformation[k][64];

        for(int i = 0; i < 64; i++){
            for(int j = 0; j < k; j++){
                positionInformation[j][i] = '$';
            }
            lett_count[i] = 0;
            exact_num[i] = false;
        }

        //read reference word
        char *R_word = malloc(sizeof(char) * k);
        if (scanf("%s", R_word) != 1) return 0;

        //create the structure that contains reference word information
        int R_hash_dimension = k + 1;
        struct R_Node R_Hash_table[R_hash_dimension];

        //initialize
        for (int j = 0; j < R_hash_dimension; j++) {
            R_Hash_table[j].key = '$';
            R_Hash_table[j].count = 0;
            R_Hash_table[j].next = NULL;
        }

        //build the hash table that contains information about the reference word
        int i = 0;
        while (R_word[i] != 0) {
            unsigned char current_index = R_hash_function(R_hash_dimension, R_word[i]);
            //pointer to the current node
            struct R_Node *R_iterate = &R_Hash_table[current_index];

            if (R_iterate->key == '$') {
                R_iterate->key = R_word[i];
                R_iterate->count += 1;
            } else if (R_iterate->key == R_word[i]) {
                R_iterate->count += 1;
            } else {
                while (R_iterate->next != NULL && R_iterate->key != R_word[i]) {
                    R_iterate = R_iterate->next;
                }
                if (R_iterate->key == R_word[i]) {
                    R_iterate->count += 1;
                } else {
                    R_iterate->next = malloc(sizeof(struct R_Node));
                    R_iterate->next->key = R_word[i];
                    R_iterate->next->count = 1;
                    R_iterate->next->next = NULL;
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

            if (strcmp(word1, "+inserisci_inizio") == 0) {
                inserisciInizio(HashTable, &IListHead, &IListTail, positionInformation, true);
                continue;
            }else if (strcmp(word1, "+stampa_filtrate") == 0) {
                stampa_filtrate(HashTable, IListHead);
                continue;
            }else{
                char *word = malloc(sizeof(char)*k);
                strcpy(word, word1);
                free(word1);

                if(is_string_present(HashTable, word)){
                    word_result(R_word, R_Hash_table, R_hash_dimension, word, result);

                    found = true;
                    for(int j = 0; j < k; j++){
                        if(result[j] != '+') found = false;
                    }
                    if(!found){
                        //print the result
                        for(int j = 0; j < k; j++) {
                            printf("%c", result[j]);
                        }
                        if(legal_word_counter > 1 || i == 0) {
                            apply_constraints(HashTable, &IListHead, &IListTail,positionInformation, word, result);
                            legal_word_counter = 0;
                            validNodeCounter(HashTable, &IListHead, &IListTail);
                        }

                        printf("\n%d", legal_word_counter);


                    }else{
                        printf("ok");
                    }
                    printf("\n");

                    if (found) break;
                }else{
                    printf("not_exists\n");
                    continue;
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
                inserisciInizio(HashTable, &IListHead, &IListTail, positionInformation, false);
            }
            else if(strcmp(game_starter, "+nuova_partita") == 0) {
                destroyList(&IListHead, &IListTail);
                //it also rebuilds the list
                reset_valid(HashTable, &IListHead, &IListTail);

                for(int j = 0; j < 64; j++) {
                    exact_num[j] = false;
                    lett_count[j] = 0;
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



















