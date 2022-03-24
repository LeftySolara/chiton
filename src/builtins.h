/**
 * @file builtins.h
 * @author Julianne Adams <julianne@julianneadams.info>
 * @brief Built-in commands for the Chiton shell
 * @version 0.1
 * @date 2022-03-24
 *
 * @copyright MIT License
 *
 */

#ifndef BUILTINS_H
#define BUILTINS_H

extern char *builtin_str[];
extern int (*builtin_func[])(char **);

int chiton_cd(char **args);
int chiton_echo(char **args);
int chiton_help(char **args);
int chiton_exit(char **args);

int chiton_num_builtins();
char *chiton_find_home();

#endif /* BUILTINS_H */