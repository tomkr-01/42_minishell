#include "../inc/minishell.h"

int	echo_builtin(char **arguments)
{
	size_t	i;
	size_t	j;
	int		new_line;

	i = 0;
	new_line = 1;
	if (arguments == NULL)
		return (ft_putstr_fd("\n", STDOUT_FILENO));
	while (arguments[i] != NULL)
	{
		j = 0;
		while (arguments[i][0] == '-' && arguments[i][++j] == 'n')
			;
		if (arguments[i][j] == '\0' && new_line > 0 && j > 0)
			new_line = 2;
		else
		{
			if (new_line > 0)
				new_line *= -1;
			ft_putstr_fd(arguments[i], STDOUT_FILENO);
			if (arguments[i + 1] != NULL)
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
		i++;
	}
	if (-2 < new_line && new_line < 2)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
