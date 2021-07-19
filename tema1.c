#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
// consider lungimea maxima a unui sir
// pe care il pot intalni sa fie 100
#define LEN 100
// cand trebuie sa afisez un caracter
// voi avea nevoie de un sir de dimensiunea 2
#define SMALL 2

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

static void char_reverse(char string1[LEN], char string2[LEN])
{
	int k = 0;
	while (k != strlen(string1))
	{
		string2[k] = string1[strlen(string1) - 1 - k];
		k++;
	}
	string2[strlen(string1)] = '\0';
}

static void dec_to_string(int number, char string[LEN])
{
	int copy = number;
	int i;
	if (number < 0)
	{
		number = -number;
	}
	if (copy > -2147483647)
	{
		for (i = 0; number != 0; i++)
		{
			int digit = number % 10;
			number /= 10;
			string[i] = digit + '0';
		}
		if (copy < 0)
		{
			string[i++] = '-';
		}
		string[i++] = '\0';
	}
	else
	{
		string[0] = '\0';
		strcat(string, "8463847412-");
	}
}

static void hex_to_string(unsigned int number, char string[LEN])
{
	int i;
	for (i = 0; number != 0; i++)
	{
		int digit = number % 16;
		number /= 16;
		if (digit <= 9)
		{
			string[i] = digit + '0';
		}
		else
		{
			// pentru ca in baza 10 este echivalentul lui a s.a.m.d
			string[i] = 'a' + (digit - 10);
		}
	}
	string[i++] = '\0';
}

static void unint_to_string(unsigned int number, char string[LEN])
{
	int i;
	for (i = 0; number != 0; i++)
	{
		int digit = number % 10;
		number /= 10;
		string[i] = digit + '0';
	}
	string[i++] = '\0';
}

int iocla_printf(const char *format, ...)
{
	int x = 0;	// numar caracterele afisate 
	va_list arg;
	va_start(arg, format);
	int length = strlen(format);
	int ok = 1;
	// cu acest ok verific daca pe ultimele
	// 2 pozitii am specificatori de conversie
	for (int i = 0; i < length; i++)
	{
		if (format[i] == '%' && ok)
		{
			if (format[i + 1] == 's')
			{
				char *buffer = va_arg(arg, char *);
				//nu trebuie facuta nicio conversie 
				x += write_stdout(buffer, strlen(buffer));
			}
			if (format[i + 1] == 'c')
			{
			 	// se va face cast automat 
				char c_buf = va_arg(arg, int);
				char workspace[SMALL];
				//creez un sir din caracterul dorit si terminator
				//pentru a putea folosi functia write_stdout
				workspace[0] = c_buf;
				workspace[1] = '\0';
				x += write_stdout(workspace, strlen(workspace));
			}
			if (format[i + 1] == 'd')
			{
				int int_buf = va_arg(arg, int);
				char workspace[LEN];
				char reversed[LEN];
				dec_to_string(int_buf, workspace);
				char_reverse(workspace, reversed);
				x += write_stdout(reversed, strlen(reversed));
			}
			if (format[i + 1] == 'x')
			{
				unsigned int hex_buf = va_arg(arg, unsigned int);
				char workspace[LEN];
				char reversed[LEN];
				hex_to_string(hex_buf, workspace);
				char_reverse(workspace, reversed);
				x += write_stdout(reversed, strlen(reversed));
			}
			if (format[i + 1] == 'u')
			{
				unsigned int uint_buf = va_arg(arg, unsigned int);
				char workspace[LEN];
				char reversed[LEN];
				unint_to_string(uint_buf, workspace);
				char_reverse(workspace, reversed);
				x += write_stdout(reversed, strlen(reversed));
			}
			if (format[i + 1] == '%')
			{
				char work[SMALL];
				work[0] = '%';
				work[1] = '\0';
				x += write_stdout(work, strlen(work));
			}
			//pentru ca verific si ce urmeaza dupa primul
			// caracter de tip %
			i++;
		}
		else if (format[i] == '\n')
		{
			char work[SMALL];
			work[0] = '\n';
			work[1] = '\0';
			x += write_stdout(work, strlen(work));
		}
		else if (format[i] == '\t')
		{
			char work[SMALL];
			work[0] = '\t';
			work[1] = '\0';
			x += write_stdout(work, strlen(work));
		}
		else
		{
			char work[SMALL];
			work[0] = format[i];
			work[1] = '\0';
			x += write_stdout(work, strlen(work));
		}
	}
	//pentru ca am parcurs lista de argumente
	va_end(arg);
	return x;
}