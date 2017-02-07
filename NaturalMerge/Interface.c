#pragma once

#include"Interface.h"

void start()
{
	printf("\t  Michal Mielewczyk, 149271\n");
	printf("\Struktury Baz Danych - Projekt 1\n");
	printf("\t    Scalanie naturalne\n");
	printf("---------------------------------------------\n");
}

void mainMenu()
{
	char *name = "1.txt";
	system("del tape*.txt");
	while (1)
	{
		system("CLS");
		start();
		printf("1. Generuj plik\n");
		printf("2. Pokaz plik\n");
		printf("3. Sortuj\n");
		printf("0. - wyjscie\n");
		fseek(stdin, 0, SEEK_END);
		char x;
		scanf_s("  %c", &x);
		switch (x)
		{
		case '1':
			generatorMenu();
			break;
		case '2':
			showMenu(name);
			break;
		case '3':
			sortMenu(name);
			break;
		case '0':
			return;
		default:
			;
		}
		
	}
}

void showMenu(char *name)
{
	system("CLS");
	while (1)
	{		
		printf("1. Pokaz plik przed sortowaniem\n");
		printf("2. Pokaz pierwsza tasme - podzial\n");
		printf("3. Pokaz druga tasme  - podzial\n");
		printf("0. Powort\n");
		char x;
		scanf_s(" %c", &x);
		system("CLS");
		switch (x)
		{
		case '1':
			showFile("1.txt");
			break;
		case '2':
			showFile(name);
			break;
		case '3':
			showFile(name);
			break;
			break;
		case '0':
			return;
		default:
			break;
		}
		fseek(stdin, 0, SEEK_END);
	}
}

void generatorMenu()
{
	system("del tape*.txt");
	while (1)
	{
		system("CLS");
		printf("1. Generowanie automatyczne\n");
		printf("2. Generowanie reczne\n");
		printf("0. Powrot\n");
		char x;
		scanf_s(" %c", &x);
		switch (x)
		{
		system("CLS");
		case '1':
			generateAuto();
			return;
		case '2':
			generateManual();
			return;
		case '0':
			return;
		default:
			break;
		}
		fseek(stdin, 0, SEEK_END);
	}
}

void generateAuto()
{
	system("CLS");
	printf("Ile rekordow ma zostac wpisanych do pliku?\n");
	int x;
	scanf_s(" %d", &x);
	generator(AUTO, "1.txt", x);
}

void generateManual()
{
	system("CLS");
	printf("Ile rekordow ma zostac wpisanych do pliku?\n");
	int x;
	scanf_s(" %d", &x);
	generator(MANUAL, "1.txt", x);
}

void sortMenu(char *name)
{
	system("CLS");
	int stage = 0;
	int reads = 0;
	int writes = 0;
	while (1)
	{
		printf("1. Sortuj \n");
		printf("2. Wykonaj jedna faze\n");
		printf("0. Powrot \n");

		fseek(stdin, 0, SEEK_END);
		char x;
		scanf_s("  %c", &x);

		switch (x)
		{
		case '1':
			do
			{
				split(name, &reads, &writes);
				stage++;
			}
			while (!merge(name, &reads, &writes));

			showFile(name);
			printf("Plik zostal posortowany w %d fazach, przy %d odczytach i %d zapisach: \n", stage, reads, writes);
			stage = reads = writes = 0;
			system("del tape*.txt");
			break;
		case '2':
			split(name, &reads, &writes);
			if (!merge(name, &reads, &writes))
			{
				printf("Wykonano %d faz, %d odczytow i %d zapisow\n", ++stage, reads, writes);
				printf("Tape1:\n");
				showFile("tape0.txt");
				printf("Tape2:\n");
				showFile("tape1.txt");
				printf("Split:\n");
				showFile("1.txt");
			}
			else
			{
				printf("Plik zostal posortowany w %d fazach, przy %d odczytach i %d zapisach: \n", ++stage, reads, writes);
				stage = reads = writes = 0;
				showFile(name);
				system("del tape*.txt");
			}
			break;
		case '0':
			return;
		default:
			;
		}
	}
}

