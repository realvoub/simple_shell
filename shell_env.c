#include "shell.h"

/**
 * make_env - Entry Point
 *
 * @env: The environment passed to main
 *
 * Description: make the shell environment from the environment passed to main
 *
 * Return: pointer to the new environment
 */

char **make_env(char **env)
{
	char **newenv = NULL;
	size_t i;

	for (i = 0; env[i] != NULL; i++)
		;
	newenv = malloc(sizeof(char *) * (i + 1));
	if (newenv == NULL)
	{
		perror("Fatal Error");
		exit(1);
	}
	for (i = 0; env[i] != NULL; i++)
		newenv[i] = _strdup(env[i]);
	newenv[i] = NULL;
	return (newenv);
}

/**
 * free_env - Entry Point
 *
 * @env: The shell's environment
 *
 * Description: free the shell's environment
 *
 * Return: void
 */

void free_env(char **env)
{
	unsigned int i;

	for (i = 0; env[i] != NULL; i++)
		free(env[i]);
	free(env);
}
