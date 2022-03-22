#include "../inc/minishell.h"

extern t_minishell	g_msh;

size_t	next_exp(char *token, size_t pos, bool print);
int		valid_exp_char(int c, bool first_char);
char	*get_varname(char *token);
char	*expand_varname(char *varname);

// run with this command!!!
// echo "$PWD"'$USER'"$USER""'$PWD'"


char	*expander(char *token, bool unquote)
{
	char	*expanded;
	size_t	i;

	expanded = NULL;
	i = 0;
	while (token != NULL && i < ft_strlen(token))
	{
		expanded = ft_strjoin_free(expanded,
				ft_substr(token, i, next_exp(token, i, true) - i));
		printf("expanded: %s|\n", expanded);
		i = next_exp(token, i, false);
		printf("i: %zu, rest token: %s\n", i, &token[i]);
		// printf("next_exp: %s|\n", &token[next_exp(&&toke[i]n[i], i, false)]);
		expanded = ft_strjoin_free(expanded,
				expand_varname(get_varname(&token[next_exp(token, i, true)])));
		printf("expanded2: %s\n", expanded);
		i += ft_strlen_free(get_varname(&token[next_exp(token, i, false)]));
		printf("i2: %zu, rest token: %s\n\n", i, ft_substr(token, i, ft_strlen(token)));
	}
	printf("expanded: %s|\n", expanded);
	if (unquote)
		return (quote_remover(expanded));
	return (expanded);
}

size_t	next_exp(char *token, size_t pos, bool print)
{
	size_t	i;
	int		s;
	int		d;

	i = 0;
	s = -1;
	d = -1;
	while (i < ft_strlen(token) && i < pos)
	{
		if (token[i] == '\'' && (s > 0 || d < 0))
		{
			// printf("single quote at %zu, %s\n", i, &token[i]);
			s *= -1;
		}
		if (token[i] == '\"' && (d > 0 || s < 0))
		{
			// printf("double quote at %zu, %s\n", i, &token[i]);
			d *= -1;
		}
		i++;
	}
	printf("next_exp: pos: %zu, %s|\n", pos, &token[pos]);
	printf("s_q: single: %i, double: %i\n", s, d);
	while (token != NULL && pos < ft_strlen(token))
	{
		if (token[pos] == '\'' && d < 0)
		{
			if (print)
				printf("next_exp, s_quote: token[%zu]: %s|\n", pos, &token[pos]);
			pos += ft_strchr_int(&token[pos + 1], '\'') + 1;
		}
		else if (token[pos] == '\"' && s < 0)
		{
			while (token[++pos] != '\0' && token[pos] != '"')
			{
				if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
				{
					if (print)
						printf("next_exp, d_qoute: token[%zu]: %s|\n", pos, &token[pos]);
					return (pos);
				}
			}
		}
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
		{
			if (print)
				printf("next_exp, $_sign: token[%zu]: %s|\n", pos, &token[pos]);
			return (pos);
		}
		if (pos < ft_strlen(token))
			pos++;
	}
	printf("pos: %zu\n", pos);
	return (pos);
}

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalpha(c) || (ft_isdigit(c) && first_char == true) || c == '_'
		|| (first_char == true && (c == '?' || c == '\'' || c == '"')))
		return (1);
	return (0);
}

char	*get_varname(char *token)
{
	size_t	i;
	size_t	j;

	i = 1;
	printf("get_varname: %s|\n", token);
	if (token == NULL || token[0] != '$')
		return (NULL);
	if (token[1] == '?')
		return (ft_strdup("$?"));
	else if (token[1] == '\'' || token[1] == '"')
		return (ft_substr(token, 0, ft_strchr_int(token + 2, token[1]) + 1));
	j = 0;
	while ((valid_exp_char(token[i], true) && j == 0) || valid_exp_char(token[i], false))
	{
		i++;
		j++;
	}
	printf("varname: %s\n", ft_substr(token, i - j - 1, i));
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
	value = ft_strjoin_free(ft_strdup("~"), value);
	value = ft_strjoin_free(value, ft_strdup("~"));
	ft_free((void **)&varname);
	return (value);
}
