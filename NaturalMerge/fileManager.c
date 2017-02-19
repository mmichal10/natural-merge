#include "fileManager.h"

int generator(int type, char* filename, int how_many_records)
{
	FILE* f = NULL;
	fopen_s(&f, filename, "w+");
	if (!f) 
		return 0;
	
	char input[MAX_RECORD_LENGTH] = { 0 };
	char buffer[PAGE_SIZE] = { 0 };

	int busy = 0;
	int i = 0;
	int noOptionalParametersInC = 0;
	switch (type)
	{
	case MANUAL:
		printf("Max len = 30\n");
		fseek(stdin, 0, SEEK_END);
		while (i++ < how_many_records)
		{
			printf("Podaj rekord nr.%d: ", i);
			fgets(input, MAX_RECORD_LENGTH, stdin);			
			input[0] = toupper(input[0]);
			printf("\n");
			putInBuffer(input, buffer, &busy, f, &noOptionalParametersInC);
		}
		break;
	case AUTO:
		while (i++ < how_many_records)
		{
			randomRecord(MAX_RECORD_LENGTH, input);
			putInBuffer(input, buffer, &busy, f, &noOptionalParametersInC);
		}
		break;
	default:
		return 0;
	}
	if (busy)
	{
		fputs(buffer, f);
	}
	fclose(f);
	return 1;
}

void randomRecord(int maxLen, char* input)
{
	for (int i = 0; i < MAX_RECORD_LENGTH; input[i++] = 0);
	int len = 0;
	while(!len)
		len = rand() % maxLen;
	for (int i = 0; i < len; i++)
	{
		unsigned char a = rand() % ('z' - 'a') + 'a';
		input[i] = a;
	}
	input[0] -= 'a' - 'A';
}

void putInBuffer(char *text, char *buffer, int *offset, FILE **f, int *writes)
{
	int len = strlen(text);

	if(text[len - 1] == '\n')//jezeli rekordy byly wprowadzane recznie,
		text[--len] = 0;   //enter na koncu jest zdecydowanie zbedny
	
	if (*offset + len < PAGE_SIZE)
	{
		//jezeli w buforze zmiesci sie caly rekord, to wpisuje go
		for (int j = 0; j < len; j++)
			buffer[*offset + j] = text[j];
		*offset += len;
		buffer[*offset] = 0;
	}
	else
	{
		//a jezeli nie, wpisuje tyle ile sie zmiesci i reszta bedzie na kolejnej stronie
		int k;
		for (k = 0; *offset < PAGE_SIZE - 1; (*offset)++, k++)
			buffer[*offset] = text[k];
		fprintf_s(f, "%s", buffer);
		(*writes)++;
		memset(buffer, 0, PAGE_SIZE);
		*offset = 0;
		for (; k < len; k++, (*offset)++)
			buffer[*offset] = text[k];
	}
}

char *getRecord(char page[], int *offset, FILE *f, int *reads)
{
	int recordSize = 0;
	char *buffer = calloc(MAX_RECORD_LENGTH, sizeof(char));
	while (1)
	{
		int len = strlen(page);

		while (*offset < len)
			if (islower(page[*offset]) || !recordSize)
				buffer[recordSize++] = page[(*offset)++];
			else
				return buffer;
		
		if (feof(f))
			return (!strlen(buffer)) ? NULL : buffer;

		fgets(page, PAGE_SIZE, f);
		(*reads)++;
		*offset = 0;
	}
}

void split(char *name, int *reads, int *writes)
{

	FILE * srcFile = NULL;
	fopen_s(&srcFile, name, "r");
	if (!srcFile) return;

	char readPage[PAGE_SIZE] = { 0 };
	int readOffset = 0;



	FILE *dstFiles[2] = { 0 };
	fopen_s(&dstFiles[0], "tape0.txt", "w+");
	fopen_s(&dstFiles[1], "tape1.txt", "w+");
	if (!dstFiles[0] || !dstFiles[1]) return;

	char *writePages[2];
	for (int i = 0; i < 2; i++)
		writePages[i] = calloc(PAGE_SIZE, sizeof(char));
	int writeOffsets[2] = { 0 };	


	int switcher = 0;

	char *lastRecord = NULL;
	char *record = NULL;
	while ((record = getRecord(readPage, &readOffset, srcFile, reads)))
	{
		if (compare(record, lastRecord) == -1)
		{
			switcher++;
			switcher %= 2;
		}
		putInBuffer(record, writePages[switcher], &writeOffsets[switcher], dstFiles[switcher], writes);
		free(lastRecord);
		lastRecord = record;
	}

	for(int i =0;i<2;i++)
		if (writeOffsets[i])
		{
			fprintf_s(dstFiles[i], "%s", writePages[i]);
			(*writes)++;
		}

	fclose(srcFile);
	for (int i = 0; i < 2; i++)
		fclose(dstFiles[i]);
	
	for (int i = 0; i < 2; i++)
		free(writePages[i]);
}

int merge(char *name, int *reads, int *writes)
{
	FILE *srcFiles[2];
	fopen_s(&srcFiles[0], "tape0.txt", "r+");
	fopen_s(&srcFiles[1], "tape1.txt", "r+");
	if (!srcFiles[0] || !srcFiles[1]) return;

	char readPages[2][PAGE_SIZE] = { { 0 } };
	int readOffsets[2] = { 0 };
	char *heads[2] = { 0 };



	FILE * dstFile = NULL;
	fopen_s(&dstFile, name, "w+");
	if (!dstFile) return;

	char *writePage = calloc(PAGE_SIZE, sizeof(char));
	int writeOffset = 0;


	int switcher = 0;
	int isSorted = 1;

	int newSeries[2] = { 0 };
	char *lastRecords[2] = { NULL };

	heads[0] = getRecord(readPages[0], &readOffsets[0], srcFiles[0], reads);
	heads[1] = getRecord(readPages[1], &readOffsets[1], srcFiles[1], reads);
	while (heads[0] || heads[1])
	{
		int id = idOfMin(heads, 2);

		if (compare(heads[id], lastRecords[id]) == -1)
		{
			newSeries[id] = 1;
			if (compare(heads[(id + 1) % 2], lastRecords[(id + 1) % 2]) == -1)
				newSeries[(id + 1) % 2] = 1;
			isSorted = 0;
		}

		if (newSeries[0] && newSeries[1])
		{
			newSeries[0] = newSeries[1] = 0;
			lastRecords[0] = lastRecords[1] = NULL;
			continue;
		}
		else if((newSeries[1] && heads[0]) || (heads[1] && newSeries[0]))
			id = (++id) % 2;

		putInBuffer(heads[id], writePage, &writeOffset, dstFile, writes);

		lastRecords[id] = heads[id];
		heads[id] = getRecord(readPages[id], &readOffsets[id], srcFiles[id], reads);
	}

	if (writeOffset)
	{
		fprintf_s(dstFile, "%s", writePage);
		(*writes)++;
	}	

	fclose(dstFile);
	for (int i = 0; i < 2; i++)
		fclose(srcFiles[i]);

	free(writePage);

	return isSorted;
}

void showFile(char* name)
{
	FILE * f = NULL;
	fopen_s(&f, name, "r");
	if (f)
	{
		char *buffer = malloc(sizeof(char)*PAGE_SIZE);
		while (fgets(buffer, PAGE_SIZE, f))
			puts(buffer);
		free(buffer);
		fclose(f);
	}
	printf("\n\n");
}

int idOfMin(char *heads[], int count)
{
	int id = 0;
	for (int i = 0; i < count; i++)
		if ((compare(heads[i], heads[id]) == -1 && heads[i] && heads[id]) || (heads[i] && !heads[id]))
			id = i;
	return id;
}

