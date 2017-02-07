#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORD_LENGTH 30
#define HASH_TABLE_LENGTH ('z' - 'a')

/*
 * 1, jezeli pierwszy wiekszy
 * -1, jezeli drugi wiekszy
 * 0, jezeli rowne
*/
int compare(char* s1, char* s2);
void fillHashTable(char *s, int *hash);
int findMax(int *tb);