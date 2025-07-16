/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:32:44 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 13:32:48 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <lexer.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>

extern volatile sig_atomic_t g_signal_received;

typedef enum e_err
{
	ERR_NONE,
	INV_TOKEN,
	SYSCALL_FAIL,
	CMD_ENOENT,
	AMBIG_REDIR,
	IS_DIR,
	OPEN_FAIL,
	FILE_ENOENT
}	t_err;

typedef struct s_err
{
	t_err	code;
	void	*data;
}	t_error;

typedef struct s_env
{
	struct s_env	*next;
	char			*key;
	char			*value;
}	t_env;

typedef struct s_shell
{
	t_cmd			*pipeline;
	t_env			*env;
	int				status;
	char			*chached_pwd;
	int				sig;
	struct termios	orig_termios;
}	t_shell;

typedef void	(*t_error_handler)(char *data);
typedef int		(*t_builtin)(char **args);

extern t_shell	g_shell;

t_list	*add_node(t_list **head, void *value);
void	free_list(t_list **head);
int		list_len(t_list *list);
int		fill_var(char *str, t_list **list, int *expanded);
char	*make_str(t_list *list);
char	*quotes_expand(char *str, int *expanded);
int		my_open(char *path, int flags);
int		my_open_builtin(char *path, int flags);
char	*mkfilename(char *path);
char	*do_heredoc(char *eof, int expand);
void	redir(t_redir *redir, int size);
int		redir_builtin(t_redir *redir, int size);
char	*mkpath(char *path, char *cmd);
char	*check_cwd(char *cmd, t_env *env);
char	*check_path(char *path);
char	*which(char *cmd, t_env *env);
void	exec(char **args);
int		cmd(t_cmd *cmd);
void	cmd_pipe(t_cmd *cmd);
t_env	*init_env(char **envp);
char	*mkvar(t_env *env);
char	**mkenvp(t_env *env, int *envsize);
t_env	*add_node_env(t_env **head, char *name, char *value);
char	*get_env_val(t_env *env, char *key);
t_env	*get_env(t_env *env, char *key);
int		env_len(t_env *env);
t_env	*update_env(t_env **env, char *key, char *value);
void	free_node(t_env **env, t_env *node);
int		env(char **args);
int		_isalpha(int c);
int		_isalnum(int c);
int		validate_id(char *name);
int		export_add(char **args);
int		export(char **args);
int		check_builtins(t_cmd *cmd);
int		echo(char **args);
int		unset(char **args);
int		__exit(char **args);
int		pwd(char **args);
int		_chdir(char *dir);
int		cd(char **args);
char	*max_str(char *a, char *b);
char	*_strtok(char *str, char *delims);
char	*_strchr(char *str, char c);
char	*_strdup(char *src);
int		_strlen(char *str);
int		_strncmp(char *s1, char *s2, unsigned int n);
char	*_strddup(char *str, char *set);
int		_strcmp(char *s1, char *s2);
void	throw_err(t_err code, char *data);
void	unexpected_token(char *data);
void	syscall_fail(char *data);
void	ambig_redir(char *data);
void	cmd_enoent(char *data);
void	is_dir(char *data);
int		start(t_cmd *pipeline);
void	init_shell(char **envp);
char	*_itoa(int n);
int		expand_status(t_list **list);
void	free_arr(char **arr, int size);
void	free_pipeline(t_cmd *pipeline);
void	free_env(t_env *env);
void	open_fail(char *data);
void	file_enoent(char *data);
void	cleanup(int n);
// void	foo(int sig);
void	reset_readline_state(void);
t_shell	*data(void);

#endif
