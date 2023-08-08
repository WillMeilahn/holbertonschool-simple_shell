/*
 * File: split.c
 * Auth: William A Meilahn
 */

#include "shell.h"

int token_len(char *str, char *delim);
int count_tokens(char *str, char *delim);
char **_strtok(char *line, char *delim);

/**
 * token_len - Locates the delimiter index marking the end
 *             of the first token contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The delimiter index marking the end of
 *         the initial token pointed to by str.
 */
int token_len(char *str, char *delim)
{
	int len = 0;

	while (str[len] != '\0' && str[len] != *delim)
		len++;

	return (len);
}

/**
 * count_tokens - Counts the number of delimited
 *                words contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The number of words contained within str.
 */
int count_tokens(char *str, char *delim)
{
	int tokens = 0;
	int index = 0;

	while (str[index] != '\0')
	{
		if (str[index] != *delim)
		{
			tokens++;
			index += token_len(str + index, delim);
		}
		else
		{
			index++;
		}
	}

	return (tokens);
}

/**
 * _strtok - Tokenizes a string.
 * @line: The string.
 * @delim: The delimiter character to tokenize the string by.
 *
 * Return: A pointer to an array containing the tokenized words.
 */
char **_strtok(char *line, char *delim)
{
	if (line == NULL)
		return (NULL);

	int tokens = count_tokens(line, delim);
	if (tokens == 0)
		return (NULL);

	char **ptr = malloc(sizeof(char *) * (tokens + 1));
	if (!ptr)
		return (NULL);

	int t = 0;
	int index = 0;

	while (*line)
	{
		if (*line != *delim)
		{
			int letters = token_len(line, delim);
			ptr[t] = malloc(sizeof(char) * (letters + 1));
			if (!ptr[t])
			{
				for (int i = 0; i < t; i++)
					free(ptr[i]);
				free(ptr);
				return (NULL);
			}

			for (int l = 0; l < letters; l++)
			{
				ptr[t][l] = line[index];
				index++;
			}
			ptr[t][letters] = '\0';
			t++;
		}
		else
		{
			line++;
			index++;
		}
	}

	ptr[t] = NULL;
	return (ptr);
}
