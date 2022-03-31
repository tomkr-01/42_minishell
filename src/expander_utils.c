#include "../inc/minishell.h"

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalpha(c) || c == '_' || ft_isdigit(c)
		|| (first_char == true && (c == '?' || c == '\'' || c == '"')))
		return (1);
	return (0);
}

t_quotes	set_quotes(char *token, size_t pos)
{
	size_t		i;
	t_quotes	q;

	i = 0;
	q.s = -1;
	q.d = -1;
	while (token != NULL && token[i] != '\0' && i < pos)
	{
		if (token[i] == '\'' && q.d < 0)
			q.s *= -1;
		if (token[i] == '\"' && q.s < 0)
			q.d *= -1;
		i++;
	}
	return (q);
}

void	expand_single_quotes(char *token, size_t *pos, t_quotes *q)
{
	*pos += 1;
	if (q->s > 0)
		q->s = -1;
	else
	{	
		while (token[*pos] != '\0' && token[*pos] != '\'')
			*pos += 1;
		*pos += 1;
	}
}

void	expand_double_quotes(char *token, size_t *pos, t_quotes *q)
{
	*pos += 1;
	if (q->d > 0)
		q->d = -1;
	else
	{
		while (token[*pos] != '\0' && token[*pos] != '\"')
		{
			if (token[*pos] == '$' && valid_exp_char(token[*pos + 1], true))
				return ;
			*pos += 1;
		}
		*pos += 1;
	}
}

size_t	next_exp(char *token, size_t pos)
{
	size_t		i;
	t_quotes	q;

	i = 0;
	q = set_quotes(token, pos);
	while (token != NULL && pos < ft_strlen(token))
	{
		if (token[pos] == '\'' && q.d < 0)
			expand_single_quotes(token, &pos, &q);
		else if (token[pos] == '\"' && q.s < 0)
			expand_double_quotes(token, &pos, &q);
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true)
			&& q.s < 0)
		{
			if (q.d > 0 && token[pos + 1] == '\"')
				pos++;
			return (pos);
		}
		else
			pos++;
	}
	return (pos);
}
