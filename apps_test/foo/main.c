#include <stdio.h>
#include <stdlib.h>

#define TAILLE_TAB 20

int main(int argc, char** argv)
{
	
	int a = 2, b = 3, c = 4,d = 5;
	
	int tab[TAILLE_TAB][TAILLE_TAB];
	
	for(int i = 1 ; i < TAILLE_TAB-1 ; i++){
		for(int j = 1 ; j < TAILLE_TAB-1 ; j++){
			tab[i][j] = a*tab[i][j-1] + b*tab[i][j+1] + c*tab[i-1][j] + d*tab[i+1][j]; 		
		}
	}
}


void foo(int d , int e){
	int b,c;
	b = 0;
	c = 0 ;
	
	c = b +1;

}
