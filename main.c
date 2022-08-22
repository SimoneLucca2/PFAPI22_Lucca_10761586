#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BigNum 2147483647

typedef enum { false, true } bool;

struct Node {
    char *key;
    struct Node *right_son;
    struct Node *left_son;
    bool valid;
};

//Reference node, contains information about a letter in the reference word
struct R_Node{
    char key;
    int count;
    struct R_Node *next;
};

struct Box{
    int wordInTreeFixed;
    int validWordInTree;
    struct Node *Tree;
};

int n, k;
int legalWordCounter = 0, startingWords = 0;
char buffer[18];

//information that the user have about the number of letters in the reference word
int lett_count[64];

bool isFirst = true;

int lett_number(char c){
    if(c == '-')
        return 0;
    else if('0' <= c && c <= '9')
        return (int)c - (int)'0' + 1;
    else if('A' <= c && c <= 'Z')
        return (int)c - (int)'A' + (int)'9' - (int)'0' + 2;
    else if(c == '_')
        return 37;
    else if('a' <= c && c <= 'z')
        return (int)c - (int)'a' + 38;
    else return -1;
}

void TreeInsert(struct Node **Root, char *word, bool isWordValid){
    struct Node *iterate = *Root;
    struct Node *newNode = malloc(sizeof(struct Node));

    newNode -> right_son = newNode -> left_son = NULL;
    newNode -> key = word;
    newNode -> valid = isWordValid;

    if(*Root == NULL){
        *Root = newNode;
    }
    while(iterate != NULL){
        if(memcmp(word, iterate -> key, k) > 0){
            if(iterate -> right_son == NULL){
                iterate -> right_son = newNode;
                break;
            }
            iterate = iterate -> right_son;
        }else if(memcmp(word, iterate -> key, k) < 0){
            if(iterate -> left_son == NULL){
                iterate -> left_son = newNode;
                break;
            }
            iterate = iterate -> left_son;
        }else{
            free(newNode);
            return;
        }
    }
}

void MatrixInsert(struct Box HashMatrix[64][64], char *word, bool isWordValid){

    int i = lett_number(word[0]);
    int j = lett_number(word[1]);

    if(HashMatrix[i][j].Tree == NULL){
        HashMatrix[i][j].wordInTreeFixed = 0;
        HashMatrix[i][j].validWordInTree = 0;
    }

    TreeInsert(&(HashMatrix[i][j].Tree), word, isWordValid);
    HashMatrix[i][j].wordInTreeFixed += 1;
    if(isWordValid){
        HashMatrix[i][j].validWordInTree += 1;
    }
}

bool IsWordInTree(struct Node *Root, char *word){
    struct Node *iterate = Root;

    while(iterate != NULL){

        if(memcmp(iterate -> key, word, k) == 0) break;

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

bool isWordPresent(struct Box HashMatrix[64][64], char *word){
    return IsWordInTree(HashMatrix[lett_number(word[0])][lett_number(word[1])].Tree, word);
}

int R_hash_function(int dim, char c){
    return (int)c % dim;
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

struct R_Node *get_hash_node(struct R_Node *hash, unsigned char R_hash_dimension, char c) {
    struct R_Node *iterate = &hash[R_hash_function(R_hash_dimension, c)];
    while(iterate -> key != c) {
        iterate = iterate -> next;
    }
    return iterate;
}

void wordResult(const char* reference_word, struct R_Node* hash, int R_hash_dimension, char *word, char* result) {
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

    for(int i = 0; i < R_hash_dimension; i++){
        struct R_Node *iterate = hash_copy[i].next;
        while(iterate != NULL){
            struct R_Node *next = iterate -> next;
            free(iterate);
            iterate = next;
        }
    }
}

void CountConstraints(struct Node *currentNode, const int *countConstraintsIndexes){
    //here the node is valid
    int i = 0;
    int tempArr[64];

    while(countConstraintsIndexes[i] != BigNum){
        tempArr[countConstraintsIndexes[i]] = 0;
        i++;
    }

    i = 0;

    for(int j = 0; j < k; j++){
        tempArr[lett_number((currentNode->key)[j])]++;
    }

    while(countConstraintsIndexes[i] != BigNum){

        int expected = lett_count[countConstraintsIndexes[i]];
        if(expected <= 0 && tempArr[countConstraintsIndexes[i]] != (-expected)){
            currentNode -> valid = false;
            legalWordCounter--;
            return;
        }else if(expected > 0 && tempArr[countConstraintsIndexes[i]] < expected){
            currentNode -> valid = false;
            legalWordCounter--;
            return;
        }
        i++;
    }
}

void TreeVisitConstraints(struct Node *Root, const char *word, const char* result, int *indexArray, int *countConstraintsIndexes){
    if(Root != NULL){
        if(Root -> valid == true || isFirst){

            int i = 0;
            while(indexArray[i] <= 1) i++;
            bool dontDo = false;

            if(isFirst){
                Root -> valid = true;
            }

            while (indexArray[i] != BigNum) {
                if ((result[indexArray[i]] == '/' || result[indexArray[i]] == '|') &&
                    (Root->key)[indexArray[i]] == word[indexArray[i]]) {
                    Root->valid = false;
                    legalWordCounter--;
                    dontDo = true;
                    break;
                }else if(result[indexArray[i]] == '+' && (Root->key)[indexArray[i]] != word[indexArray[i]]){
                    Root->valid = false;
                    legalWordCounter--;
                    dontDo = true;
                    break;
                }
                i++;
            }
            if(dontDo == false) {
                CountConstraints(Root, countConstraintsIndexes);
            }
        }
        TreeVisitConstraints(Root -> left_son, word, result, indexArray, countConstraintsIndexes);
        TreeVisitConstraints(Root -> right_son, word, result, indexArray, countConstraintsIndexes);
    }
}

void applyConstraints(struct Box HashTable[64][64], char (*positionInformation)[64],char *word, const char* result){

    //--------------------------------------------------------------------------------------------------
    //the array lett_count contains the count of the letter in the word
    // 0 means "the letter is not in the word"
    // INFINITY means "the user does not have information"
    // positive number means "the user didn't have this information and there could be more letters"
    // negative number means "the user didn't have this information and the letters are exact"
    //--------------------------------------------------------------------------------------------------

    int temporaryCount[64];
    for(int i = 0; i < 64; i++){
        temporaryCount[i] = BigNum;
    }

    //+1 because the last index is used to stop the iteration
    int countConstraintsIndexes[k+1];
    int tempIndex = 0;

    for(int i = 0; i < k; i++){
        int CurrentLettNum = lett_number(word[i]);

        //--------------------------------------------------------------------------------------------------
        //if '/' there are 2 cases -> there are 0 of the corresponding letters
        //                         -> the number of letters is exact (if there is + or | )
        //if '+' or '|' still 2 cases
        //                         -> if I encountered '/' before then the number is exact
        //                         -> if I didn't the number might be exact if I will encounter '/'
        //--------------------------------------------------------------------------------------------------


        if(result[i] == '/' && temporaryCount[CurrentLettNum] == BigNum){
            temporaryCount[CurrentLettNum] = 0;
            countConstraintsIndexes[tempIndex] = CurrentLettNum;
            tempIndex++;
        }
        else if(result[i] == '/' && temporaryCount[CurrentLettNum] > 0){
            temporaryCount[CurrentLettNum] = (- temporaryCount[CurrentLettNum]);
        }
        else if((result[i] == '+' || result[i] == '|') && temporaryCount[CurrentLettNum] == BigNum){
            temporaryCount[CurrentLettNum] = 1;
            countConstraintsIndexes[tempIndex] = CurrentLettNum;
            tempIndex++;
        }
        else if(result[i] == '+' || result[i] == '|'){
            if(temporaryCount[CurrentLettNum] > 0)
                temporaryCount[CurrentLettNum] += 1;
            else
                temporaryCount[CurrentLettNum]--;
        }
    }

    countConstraintsIndexes[tempIndex] = BigNum;

    //it is necessary to go through the array to compare the temporaryCount to the past counts
    int u = 0;
    tempIndex = 0;
    while(countConstraintsIndexes[u] != BigNum){

        if(lett_count[countConstraintsIndexes[u]] <= 0){
            u++;
            continue;
        }

        if(lett_count[countConstraintsIndexes[u]] == BigNum){
            lett_count[countConstraintsIndexes[u]] = temporaryCount[countConstraintsIndexes[u]];
            countConstraintsIndexes[tempIndex] = countConstraintsIndexes[u];
            tempIndex++;
        } else if(temporaryCount[countConstraintsIndexes[u]] < 0 && lett_count[countConstraintsIndexes[u]] > 0){
            lett_count[countConstraintsIndexes[u]] = temporaryCount[countConstraintsIndexes[u]];
            countConstraintsIndexes[tempIndex] = countConstraintsIndexes[u];
            tempIndex ++;
        }else if(lett_count[countConstraintsIndexes[u]] < temporaryCount[countConstraintsIndexes[u]] && temporaryCount[countConstraintsIndexes[u]] > 0){
            lett_count[countConstraintsIndexes[u]] = temporaryCount[countConstraintsIndexes[u]];
            countConstraintsIndexes[tempIndex] = countConstraintsIndexes[u];
            tempIndex ++;
        }
        u++;
    }

    countConstraintsIndexes[tempIndex] = BigNum;

    //this array contains the indexes of the letter with more information
    int indexArray[k+1];

    tempIndex = 0;
    int lastPlus = 0;
    for(int i = 0; i < k; i++){

        int CurrentLettNum = lett_number(word[i]);

        if((result[i] == '/' || result[i] == '|') && positionInformation[i][CurrentLettNum] != '/'){
            indexArray[tempIndex] = i;
            tempIndex++;

            //indexArray is made like this: {0, 1, indexes of +, other indexes, INFINITY, ... }
            if(i == 0 || i == 1) lastPlus++;

            positionInformation[i][CurrentLettNum] = '/';

        }else if(result[i] == '+' && positionInformation[i][CurrentLettNum] != '+'){
            if(lastPlus != tempIndex){
                int temp = indexArray[lastPlus];
                indexArray[lastPlus] = i;
                indexArray[tempIndex] = temp;
            }else{
                indexArray[tempIndex] = i;
            }

            tempIndex++;
            lastPlus++;

            //set all the other char in positionInformation[i] as '/'
            for(int j = 0; j < 64; j++){
                positionInformation[i][j] = '/';
            }
            positionInformation[i][lett_number(word[i])] = '+';
        }
    }

    indexArray[tempIndex] = BigNum;

    //--------------------------------------------------------------------------------------------------
    //now there are 2 arrays:
    // countConstraintsIndexes -> {32, 34, 23, INFINITY, casual stuff...}
    // indexArray -> {1, 3, 4, 5, INFINITY, ... }
    //using these 2 arrays it will not be necessary to apply all the constraints,
    //but only the new ones
    //--------------------------------------------------------------------------------------------------

    if(countConstraintsIndexes[0] == BigNum && indexArray[0] == BigNum) return;

    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 64; j++){

            if(HashTable[i][j].Tree == NULL) continue;
            if(!isFirst && HashTable[i][j].validWordInTree <= 0) continue;

            if(isFirst){
                HashTable[i][j].validWordInTree = HashTable[i][j].wordInTreeFixed;
            }

            bool skip = false;

            u = 0;
            while (indexArray[u] <= 1) {

                if ((result[indexArray[u]] == '/' || result[indexArray[u]] == '|') &&
                    (HashTable[i][j].Tree->key)[indexArray[u]] == word[indexArray[u]]) {

                    legalWordCounter -= HashTable[i][j].validWordInTree;

                    HashTable[i][j].validWordInTree = 0;
                    skip = true;
                    break;

                } else if (result[indexArray[u]] == '+' &&
                           (HashTable[i][j].Tree->key)[indexArray[u]] != word[indexArray[u]]) {

                    legalWordCounter -= HashTable[i][j].validWordInTree;

                    HashTable[i][j].validWordInTree = 0;
                    skip = true;
                    break;
                }
                u++;
            }

            if(skip) continue;

            int temp = legalWordCounter;
            TreeVisitConstraints(HashTable[i][j].Tree, word, result, indexArray, countConstraintsIndexes);
            HashTable[i][j].validWordInTree -= temp - legalWordCounter;

        }
    }
    isFirst = false;
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

void printTreeFirst(struct Node *Tree){
    if(Tree != NULL) {
        printTreeFirst(Tree->left_son);
        printf("%s\n", Tree->key);
        printTreeFirst(Tree->right_son);
    }
}

void stampaFiltrate(struct Box HashTable[64][64]){

    int printedWords = 0;

    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 64; j++){

            if(HashTable[i][j].Tree == NULL) continue;
            if(!isFirst && HashTable[i][j].validWordInTree <= 0) continue;

            if(isFirst){
                printTreeFirst(HashTable[i][j].Tree);
                printedWords += HashTable[i][j].wordInTreeFixed;
            }else{
                print_tree(HashTable[i][j].Tree);
                printedWords += HashTable[i][j].validWordInTree;
            }

            if(printedWords >= legalWordCounter) return;

        }
    }
}

void inserisciInizio(struct Box HashTable[64][64], char (*positionInformation)[64], bool need_constraints){

    bool globalNeedConstraints = need_constraints;

    while (1) {

        need_constraints = globalNeedConstraints;

        //read the word
        if (scanf("%s", buffer) != 1) return;

        //end the function if true
        if (buffer[0] == '+' && buffer[1] == 'i') return;

        char *word = malloc(k+1);
        memcpy(word, buffer, k+1);

        bool isWordValid = true;

        if(positionInformation[0][lett_number(word[0])] == '/' || positionInformation[1][lett_number(word[1])] == '/'){
            need_constraints = false;
            isWordValid = false;
        }

        if(need_constraints) {
            int count[64];

            for(int i = 0; i < 64; i++){
                count[i] = 0;
            }

            //apply_constraints
            for (int i = 0; i < k; i++) {
                if(positionInformation[i][lett_number(word[i])] == '/'){
                    isWordValid = false;
                    break;
                }else {
                    count[lett_number(word[i])]++;
                }
            }

            for(int i = 0; i < 64; i++){
                if(!isWordValid) break;
                if(lett_count[i] != BigNum){

                    if(count[i] < lett_count[i] && lett_count[i] > 0){
                        isWordValid = false;
                        break;
                    }else if(count[i] != -lett_count[i] && lett_count[i] <= 0){
                        isWordValid = false;
                        break;
                    }
                }
            }

        }

        if(isWordValid){
            legalWordCounter++;
        }

        startingWords++;

        MatrixInsert(HashTable, word, isWordValid);
    }
}

int main() {

    //reading and storing part
    //read k
    if(scanf("%d", &k) != 1) return 0;

    struct Box HashMatrix[64][64];

    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 64; j++){
            HashMatrix[i][j].Tree = NULL;
        }
    }

    //read and store words
    while(1){

        if(scanf("%s", buffer) != 1) return 0;

        //start the game if true
        if(buffer[0] == '+' && buffer[1] == 'n') break;

        if(buffer[0] == '+' && buffer[1] == 'i') continue;

        char *word = malloc(k+1);

        memcpy(word, buffer, k+1);

        startingWords++;

        MatrixInsert(HashMatrix, word, true);
    }

    legalWordCounter = startingWords;

    while(true) {

        //read reference word
        char R_word[k+1];
        if (scanf("%s", R_word) != 1) return 0;

        //create the structure that contains reference word information
        int hash_dimension = k+1;
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
            int current_index = R_hash_function(hash_dimension, R_word[i]);
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

        char positionInformation[k][64];

        //initialize
        for(i = 0; i < 64; i++){
            for(int j = 0; j < k; j++){
                positionInformation[j][i] = '$';
            }
            lett_count[i] = BigNum;
        }


        //read word p - note exceptions +inserisci_inizio +stampa_filtrate
        i = 0;
        bool found = false;
        isFirst = true;

        while (i < n) {

            char result[k];

            if (scanf("%s", buffer) != 1) return 0;

            if(buffer[0] != '+'){

                //if the string is present in the matrix calculate the result and apply constraints
                if(!isWordPresent(HashMatrix, buffer)){
                    printf("not_exists\n");
                    continue;
                } else{

                    if(strcmp(buffer, R_word) == 0){
                        found = true;
                        break;
                    }

                    wordResult(R_word, R_Hash_table, hash_dimension, buffer, result);

                    //print the result
                    for(int j = 0; j < k; j++){
                        printf("%c", result[j]);
                    }
                    printf("\n");

                    if(legalWordCounter > 1){
                        applyConstraints(HashMatrix, positionInformation,buffer, result);
                    }

                    isFirst = false;

                    printf("%d\n", legalWordCounter);
                }
            }

            //inserisci inizio
            else if(buffer[1] == 'i'){
                if(isFirst){
                    inserisciInizio(HashMatrix, positionInformation, false);
                }else{
                    inserisciInizio(HashMatrix, positionInformation, true);
                }
                continue;
            }

            //stampa filtrate
            else if(buffer[1] == 's'){

                if(!isFirst && legalWordCounter == 1){
                    printf("%s\n", R_word);
                }else{
                    stampaFiltrate(HashMatrix);
                }
                continue;
            }
            i++;
        }

        if(found){
            printf("ok\n");
        }else{
            printf("ko\n");
        }

        while(true){

            if (scanf("%s", buffer) != 1) return 0;

            if(buffer[0] == '+' && buffer[1] == 'i'){
                //INSERISCI INIZIO
                inserisciInizio(HashMatrix, positionInformation, false);
            } else if(buffer[0] == '+' && buffer[1] == 'n'){
                legalWordCounter = startingWords;

                for(i = 0; i < hash_dimension; i++){
                    struct R_Node *iterate = R_Hash_table[i].next;
                    while(iterate != NULL){
                        struct R_Node *next = iterate -> next;
                        free(iterate);
                        iterate = next;
                    }
                }

                break;
            }else{
                return 0;
            }
        }

    }
}



