#include "../inc/minishell.h"

// This f() returns returns a malloced char ** with the added element and frees
// the old one
char	**add_array_element(char **old_arr, char *new_el)
{
	char	**new_arr;
	size_t	i;

	i = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr) + 2, sizeof(*new_arr));
	if (new_arr == NULL)
		exit(put_stderr(SHELL, "add_array_element()", NULL, "hallo"));
	while (old_arr != NULL && old_arr[i] != NULL)
	{
		new_arr[i] = old_arr[i];
		i++;
	}
	ft_free((void **)&old_arr);
	new_arr[i] = ft_strdup(new_el);
	new_arr[i + 1] = NULL;
	return (new_arr);
}

// This f() returns a malloced char ** with the removed element and frees the
// old one
char	**rm_array_element(char **old_arr, char	*old_el)
{
	char	**new_arr;
	size_t	new;
	size_t	old;

	new = 0;
	old = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr), sizeof(*new_arr));
	if (new_arr == NULL)
		exit(put_stderr(SHELL, "rm_array_element()", NULL, "hallo"));
	while (old_arr[old] != NULL)
	{
		if (old_arr[old] == old_el)
			old++;
		else
			new_arr[new++] = old_arr[old++];
	}
	new_arr[new] = NULL;
	ft_free((void **)&old_el);
	ft_free((void **)&old_arr);
	return (new_arr);
}

int	put_stderr(char	*shell_name, char *cmd, char *arg, char *message)
{
	if (shell_name != NULL)
	{
		ft_putstr_fd(shell_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg != NULL)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (message != NULL)
	{
		ft_putstr_fd(message, STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
	return (EXIT_FAILURE);
}

// we need this function to free as we want to free the tokens in the parser
// or at the end after an command was executed
void	del_content(void *ptr)
{
	ptr = NULL;
}
