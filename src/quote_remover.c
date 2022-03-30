#include "../inc/minishell.h"

size_t	next_quote(char *expanded, char quote);

char	*quote_remover(char *expanded)
{
	char	*q_removed;
	size_t	i;
	char	quote;

	i = 0;
	q_removed = NULL;
	while (expanded != NULL && expanded[i] != '\0')
	{
		quote = expanded[next_quote(expanded, 0)];
		q_removed = ft_strjoin_free(q_removed,
				ft_substr(expanded, 0, next_quote(expanded, 0)));
		expanded = ft_substr_free(expanded,
				next_quote(expanded, 0) + 1, ft_strlen(expanded));
		q_removed = ft_strjoin_free(q_removed,
				ft_substr(expanded, 0, next_quote(expanded, quote)));
		expanded = ft_substr_free(expanded,
				next_quote(expanded, quote) + 1, ft_strlen(expanded));
	}
	ft_free((void **)&expanded);
	return (q_removed);
}

size_t	next_quote(char	*expanded, char quote)
{
	size_t	i;
	char	*check_str;

	i = 0;
	while (expanded != NULL && expanded[i] != '\0')
	{
		check_str = ft_substr(expanded, 0, i + 1);
		if (quote == 0 && (expanded[i] == '\'' || expanded[i] == '\"'))
		{
			ft_free((void **)&check_str);
			return (i);
		}
		else if (quote == expanded[i])
		{
			ft_free((void **)&check_str);
			return (i);
		}
		i++;
		ft_free((void **)&check_str);
	}
	return (i);
}
