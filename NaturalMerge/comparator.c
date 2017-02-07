#include "comparator.h"

#include <ctype.h>

int compare(char *s1, char *s2)
{
	if (!s1) return -1;
	else if (!s2) return 1;
	else if (!s1 && !s2) return 0;

	int hash1[HASH_TABLE_LENGTH] = { 0 };
	int hash2[HASH_TABLE_LENGTH] = { 0 };
	//kazdy string bedzie mial hash-tablice z zapisana liczba wystapien kazdej litery

	fillHashTable(s1, hash1);
	fillHashTable(s2, hash2);
	while (1)
	{
		int max1 = findMax(hash1);
		int max2 = findMax(hash2);
		if (max1 > max2) 
			return 1;
		else if (max1 < max2)
			return -1;
		else if (!max1 && !max2)
			return 0;
	}
}

void fillHashTable(char *s, int *hash)
{
	int i = 0;
	while (s[i])
		hash[tolower(s[i++]) - 'a']++;
}

int findMax(int *tb)
{
	int max_id = 0;
	for (int i = 1; i < HASH_TABLE_LENGTH; i++)
		if (tb[i] > tb[max_id])
			max_id = i;
	int max = tb[max_id];
	tb[max_id] = 0;
	return max;
}

int array_max(int*t, int size)
{
	int index = 0;

	for(int i=1;i<size;i++)
		if (t[i] > t[index]) 
			index = i;

	return index;
}
