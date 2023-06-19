#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

/**
 * struct variables - variables
 * @av: command line arguments
 * @buffer: buffer of command
 * @env: environment variables
 * @com_count: count of commands entered
 * @argv: arguments at opening of shell
 * @ext_status: exit status
 * @commands: commands to execute
 */

typedef struct variables
{
	char **av;
	char *buffer;
	char **env;
	size_t com_count;
	char **argv;
	int ext_status;
	char **commands;
} t_var;

/**
 * struct builtins - struct for the builtin functions
 * @name: name of builtin command
 * @f: function for corresponding builtin
 */

typedef struct builtins
{
	char *name;
	void (*f)(t_var *);
} t_builtins;



char **make_env(char **env);
void free_env(char **env);

ssize_t _puts(char *str);
char *_strdup(char *strtodup);
int _strcmpr(char *strcmp1, char *strcmp2);
char *_strcat(char *strc1, char *strc2);
unsigned int _strlen(char *str);

char **tokenize(char *buffer, char *delimiter);
char **_realloc(char **ptr, size_t *size);
char *new_strtok(char *str, const char *delim);

void (*check_for_builtins(t_var *var))(t_var *var);
void new_exit(t_var *var);
void _env(t_var *var);
void new_setenv(t_var *var);
void new_unsetenv(t_var *var);

void add_key(t_var *var);
char **find_key(char **env, char *key);
char *add_value(char *key, char *value);
int _atoi(char *str);

void check_for_path(t_var *var);
int path_execute(char *command, t_var *var);
char *find_path(char **env);
int execute_cwd(t_var *var);
int check_for_dir(char *str);

void print_error(t_var *var, char *msg);
void _puts2(char *str);
char *_uitoa(unsigned int count);

#endif /* _SHELL_H_ */
