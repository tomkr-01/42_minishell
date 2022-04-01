/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:26:26 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:26:27 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <signal.h>
#include <termios.h>

void	heredoc_signals(int sig)
{
	if (sig == SIGINT)
	{
		close(STDIN_FILENO);
		write(2, "\n", 1);
	}
}

void	control_c(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	change_attributes(bool print_controls)
{
	int				err;
	struct termios	termios;

	err = tcgetattr(STDOUT_FILENO, &termios);
	if (err == -1)
		return (-1);
	if (print_controls)
		termios.c_lflag |= ECHOCTL;
	else
		termios.c_lflag &= ~(ECHOCTL);
	err = tcsetattr(STDOUT_FILENO, TCSANOW, &termios);
	if (err == -1)
		return (-1);
	return (0);
}
