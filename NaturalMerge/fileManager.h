#pragma once
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "comparator.h"

#define MANUAL 0
#define AUTO 1
#define MAX_RECORD_LENGTH 30
#define PAGE_SIZE 512

int generator(int type, char* filename, int how_many_records);
void randomRecord(int maxLen, char* input);
void putInBuffer(char *text, char *buffer, int *busy, FILE **f, int *writes);
//char *recordsCat(char *s1, char *s2);
void showFile(char *name);
char *getRecord(char **page, int *offset, FILE *f, int *reads);

int idOfMin(char *heads[], int count);

void split(char *name, int *reads, int *writes);
int merge(char *name, int *reads, int *writes);