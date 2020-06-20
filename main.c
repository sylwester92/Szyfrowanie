#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_SIZE 27
#define NUMBER_OF_STRING 27

// Inicjalizacja Szyfru Cezara
caesar(int key)
{
	FILE* input_file_caesar, * output_file_caesar;
	char ch;

	input_file_caesar = fopen("input.txt", "r");
	if (input_file_caesar == NULL)
	{
		puts("Nie mozna otworzyc pliku!");
		exit(1);
	}

	output_file_caesar = fopen("encrypted.txt", "w+");
	if (output_file_caesar == NULL)
	{
		puts("Nie mozna otworzyc pliku!");
		exit(1);
	}
	printf("Wiadomosc do zaszyfrowania:\n");
	fseek(input_file_caesar, 0, SEEK_END);
	size_t length = ftell(input_file_caesar); // Ustalamy d³ugoœæ wiadomoœci
	rewind(input_file_caesar);
	int i = 0;
	do // Wyœwietlamy wiadomoœæ, któr¹ chcemy zaszyfrowaæ
	{
		ch = fgetc(input_file_caesar);
		printf("%c", ch);
		fputc(ch + key, output_file_caesar); // Szyfrujemy wiadomoœæ i zapisujemy zaszyfrowane znaki do pliku
		i++;
	} while (i < length);
	printf("\n\nWiadomosc zaszyfrowana wyglada nastepujaco:\n");
	rewind(output_file_caesar); // Ustawiamy wskaŸnik na pocz¹tek pliku
	do // Wyœwietlamy wiadomoœæ w postaci zaszyfrowanej
	{
		ch = fgetc(output_file_caesar);
		printf("%c", ch);
	} while (ch != EOF);
	printf("\n\nOdszyfrowuje wiadomosc:\n");
	rewind(output_file_caesar);
	i = 0;
	do
	{
		ch = fgetc(output_file_caesar);
		printf("%c", ch - key); // Odszyfrowujemy wiadomoœæ podstawiaj¹c w odwrotnym kierunku o wartoœæ klucza
		i++;
	} while (i < length);
	fclose(output_file_caesar);
	fclose(input_file_caesar);
}

// Inicjalizacja homofonicznego szyfru podstawieniowego
homophonic_cipher()
{
	FILE* input_file, * output_file, * dictionary_file;
	char ch, x;

	input_file = fopen("input.txt", "r");
	if (input_file == NULL)
	{
		puts("Nie znaleziono pliku!");
		exit(1);
	}

	output_file = fopen("homophonic.txt", "w+");
	if (output_file == NULL)
	{
		puts("Nie znaleziono pliku!");
		exit(1);
	}
	printf("Wiadomosc do zaszyfrowania:\n");
	int i = 0;
	dictionary_file = fopen("dictionary.txt", "r");
	do
	{
		ch = fgetc(input_file);
		printf("%c", ch); // Wyœwietlamy wiadomoœæ, któr¹ chcemy zaszyfrowaæ
		do
		{
			x = fgetc(dictionary_file); // Pobieramy znak z pliku ze s³ownikiem

			if (x == ch) // Sprawdzamy, czy znaleziono znak w kluczu
			{
				break; // Je¿eli znaleziono, przerywamy pêtle; wskaŸnik ustawia siê na pozycji
			}		   // znalezionej litery
		} while (x != EOF);
		x = fgetc(dictionary_file);  // Wywo³uj¹c kolejne fgetc pobieramy znak o jeden dalej w prawo
		fputc(x, output_file);		 // zapisujemy zmienn¹ do pliku
		rewind(dictionary_file);
		i = 0;
	} while (ch != EOF);
	printf("\n\nWiadomosc zaszyfrowana:\n");
	rewind(output_file);
	rewind(dictionary_file);
	fclose(input_file);
	do
	{
		ch = fgetc(output_file);
		printf("%c", ch); // Wyœwietlamy wiadomoœæ w postaci zaszyfrowanej
	} while (ch != EOF);
	rewind(output_file);
	printf("\n\nOdszyfrowuje wiadomosc:\n");

	// Odszyfrowujemy wiadomoœæ
	do
	{
		ch = fgetc(output_file);
		do
		{
			x = fgetc(dictionary_file);
			if (x == ch)
			{
				break;
			}
		} while (x != EOF);
		i = ftell(dictionary_file);
		fseek(dictionary_file, i - 2, SEEK_SET); // Przesuwamy wskaŸnik pliku o dwa w lewo
		x = fgetc(dictionary_file); // fgetc przesuwa wskaŸnik o jeden w prawo, czyli finalnie
		rewind(dictionary_file);	// mamy przesuniêcie o jeden w lewo
		printf("%c", x); // Wyœwietlamy odszyfrowan¹ wiadomoœæ
		i = 0;
	} while (ch != EOF);
	fclose(dictionary_file);
	fclose(output_file);
	fclose(input_file);
}
// Inicjalizacja wieloalfabetowego szyfru podstawieniowego
running_key_cipher()
{
	size_t size = 1;
	FILE* input_file, * output_file, * keystream_file, * code_table;
	input_file = fopen("message.txt", "r");
	char* message;
	fseek(input_file, 0, SEEK_END);
	size_t length = ftell(input_file); // Ustalamy d³ugoœæ wiadomoœci
	message = malloc(length); // alokacja pamiêci dla tablicy stringów
	rewind(input_file);
	char tabula_recta[NUMBER_OF_STRING][MAX_STRING_SIZE] = { "" }; // Okreœlenie wymiarów tablicy
	char ch, x;
	int i = 0;
	int j = 0;
	int k = 0;
	int z = 0;
	printf("Wiadomosc do zaszyfrowania:\n");
	do
	{
		ch = fgetc(input_file);
		message[i] = ch;
		printf("%c", message[i]);
		i++;
	} while (ch != EOF);
	printf("\n");
	code_table = fopen("tabula_recta.txt", "r");
	for (i = 0; i <= 26; i++)
	{
		for (j = 0; j <= 26; j++)
		{
			ch = fgetc(code_table);
			tabula_recta[i][j] = ch; // Uzupe³niamy nasz¹ tablicê "tabula recta"
		}
	}
	rewind(code_table);
	fclose(code_table);
	fclose(input_file);
	output_file = fopen("encrypted_message.txt", "w+");
	keystream_file = fopen("keystream.txt", "r");
	int row = 0, column = 0;
	k = 0;
	printf("\n\nWiadomosc zaszyfrowana: \n");
	/* Szyfrujemy wiadomoœæ odszukuj¹c kolejne litery klucza w pierwszym wierszu oraz 
	litery wiadomoœci w pierwszej kolumnie. Nastêpnie na przeciêciu wiersza z kolumn¹
	mamy nasz zaszyfrowany znak */

	// SZYFROWANIE //
	do
	{
		x = fgetc(keystream_file);
		for (i = 0; i < 26; i++)
		{
			if (tabula_recta[0][i] == x)
			{
				row = i;
				break;
			}
		}
		for (j = 0; j < 26; j++)
		{
			if (tabula_recta[j][0] == message[k])
			{
				column = j;
				k++;
				break;
			}
		}
		fputc(tabula_recta[row][column], output_file); // Zapisujemy szyfr do pliku
		z++;
		printf("%c", tabula_recta[row][column]);
	} while (z < strlen(message) - sizeof(int));

	printf("\n\nOdszyfrowanie wiadomosci: \n");
	rewind(keystream_file);
	rewind(output_file);
	int calc = 0, key_value = 0, cipher_value = 0;
	k = 0;
	/* Odszyfrowanie polega na odjêciu wartoœci INT klucza od szyfru. Ka¿da litera ma
	swoj¹ pozycjê w tabeli (np. D ma wartoœæ 3). Je¿eli ró¿nica wychodzi ujemna dodajemy
	liczbê 26. Otrzymana liczba odpowiada danej literze w pierwszym pierszu, czyli literze
	naszej wiadomoœci */

	// ODSZYFROWANIE //
	do
	{

		x = fgetc(keystream_file);
		ch = fgetc(output_file);
		for (j = 0; j <= 26; j++)
		{
			if (ch == tabula_recta[0][j])
			{

				cipher_value = j;
				break;
			}
		}
		for (i = 0; i <= 26; i++)
		{
			if (x == tabula_recta[0][i])
			{
				key_value = i;
				break;
			}
		}
		calc = cipher_value - key_value;
		if (calc < 0)
		{
			calc = calc + 26;
			printf("%c", tabula_recta[0][calc]);

		}
		else
		{
			printf("%c", tabula_recta[0][calc]);
		}
		k++;
	} while (k < length);
}

int main()
{
	int *option = 0;
	int t = 0;
	do
	{
		
		printf("1: Szyfr Cezara\n");
		printf("2: Homofoniczny szyfr podstawieniowy\n");
		printf("3: Wieloalfabetowy szyfr podstawieniowy\n");
		printf("4: Zakoncz\n");
		printf("Wybierz jedna z powyzszych operacji: ");
		scanf("%d", &option);
		if (option == 1)
		{
			printf("\n");
			printf("Podaj przesuniecie: ");
			scanf("%d", &t);
			caesar(t);
			printf("\n");
		}
		else if (option == 2)
		{
			printf("\n");
			homophonic_cipher();
			printf("\n");
		}
		else if (option == 3)
		{
			printf("\n");
			running_key_cipher();
			printf("\n");
		}
		printf("Wybrano nieprawidlowa opcje! Powrotc do menu...\n");
	} while (option != 4);
	printf("\n");
	printf("\Wybrano wyjscie z programu...\n");
}