#include "shell.h"

/**
 * check_for_builtins - Entry Point
 *
 * @var: variables
 *
 * Description: Checks if the command is a builtin
 *
 * Return: pointer to the function or NULL
 */

void (*check_for_builtins(t_var * var))(t_var * var)
{
	unsigned int i;
	t_builtins check[] = {
		{"exit", new_exit},
		{"env", _env},
		{"setenv", new_setenv},
		{"unsetenv", new_unsetenv},
		{NULL, NULL}
	};

	for (i = 0; check[i].f != NULL; i++)
	{
		if (_strcmpr(var->av[0], check[i].name) == 0)
			break;
	}
	if (check[i].f != NULL)
		check[i].f(var);
	return (check[i].f);
}

/**
 * new_exit - Entry Point
 *
 * @var: variables
 *
 * Description: exit program
 *
 * Return: void
 */

void new_exit(t_var *var)
{
	int status;

	if (_strcmpr(var->av[0], "exit") == 0 && var->av[1] != NULL)
	{
		status = _atoi(var->av[1]);
		if (status == -1)
		{
			var->ext_status = 2;
			print_error(var, ": Illegal number: ");
			_puts2(var->av[1]);
			_puts2("\n");
			free(var->commands);
			var->commands = NULL;
			return;
		}
		var->ext_status = status;
	}
	free(var->buffer);
	free(var->av);
	free(var->commands);
	free_env(var->env);
	exit(var->ext_status);
}

/**
 * _env - Entry Point
 *
 * @var: struct of variables
 *
 * Description: prints the current environment
 *
 * Return: void.
 */

void _env(t_var *var)
{
	unsigned int i;

	for (i = 0; var->env[i]; i++)
	{
		_puts(var->env[i]);
		_puts("\n");
	}
	var->ext_status = 0;
}

/**
 * new_setenv - Entry Point
 *
 * @var: pointer to struct of variables
 *
 * Description: create a new or edit an existing environment variable.
 *
 * Return: void
 */

void new_setenv(t_var *var)
{
	char **key;
	char *env_var;

	if (var->av[1] == NULL || var->av[2] == NULL)
	{
		print_error(var, ": Incorrect number of arguments\n");
		var->ext_status = 2;
		return;
	}
	key = find_key(var->env, var->av[1]);
	if (key == NULL)
		add_key(var);
	else
	{
		env_var = add_value(var->av[1], var->av[2]);
		if (env_var == NULL)
		{
			print_error(var, NULL);
			free(var->buffer);
			free(var->commands);
			free(var->av);
			free_env(var->env);
			exit(127);
		}
		free(*key);
		*key = env_var;
	}
	var->ext_status = 0;
}

/**
 * new_unsetenv - Entry Point
 *
 * @var: pointer to a struct of variables
 *
 * Description: remove an environment variable
 *
 * Return: void
 */

void new_unsetenv(t_var *var)
{
	char **key, **newenv;

	unsigned int i, j;

	if (var->av[1] == NULL)
	{
		print_error(var, ": Incorrect number of arguments\n");
		var->ext_status = 2;
		return;
	}
	key = find_key(var->env, var->av[1]);
	if (key == NULL)
	{
		print_error(var, ": No variable to unset");
		return;
	}
	for (i = 0; var->env[i] != NULL; i++)
		;
	newenv = malloc(sizeof(char *) * i);
	if (newenv == NULL)
	{
		print_error(var, NULL);
		var->ext_status = 127;
		new_exit(var);
	}
	for (i = 0; var->env[i] != *key; i++)
		newenv[i] = var->env[i];
	for (j = i + 1; var->env[j] != NULL; j++, i++)
		newenv[i] = var->env[j];
	newenv[i] = NULL;
	free(*key);
	free(var->env);
	var->env = newenv;
	var->ext_status = 0;
}
