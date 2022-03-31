#include "../inc/minishell.h"

int	export_builtin(char **arguments);

int	cd_builtin(char **arguments)
{
	char	**export_pwds;
	char	*home_dir;

	if (arguments[0] == NULL)
	{
		home_dir = get_var("HOME");
		chdir(home_dir);
		ft_free((void **)&home_dir);
	}
	else if (chdir(arguments[0]))
	{
		put_stderr(SHELL, "cd", arguments[0], strerror(ENOENT));
		return (EXIT_FAILURE);
	}
	export_pwds = ft_calloc(3, sizeof(*export_pwds));
	if (export_pwds == NULL)
		put_stderr(SHELL, "cd_builtin()", NULL, strerror(ENOMEM));
	if (export_pwds == NULL)
		exit(ENOMEM);
	export_pwds[0] = ft_strjoin_free(ft_strdup("OLDPWD="), get_var("PWD"));
	export_pwds[1] = ft_strjoin_free(ft_strdup("PWD="), getcwd(NULL, 0));
	export_pwds[2] = NULL;
	export_builtin(export_pwds);
	ft_free_array(&export_pwds);
	return (EXIT_SUCCESS);
}
