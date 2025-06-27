#include "lexer.h"
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

extern t_minishell g_minishell;

/**
 * SIGINT handler — when Ctrl-C is pressed.
 * Behavior changes depending on whether you're in prompt or executing a command.
 */
void	ft_sigint_handler(int signum)
{
	(void)signum;
	if (g_minishell.signint_child)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_minishell.signint_child = false;
		g_minishell.heredoc_sigint = true;
	}
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);    // Clear current input
		rl_on_new_line();          // Move to next line
		rl_redisplay();            // Redraw prompt
	}
}

/**
 * This function is called at the beginning of each readline loop
 * It prepares the terminal settings and signal handlers.
 */
void	ft_init_signals(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);           // Get current termios settings
	// term.c_lflag &= ~ECHOCTL;                 // Hide ^C and ^\ characters
    term.c_lflag |= ECHOCTL;                    // Show ^C and ^\ characters
	tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply modified termios

	signal(SIGINT, ft_sigint_handler);        // Custom handler for Ctrl-C
	signal(SIGQUIT, SIG_IGN);                 // Ignore Ctrl-
	g_minishell.heredoc_sigint = false;
	g_minishell.signint_child = false;
}

/**
 * This should be called inside child processes right before execve
 * so Ctrl-C and Ctrl-\ behave like in Bash.
 */
void	ft_reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);    // Reset Ctrl-C to kill child
	signal(SIGQUIT, SIG_DFL);   // Reset Ctrl-\ to terminate with "Quit: 3"
}
