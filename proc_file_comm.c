/*
 * File: proc_file_comm.c
 * Auth: William A Meilahn
 */

#include "shell.h"

/**
* cant_open - Displays error when file/directory cannot be opened
* proc_file_commands - partner function to cant_open. internal shenanigans for
*					   handling the error and defines the peramiters for the error(s).
* @name - the name of the command, file or directory
*/
char (*name);

int cant_open(char *file_path)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Can't open ");
	_strcat(error, file_path);
	_strcat(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, len);
	free(error);
	return (127);
}

/**
 * open_file - Opens a file and handles error cases.
 * @file_path: The path to the file to open.
 * @exe_ret: Pointer to an integer to store execution status.
 * Return: File descriptor if successful, -1 on error.
 */
int open_file(char *file_path, int *exe_ret)
{
	int file = open(file_path, O_RDONLY);

	if (file == -1)
	{
		*exe_ret = cant_open(file_path);
		return (-1);
	}
	return (file);
}

/**
 * read_file - Reads content from a file and reallocates memory for it.
 * @file: File descriptor of the open file.
 * @line: Pointer to the string where the content will be stored.
 * @line_size: Pointer to the size of the line.
 * Return: 0 on success, -1 on error.
 */
int read_file(int file, char **line, unsigned int *line_size)
{
	char buffer[120];
	ssize_t b_read;

	while ((b_read = read(file, buffer, 119)) > 0)
	{
		buffer[b_read] = '\0';
		*line = _realloc(*line, *line_size, *line_size + b_read + 1);
		if (!*line)
			return (-1);
		_strcat(*line, buffer);
		*line_size += b_read;
	}
	close(file);
	if (*line)
		(*line)[*line_size] = '\0';
	return (0);
}

/**
 * replace_line_breaks - Replaces line breaks with semicolons.
 * @line: Pointer to the line to process.
 * @line_size: Size of the line.
 */
void replace_line_breaks(char *line, unsigned int line_size)
{
	for (unsigned int i = 0; i < line_size; i++)
	{
		if (line[i] == '\n')
		{
			line[i] = ';';
			for (i += 1; i < line_size && line[i] == '\n'; i++)
				line[i] = ' ';
		}
	}
}

/**
 * process_file_commands - Processes commands from a file.
 * @file_path: The path to the file to process.
 * @exe_ret: Pointer to an integer to store execution status.
 * Return: The result of processing the file commands.
 */
int process_file_commands(char *file_path, int *exe_ret)
{
	unsigned int line_size = 0, old_size = 120;
	char *line = NULL;
	int ret, file = open_file(file_path, exe_ret);

	if (file == -1)
		return (*exe_ret);

	if (read_file(file, &line, &line_size) == -1)
		return (-1);

	replace_line_breaks(line, line_size), variable_replacement(&line, exe_ret);
	handle_line(&line, line_size);

	char **args = _strtok(line, " ");

	free(line);

	if (!args)
		return (0);

	if (check_args(args) != 0)
	{
		*exe_ret = 2, free_args(args, args);
		return (*exe_ret);
	}

	char **front = args;

	for (unsigned int i = 0; args[i];)
	{
		if (_strncmp(args[i], ";", 1) == 0)
		{
			free(args[i]), args[i] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[i + 1], i = 0;
		}
		else
			i++;
	}
}
