#include "../inc/minishell.h"

extern t_minishell	g_msh;

size_t	next_exp(char *token);
int		valid_exp_char(int c, bool first_char);
char	*get_varname(char *token);
char	*expand_varname(char *varname);

char	*expansion(char *token, bool unquote)
{
	char	*expanded;
	size_t	i;

	expanded = NULL;
	i = 0;
	while (token != NULL && token[i] != '\0')
	{
		expanded = ft_strjoin_free(expanded,
				ft_substr(token, 0, next_exp(token)));
		token = ft_substr_free(token,
				next_exp(token), ft_strlen(token));
		expanded = ft_strjoin_free(expanded,
				expand_varname(get_varname(token)));
		token = ft_substr_free(token,
				ft_strlen_free(get_varname(token)) + 1, ft_strlen(token));
	}
	if (unquote)
		return (quote_remover(expanded));
	return (expanded);
}

// you have probably split the expander in two files because
// of the 3 functions below

char	*without_quotes(char *token, int *index, int old_index)
{
	char	*string;

	while (token[*index] != '\0' && token[*index] != '\'' && token[*index] != '\"')
		*index += 1;
	string = ft_substr(token, old_index, *index - old_index);
	*index -= 1;
	return (string);
}

char	*with_quotes(char *token, int *index, int old_index, char c)
{
	char	*string;

	*index += 1;
	while (token[*index] != '\0' && token[*index] != c)
		*index += 1;
	string = ft_substr(token, old_index, *index - old_index + 1);
	// *index += 1;
	return (string);
}

char	*expander(char *token, bool unquote)
{
	int			index;
	int			old_index;
	char		*string;
	char		*expanded;

	if (token == NULL)
		return (NULL);
	index = 0;
	expanded = NULL;
	while (token[index] != '\0')
	{
		old_index = index;
		if (token[index] != '\'' && token[index] != '\"')
			string = without_quotes(token, &index, old_index);
		else
			string = with_quotes(token, &index, old_index, token[index]);
		index++;
		expanded = ft_strjoin_free(expanded, expansion(string, unquote));
	}
	ft_free((void **)&token);
	return (expanded);
}

size_t	next_exp(char *token)
{
	size_t	pos;

	pos = 0;
	while (token != NULL && token[pos] != '\0')
	{
		if (token[pos] == '\'')
		{
			pos += ft_strchr_int(&token[pos + 1], '\'') + 1;
		}
		else if (token[pos] == '"')
		{
			while (token[++pos] != '\0' && token[pos] != '"')
			{
				if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
					return (pos);
			}
		}
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
		{
			return (pos);
		}
		if (token[pos] != '\0')
			pos++;
	}
	return (pos);
}

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalnum(c) || c == '_'
		|| (first_char == true && (c == '?' || c == '\'' || c == '"')))
		return (1);
	return (0);
}

char	*ft_chrdup(char c)
{
	char	*duplicate;

	duplicate = NULL;
	duplicate = (char *)malloc(2 * sizeof(char));
	if (duplicate == NULL)
		return (NULL);
	duplicate[0] = c;
	duplicate[1] = '\0';
	return (duplicate);
}

char	*get_varname(char *token)
{
	size_t	i;

	i = 1;
	if (token[0] == '\0')
		return (NULL);
	else if (token[1] == '?')
		return (ft_strdup("?"));
	else if (ft_isdigit(token[1]))
		return (ft_chrdup(token[1]));
	else if (token[1] == '\'' || token[1] == '"')
		return (ft_substr(token, 1, ft_strchr_int(token + 2, token[1]) + 2));
	while (valid_exp_char(token[i], false))
		i++;
	return (ft_substr(token, 1, i - 1));
}

char	*expand_varname(char *varname)
{
	char	*value;

	if (varname == NULL)
		return (NULL);
	else if (ft_strcmp(varname, "?") == 0)
		value = ft_itoa(g_msh.exit_code);
	else if (varname[0] == '\'' || varname[0] == '\"')
		value = ft_strdup(varname);
	else
		value = get_var((const char *)varname);
	value = ft_strjoin_free(ft_strdup(""), value);
	value = ft_strjoin_free(value, ft_strdup(""));
	ft_free((void **)&varname);
	return (value);
}

// int	main(int argc, char *argv[])
// {
// 	char	*str;
// 	char	*expanded;

// 	if (argc < 2)
// 		return (-1);
// 	str = ft_strdup(argv[1]);
// 	expanded = expander(str, true);
// 	printf("expanded: %s\n", expanded);
// 	return (0);
// }
