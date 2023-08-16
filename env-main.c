#include <stdio.h>

ac = argument count
av = an array of string pointers containing command line arguments
env = the environment to be printed
/**
 * main - prints the environment
 *
 * ac: argument count
 * av: an array of string pointers containing command line arguments
 * env: the environment to be printed
 *
 * Return: Always 0.
 */
int main(int ac, char **av, char **env)
{
	unsigned int i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
