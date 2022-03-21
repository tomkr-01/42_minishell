#include "../inc/minishell.h"

size_t	text_token(const char *line, size_t *i);
size_t	operator_token(const char *line, size_t *i);

void	*lexer(char const *line)
{
	t_list	*tokens;
	size_t	i;
	size_t	chrs;

	i = 0;
	tokens = NULL;
	while (line != NULL && i < (size_t)ft_strlen(line))
	{
		chrs = 0;
		if (ft_strchr(OPERATORS, line[i]))
			chrs = operator_token(line, &i);
		else if (!ft_strchr(METACHARS, line[i]))
			chrs = text_token(line, &i);
		if (chrs != 0)
			ft_lstadd_back(&tokens, ft_lstnew(ft_substr(line, i - chrs, chrs)));
		else
			i++;
	}
	return (tokens);
}

size_t	operator_token(const char *line, size_t *i)
{
	size_t	chrs;

	chrs = 0;
	if (line[*i] == '|')
	{
		*i += 1;
		chrs += 1;
	}
	else if (line[*i] == '<')
	{
		while (line[*i] == '<')
		{
			*i += 1;
			chrs += 1;
		}
	}
	else if (line[*i] == '>')
	{
		while (line[*i] == '>')
		{
			*i += 1;
			chrs += 1;
		}
	}
	return (chrs);
}

size_t	text_token(const char *line, size_t *i)
{
	size_t	chrs;
	int		single_quotes;
	int		double_quotes;

	chrs = 0;
	single_quotes = -1;
	double_quotes = -1;
	while (line[*i] != '\0')
	{
		if (line[*i] == '\'')
			single_quotes *= -1;
		if (line[*i] == '"')
			double_quotes *= -1;
		if (ft_strchr(METACHARS, line[*i])
			&& single_quotes == -1 && double_quotes == -1)
			break ;
		*i += 1;
		chrs++;
	}
	return (chrs);
}
