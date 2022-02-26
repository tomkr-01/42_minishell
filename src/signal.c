#include "../inc/minishell.h"

void	control_c(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

/* main thing to change:
	1. when typing in a command control c should only display the prompt again without anything
	2. while running a command the opposite should happen
	this is managed through the flag ECHOCTL, which if set returns a control character.
	it is a bit representation of the number 64.
*/

int	change_attributes(bool state)
{
	int					err;
	struct termios		termios;

	err = tcgetattr(1, &termios);
	if (err == -1)
		return (-1);
	printf("this is the value of c_lflag %u\n", termios.c_lflag);
	/* ECHO und ECHOCTL on consist of one bit being a 1 and the rest being 0's
	to turn on option 'on', you use the bitwise or operator, as all the other
	flags stay the same while the one you want is turned to a 1 */
	if (state)
		termios.c_lflag |= ECHOCTL;
	/* the ~ operator changes 0's to 1's and 1's to 0's together with the and operator
	everything that is a 1 will stay a 1 and the bit we want is turned to a 0 */
	else
		termios.c_lflag &= ~(ECHOCTL);
	err = tcsetattr(1, TCSANOW, &termios);
	if (err == -1)
		return (-1);
	return (0);
}
