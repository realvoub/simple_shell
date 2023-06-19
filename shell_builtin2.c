#include "shell.h"

/**
 * add_key - Entry Point
 *
 * @var: pointer to struct of variables
 *
 * Description: create a new environment variable
 *
 * Return: void
 */

void add_key(t_var *var)
{
	unsigned int i;
	char **newenv;

	for (i = 0; var->env[i] != NULL; i++)
		;
	newenv = malloc(sizeof(char *) * (i + 2));
	if (newenv == NULL)
	{
		print_error(var, NULL);
		var->ext_status = 127;
		new_exit(var);
	}
	for (i = 0; var->env[i] != NULL; i++)
		newenv[i] = var->env[i];
	newenv[i] = add_value(var->av[1], var->av[2]);
	if (newenv[i] == NULL)
	{
		print_error(var, NULL);
		free(var->buffer);
		free(var->commands);
		free(var->av);
		free_env(var->env);
		free(newenv);
		exit(127);
	}
	newenv[i + 1] = NULL;
	free(var->env);
	var->env = newenv;
}

/**
 * find_key - Entry Point
 *
 * @env: array of environment variables
 * @key: environment variable to find
 *
 * Description: finds an environment variable
 *
 * Return: pointer to address of the environment variable
 */

char **find_key(char **env, char *key)
{
	unsigned int i, j, len;

	len = _strlen(key);
	for (i = 0; env[i] != NULL; i++)
	{
		for (j = 0; j < len; j++)
			if (key[j] != env[i][j])
				break;
		if (j == len && env[i][j] == '=')
			return (&env[i]);
	}
	return (NULL);
}

/**
 * add_value - Entry Point
 *
 * @key: variable name
 * @value: variable value
 *
 * Description: creates a new environment variable string
 *
 * Return: pointer to the new string;
 */

char *add_value(char *key, char *value)
{
	unsigned int len1, len2, i, j;
	char *new;

	len1 = _strlen(key);
	len2 = _strlen(value);
	new = malloc(sizeof(char) * (len1 + len2 + 2));
	if (new == NULL)
		return (NULL);
	for (i = 0; key[i] != '\0'; i++)
		new[i] = key[i];
	new[i] = '=';
	for (j = 0; value[j] != '\0'; j++)
		new[i + 1 + j] = value[j];
	new[i + 1 + j] = '\0';
	return (new);
}

/**
 * _atoi - Entry Point
 *
 * @str: string to convert
 *
 * Description: Converts a string into an integer
 *
 * Return: the integer value, or -1 if an error occurs
 */

int _atoi(char *str)
{
	unsigned int i, digits;
	int num = 0, num_test;

	num_test = INT_MAX;
	for (digits = 0; num_test != 0; digits++)
		num_test /= 10;
	for (i = 0; str[i] != '\0' && i < digits; i++)
	{
		num *= 10;
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if ((i == digits - 1) && (str[i] - '0' > INT_MAX % 10))
			return (-1);
		num += str[i] - '0';
		if ((i == digits - 2) && (str[i + 1] != '\0') && (num > INT_MAX / 10))
			return (-1);
	}
	if (i > digits)
		return (-1);
	return (num);
}
