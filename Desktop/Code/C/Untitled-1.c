#include <stdlib.h>
#include <stdio.h>

void carre_1(int cote ) {
    int ligne = 0 ;
    while (ligne < cote) {
        int col = 0 ; 
        while (col < cote) {
            printf("%d", ligne) ;
            col = col + 1 ;
        }
        putchar('\n') ; 
        ligne = ligne + 1 ; 
    }
}

int main() {
    carre_1(3)
}