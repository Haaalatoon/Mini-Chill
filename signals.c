#include <lexer.h>
#include <main.h>

// SIGINT handler — when Ctrl-C is pressed.
void	ft_sigint_handler(int signum)
{
	(void)signum;
	if (g_shell.in_execution)
	{
		write(1, "\n", 1);
		g_shell.in_execution = false;
		g_shell.heredoc_sigint = true;
	}
	else
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);    // Clear current input
		rl_on_new_line();          // Move to next line
		rl_redisplay();            // Redraw prompt
	}
}

// SIGQUIT handler — when Ctrl-\ is pressed.
void	ft_sigquit_handler(int num)
{
	(void)num;
	ft_putstr_fd("Quit: 3\n", 1);
}

 /** This function is called at the beginning of each readline loop
 * It prepares the terminal settings and signal handlers.
 */
void	ft_init_signals(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);           // Get current termios settings
	// term.c_lflag &= ~ECHOCTL;                 // Hide ^C and ^\ characters
    term.c_lflag |= ECHOCTL;                    // Show ^C and ^\ characters
	tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply modified termios

	g_shell.heredoc_sigint = false;
	g_shell.in_execution = false;

	signal(SIGINT, ft_sigint_handler);        // Custom handler for Ctrl-C
	signal(SIGQUIT, SIG_IGN);                 // Ignore Ctrl-

}
