#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#define LEN 300
#define HLN LEN/2
#define CSIZE 2
#define B2LEN LEN*LEN*sizeof(int)
#define BORDER 120

void initArrays();
void calculateLife(int N);
void freeArrays();
int* get_h_in();