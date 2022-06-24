#include <stdlib.h>
#include <stdio.h>

int n, k;

void read_legal_words(){
    char *word;
    while(1){
        if(scanf("%s", word) != 1) return;
        if(word[0] == '+' && word[1] == 'n') break; //when the user types +nuova_partita it stops reading
        //INSERT word IN DATA STRUCTURE
    }
}


int main() {
    //reading and storing part
        //read k
        //read words
        //store each word
    if(scanf("%d", &k) != 1) return 0;
    read_legal_words();


    //game part
        //read reference word
        //read n
        //read word p - note exceptions +inserisci_inizio
        //check if p respects constraints
        //print res  / ok / ko
        //apply constrains
        //count every legal word remaining

    char *r;
    if(scanf("%s", r) != 1) return 0;
    if(scanf("%d", &n) != 1) return 0;

    while(1){
        if(scanf("%s", p) != 1) return 0;
        if(p[0] == '+' && p[1] == 'i') //CALL READ FUNCTION
    }




}

