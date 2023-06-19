#include "shell.h"

/**
 * print_error - Entry Point
 *
 * @var: pointer to struct of variables
 * @msg: message to print
 *
 * Description: prints error messages to standard error
 *
 * Return: void
 */

void print_error(t_var *var, char *msg)
{
	char *count;

	_puts2(var->argv[0]);
	_puts2(": ");
	count = _uitoa(var->com_count);
	_puts2(count);
	free(count);
	_puts2(": ");
	_puts2(var->av[0]);
	if (msg)
	{
		_puts2(msg);
	}
	else
		perror("");
}

/**
 * _puts2 - Entry Point
 *
 * @str: string to print
 *
 * Description: prints a string to standard error
 *
 * Return: void
 */

void _puts2(char *str)
{
	ssize_t num, len;

	num = _strlen(str);
	len = write(STDERR_FILENO, str, num);
	if (len != num)
	{
		perror("Fatal Error");
		exit(1);
	}

}

/**
 * _uitoa - Entry Point
 *
 * @count: unsigned int to convert
 *
 * Description: Converts an unsigned int to a string
 *
 * Return: pointer to the converted string
 */

char *_uitoa(unsigned int count)
{
	char *numstr;
	unsigned int tmp, digits;

	tmp = count;
	for (digits = 0; tmp != 0; digits++)
		tmp /= 10;
	numstr = malloc(sizeof(char) * (digits + 1));
	if (numstr == NULL)
	{
		perror("Fatal Error1");
		exit(127);
	}
	numstr[digits] = '\0';
	for (--digits; count; --digits)
	{
		numstr[digits] = (count % 10) + '0';
		count /= 10;
	}
	return (numstr);
}
