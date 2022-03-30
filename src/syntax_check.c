#include "../inc/minishell.h"

extern t_minishell		g_msh;

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
	if (ft_lstlast(tokens)->content[0] == '<'
		|| ft_lstlast(tokens)->content[0] == '>'
		|| ft_lstlast(tokens)->content[0] == '|')
		return (put_stderr(SHELL, NULL, NULL,
				"syntax error near unexpected operator"));
	while (tokens != NULL)
	{
		if (is_operator(tokens))
		{
			if (ft_strlen(tokens->content) > 2 || prev_op == 2
				|| (tokens->content[0] == '|' && prev_op == 1))
				return (put_stderr(SHELL, NULL, NULL,
						"syntax error near unexpected token"));
			prev_op = 2;
			if (tokens->content[0] == '|')
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
	int	single_q;
	int	double_q;

	while (tokens != NULL)
	{
		single_q = ft_strchr_int(tokens->content, '\'');
		double_q = ft_strchr_int(tokens->content, '\"');
		if (((single_q != -1 && double_q == -1) || single_q < double_q)
			&& ft_count_char(tokens->content, '\'') % 2 != 0)
		{
			return (put_stderr(SHELL, NULL, NULL, "syntax error near quote"));
		}
		else if (((double_q != -1 && single_q == -1) || double_q < single_q)
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
	if (token->content[0] == '|'
		|| token->content[0] == '<'
		|| token->content[0] == '>')
	{
		return (true);
	}
	return (false);
}
