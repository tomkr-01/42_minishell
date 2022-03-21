#include "../inc/minishell.h"

extern t_minishell g_msh;

int		operator_check(t_list *tokens);
int		quote_check(t_list *tokens);
bool	is_operator(t_list *token);

bool	syntax_check(t_list *tokens)
{
	if (operator_check(tokens) != 0 || quote_check(tokens) != 0)
	{
		g_msh.exit_code = 258;
		return (false);
	}
	return (true);
}

int	operator_check(t_list *tokens)
{
	int	prev_op;

	prev_op = 1;
	if (ft_strncmp(ft_lstlast(tokens)->content, "<", 1) == 0
		|| ft_strncmp(ft_lstlast(tokens)->content, ">", 1) == 0)
		return (put_stderr(SHELL, NULL, NULL,
				"syntax error near unexpected operator"));
	while (tokens != NULL)
	{
		if (is_operator(tokens))
		{
			if (ft_strlen(tokens->content) > 2 || prev_op == 2
				|| (ft_strncmp(tokens->content, "|", 1) == 0 && prev_op == 1))
				return (put_stderr(SHELL, NULL, NULL,
						"syntax error near unexpected token"));
			prev_op = 2;
			if (ft_strncmp(tokens->content, "|", 1) == 0)
				prev_op = 1;
		}
		else
			prev_op = 0;
		tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}

int	quote_check(t_list *tokens)
{
	while (tokens != NULL)
	{
		if (ft_strncmp(tokens->content, "\'", 1) == 0
			&& ft_count_char(tokens->content, '\'') % 2 != 0)
		{
			return (put_stderr(SHELL, NULL, NULL, "syntax error near quote"));
		}
		if (ft_strncmp(tokens->content, "\"", 1) == 0
			&& ft_count_char(tokens->content, '\"') % 2 != 0)
		{
			return (put_stderr(SHELL, NULL, NULL, "syntax error near quote"));
		}
		tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}

bool	is_operator(t_list *token)
{
	if (ft_strncmp(token->content, "|", 1) == 0
		|| ft_strncmp(token->content, "<", 1) == 0
		|| ft_strncmp(token->content, ">", 1) == 0)
	{
		return (true);
	}
	return (false);
}
