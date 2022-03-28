#include "../inc/minishell.h"

extern t_minishell	g_msh;

size_t	next_exp(char *token, size_t pos, bool print);
int		valid_exp_char(int c, bool first_char);
char	*get_varname(char *token);
char	*expand_varname(char *varname);

char	*expander(char *token, bool unquote)
{
	char	*expanded;
	char	*one;
	char	*two;
	size_t	i;

	one = NULL;
	two = NULL;
	expanded = NULL;
	i = 0;
	while (token != NULL && i < ft_strlen(token))
	{
		expanded = ft_strjoin_free(expanded,
				ft_substr(token, i, next_exp(token, i, true) - i));
		i = next_exp(token, i, false);
		expanded = ft_strjoin_free(expanded,
				expand_varname(get_varname(&token[next_exp(token, i, true)])));
		i += ft_strlen_free(get_varname(&token[next_exp(token, i, false)]));
	}
	return (quote_remover(expanded));
}

typedef struct s_quotes
{
	int	s = -1;
	int	d = -1;
}	t_quotes;

t_quotes	set_quotes(char *token, size_t pos)
{
	size_t		i;
	t_quotes	q;

	i = 0;
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

void	expand_single_quotes(size_t *pos, t_quotes *q)
{
	pos++;
	if (q.s > 0)
		q.s = -1;
	else
	{	
		while (token[pos] != '\0' && token[pos] != '\'')
			pos++;
		pos++;
	}
}

void	expand_double_quotes(size_t *pos, t_quotes *q)
{
	pos++;
	if (q.d > 0)
		q.d = -1;
	else
	{
		while (token[pos] != '\0' && token[pos] != '\"')
		{
			if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
				return (pos);
			pos++;
		}
		pos++;
	}
}

size_t	next_exp(char *token, size_t pos, bool print)
{
	size_t		i;
	t_quotes	q;

	i = 0;
	q = set_quotes(token, pos);
	while (token != NULL && pos < ft_strlen(token))
	{
		if (token[pos] == '\'' && q.d < 0)
		{
			expand_single_quotes(&pos, &q);
		}
		else if (token[pos] == '\"' && q.s < 0)
		{
			expand_double_quotes(&pos, &q);
		}
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true)
		{
			&& q.s < 0)
			return (pos);
		}
		else
			pos++;
	}
	return (pos);
}

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalpha(c) || c == '_'
		|| (first_char == true && (c == '?' || c == '\'' || c == '"'))
		|| (first_char == false && ft_isdigit(c)))
		return (1);
	return (0);
}

char	*get_varname(char *token)
{
	size_t	i;
	size_t	j;

	i = 1;
	if (token == NULL || token[0] != '$')
		return (NULL);
	if (token[1] == '?')
		return (ft_strdup("$?"));
	else if (token[1] == '\'' || token[1] == '"')
		return (ft_substr(token, 0, ft_strchr_int(token + 2, token[1]) + 1));
	j = 0;
	while ((valid_exp_char(token[i], true) && j == 0)
		|| valid_exp_char(token[i], false))
	{
		i++;
		j++;
	}
	return (ft_substr(token, i - j - 1, i));
}

char	*expand_varname(char *varname)
{
	char	*value;

	if (varname == NULL || varname[0] == '\0')
		return (NULL);
	else if (ft_strcmp(varname, "$?") == 0)
		value = ft_itoa(g_msh.exit_code);
	else if (varname[1] == '\'' || varname[1] == '\"')
		value = ft_strdup(&varname[1]);
	else
		value = get_var((const char *)&varname[1]);
	value = ft_strjoin_free(ft_strdup(NULL), value);
	value = ft_strjoin_free(value, ft_strdup(NULL));
	ft_free((void **)&varname);
	return (value);
}
