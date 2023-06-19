#include "shell.h"

/**
 * path_execute - Entry Point
 *
 * @command: full path to the command
 * @var: pointer to struct of variables
 *
 * Description: Executes a command in the path
 *
 * Return: 0 (succcess), 1 (failure)
 */

int path_execute(char *command, t_var *var)
{
	pid_t child_pid;

	if (access(command, X_OK) == 0)
	{
		child_pid = fork();
		if (child_pid == -1)
			print_error(var, NULL);
		if (child_pid == 0)
		{
			if (execve(command, var->av, var->env) == -1)
				print_error(var, NULL);
		}
		else
		{
			wait(&var->ext_status);
			if (WIFEXITED(var->ext_status))
				var->ext_status = WEXITSTATUS(var->ext_status);
			else if (WIFSIGNALED(var->ext_status)
				 && WTERMSIG(var->ext_status) == SIGINT)
				var->ext_status = 130;
			return (0);
		}
		var->ext_status = 127;
		return (1);
	}
	else
	{
		print_error(var, ": Permission denied\n");
		var->ext_status = 126;
	}
	return (0);
}

/**
 * find_path - Entry Point
 *
 * @env: array of environment variables
 *
 * Description: finds the PATH variable
 *
 * Return: pointer to the node that contains the PATH, or NULL on failure
 */

char *find_path(char **env)
{
	char *path = "PATH=";
	unsigned int i, j;

	for (i = 0; env[i] != NULL; i++)
	{
		for (j = 0; j < 5; j++)
			if (path[j] != env[i][j])
				break;
		if (j == 5)
			break;
	}
	return (env[i]);

}

/**
 * check_for_path - Entry Point
 *
 * @var: variables
 *
 * Description: checks if the command is in the PATH
 *
 * Return: void
 */

void check_for_path(t_var *var)
{
	char *path, *path_dup = NULL, *check = NULL;
	unsigned int i = 0, r = 0;
	char **path_tokens;
	struct stat buf;

	if (check_for_dir(var->av[0]))
		r = execute_cwd(var);
	else
	{
		path = find_path(var->env);
		if (path != NULL)
		{
			path_dup = _strdup(path + 5);
			path_tokens = tokenize(path_dup, ":");
			for (i = 0; path_tokens && path_tokens[i]; i++, free(check))
			{
				check = _strcat(path_tokens[i], var->av[0]);
				if (stat(check, &buf) == 0)
				{
					r = path_execute(check, var);
					free(check);
					break;
				}
			}
			free(path_dup);
			if (path_tokens == NULL)
			{
				var->ext_status = 127;
				new_exit(var);
			}
		}
		if (path == NULL || path_tokens[i] == NULL)
		{
			print_error(var, ": not found\n");
			var->ext_status = 127;
		}
		free(path_tokens);
	}
	if (r == 1)
		new_exit(var);
}

/**
 * execute_cwd - Entry Point
 *
 * @var: pointer to struct of variables
 *
 * Description: Executes the command in the current working directory
 *
 * Return: 0 on success, 1 on failure
 */

int execute_cwd(t_var *var)
{
	pid_t child_pid;
	struct stat buf;

	if (stat(var->av[0], &buf) == 0)
	{
		if (access(var->av[0], X_OK) == 0)
		{
			child_pid = fork();
			if (child_pid == -1)
				print_error(var, NULL);
			if (child_pid == 0)
			{
				if (execve(var->av[0], var->av, var->env) == -1)
					print_error(var, NULL);
			}
			else
			{
				wait(&var->ext_status);
				if (WIFEXITED(var->ext_status))
					var->ext_status = WEXITSTATUS(var->ext_status);
				else if (WIFSIGNALED(var->ext_status)
					 && WTERMSIG(var->ext_status) == SIGINT)
					var->ext_status = 130;
				return (0);
			}
			var->ext_status = 127;
			return (1);
		}
		else
		{
			print_error(var, ": Permission denied\n");
			var->ext_status = 126;
		}
		return (0);
	}
	print_error(var, ": not found\n");
	var->ext_status = 127;
	return (0);
}

/**
 * check_for_dir - Entry Point
 *
 * @str: command
 *
 * Description: Checks if the command is located in a path
 *
 * Return: 1(success), 0 (failure)
 */

int check_for_dir(char *str)
{
	unsigned int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '/')
			return (1);
	}
	return (0);
}
