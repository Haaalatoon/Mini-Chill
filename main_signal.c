#include "lexer.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


t_minishell g_minishell;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	g_minishell.environ = envp;
	g_minishell.exit_s = 0;

	while (1)
	{
		ft_init_signals();
		char *line = readline("minishell> ");

		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*line)
			add_history(line);

		// Simulate command
		if (!strcmp(line, "test"))
			printf("You typed 'test'.\n");

		free(line);
	}
	return (0);
}
