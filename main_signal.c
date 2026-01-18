// #include "lexer.h"

// #include <readline/readline.h>
// #include <readline/history.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>


// // t_minishell g_minishell;

// // int	main(int argc, char **argv, char **envp)
// // {
// // 	(void)argc;
// // 	(void)argv;
// // 	g_minishell.environ = envp;
// // 	g_minishell.exit_s = 0;

// // 	while (1)
// // 	{
// // 		ft_init_signals();
// // 		char *line = readline("minishell> ");

// // 		if (!line)
// // 		{
// // 			write(1, "exit\n", 5);
// // 			break;
// // 		}
// // 		if (*line)
// // 			add_history(line);

// // 		// Simulate command
// // 		if (!strcmp(line, "test"))
// // 			printf("You typed 'test'.\n");

// // 		free(line);
// // 	}
// // 	return (0);
// // }
// // main.c

// #include <readline/readline.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/wait.h>

// t_minishell g_minishell;

// void	test_execve(const char *cmd)
// {
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		ft_reset_signals_in_child();
// 		char *args[] = {(char *)cmd, NULL};
// 		execve(cmd, args, g_minishell.environ);
// 		perror("execve");
// 		exit(1);
// 	}
// 	else
// 	{
// 		g_minishell.signint_child = true;
// 		int status;
// 		waitpid(pid, &status, 0);
// 		g_minishell.signint_child = false;
// 		printf("child exited with status: %d\n", status);
// 	}
// }

// void	test_heredoc(const char *delim)
// {
// 	int	pipefd[2];
// 	pipe(pipefd);
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		ft_reset_signals_in_child();
// 		close(pipefd[0]);
// 		char *line;
// 		while (1)
// 		{
// 			line = readline("> ");
// 			if (!line || strcmp(line, delim) == 0)
// 				break ;
// 			write(pipefd[1], line, strlen(line));
// 			write(pipefd[1], "\n", 1);
// 			free(line);
// 		}
// 		free(line);
// 		exit(0);
// 	}
// 	else
// 	{
// 		g_minishell.signint_child = true;
// 		close(pipefd[1]);
// 		int status;
// 		waitpid(pid, &status, 0);
// 		g_minishell.signint_child = false;
// 		char buffer[1024];
// 		int n = read(pipefd[0], buffer, 1023);
// 		if (n > 0)
// 		{
// 			buffer[n] = '\0';
// 			printf("[HEREDOC CONTENT]:\n%s\n", buffer);
// 		}
// 		close(pipefd[0]);
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	g_minishell.environ = envp;
// 	g_minishell.exit_s = 0;

// 	while (1)
// 	{
// 		ft_init_signals();

// 		char *line = readline("minishell> ");
// 		if (!line)
// 		{
// 			write(1, "exit\n", 5);
// 			break;
// 		}
// 		if (*line)
// 			add_history(line);

// 		if (!strcmp(line, "exit"))
// 			break ;
// 		else if (!strcmp(line, "cat"))
// 			test_execve("/bin/cat");
// 		else if (!strcmp(line, "heredoc"))
// 			test_heredoc("EOF");
// 		else
// 			printf("Unknown command: '%s'\n", line);
// 		free(line);
// 	}
// 	return (0);
// }
