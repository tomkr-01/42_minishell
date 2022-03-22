#include "../inc/minishell.h"

// msh:
// $> export hello=textvar
// $> echo $hello
// textvar 
// $> echo $"hello"
// "hello" 						faulty!!!
// $> echo $'hello'
// 'hello'						faulty!!!

// bash:
// bash-3.2$ export hello=textvar
// bash-3.2$ echo $"hello"
// hello
// bash-3.2$ echo $'hello'
// hello

size_t	next_quote(char *token, char quote);
char	*expansion_char_remover(char *token);

char	*quote_remover(char *token)
{
	char	*q_removed;
	size_t	i;
	char	quote;

	i = 0;
	q_removed = NULL;
	while (token != NULL && token[i] != '\0')
	{
		quote = token[next_quote(token, 0)];
		q_removed = ft_strjoin_free(q_removed,
				ft_substr(token, 0, next_quote(token, 0)));
		token = ft_substr_free(token,
				next_quote(token, 0) + 1, ft_strlen(token));
		q_removed = ft_strjoin_free(q_removed,
				ft_substr(token, 0, next_quote(token, quote)));
		token = ft_substr_free(token,
				next_quote(token, quote) + 1, ft_strlen(token));
	}
	// printf("q_removed: %s|\n", q_removed);
	return (expansion_char_remover(q_removed));
}

size_t	next_quote(char	*token, char quote)
{
	size_t	i;
	char	*check_str;

	i = 0;
	while (token != NULL && token[i] != '\0')
	{
		check_str = ft_substr(token, 0, i + 1);
		if (quote == 0 && (token[i] == '\'' || token[i] == '\"')
			&& ft_count_char(check_str, '~') % 2 == 0)
		{
			ft_free((void **)&check_str);
			return (i);
		}
		else if (quote == token[i] && ft_count_char(check_str, '~') % 2 == 0)
		{
			ft_free((void **)&check_str);
			return (i);
		}
		i++;
		ft_free((void **)&check_str);
	}
	return (i);
}

char	*expansion_char_remover(char *token)
{
	size_t	i;
	size_t	j;
	char	*c_removed;

	i = 0;
	j = 0;
	c_removed = ft_calloc(ft_strlen(token) + 1, sizeof(*c_removed));
	if (c_removed == NULL)
		exit(put_stderr(SHELL, "expansion_char_remover()", NULL,
			strerror(ENOMEM)));
	while (token != NULL && token[i] != '\0')
	{
		if (token[i] == '~')
			i++;
		else
			c_removed[j++] = token[i++];
	}
	c_removed[j] = '\0';
	// printf("c_removed: %s|\n", c_removed);
	return (c_removed);
}
