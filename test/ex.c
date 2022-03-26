#include "../inc/minishell.h"
#include <stdlib.h>

extern char	*environ;

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

bool	to_be_expanded(char *string)
{
	if (string[0] == '\'')
		return (false);
	return (true);
}

char	*remove_quotes(char *string)
{
	int		index;
	int		start;
	int		len;
	char	to_remove;
	char	*result;

	index =0 ;
	len = 0;
	// printf("string: %s\n", string);
	if (string[index] == '\'' || string[index] == '\"')
	{
		to_remove = string[index];
		start = 1;
		index++;
	}
	else
	{
		printf("result: %s\n", result);
		return (string);
	}
	while (string[index] != '\0' && string[index++] != to_remove)
		len++;
	result = ft_substr(string, start, len);
	free(string);
	printf("result: %s\n", result);
	return (result);
}

// char	*start_expansion(char *string)
// {
// 	bool	expansion;
// 	char	*unquoted;

// 	if (string == NULL)
// 		return (NULL);
// 	expansion = to_be_expanded(string);
// 	// unquoted = remove_quotes(string);
// 	return (expander(unquoted, expansion));
// }

char	*expansion(char *token, bool unquote)
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
		printf("%s\n", string);
		index++;
		expanded = ft_strjoin_free(expanded, expander(string, true));
	}
	return (expanded);
}

int	main(void)
{
	char	*str;
	char	*str1;
	char	*str2;
	char	*str3;

	str = "\"$USER\"\"Users/$USER/file\"\"\'$USER\'\"\'$USER\'";
	// str1 = "\"ashdfa j \' hsdjkfhska jfas \'fjaskhf sakf hsalhdf as";
	// str2 = "";
	// str3 = NULL;
	expansion(str, false);
	write(1, "\n", 1);
	// expansion(str1, false);
	// write(1, "\n", 1);
	// expansion(str2, false);
	// write(1, "\n", 1);
	// expansion(str3, false);
	// write(1, "\n", 1);
	return (0);
}

///////////////////////////////////////////

// old expander

// #include "../inc/minishell.h"

// extern t_minishell	g_msh;

// size_t	next_exp(char *token);
// int		valid_exp_char(int c, bool first_char);
// char	*get_varname(char *token);
// char	*expand_varname(char *varname);

// char	*expander(char *token, bool unquote)
// {
// 	char	*expanded;
// 	size_t	i;

// 	expanded = NULL;
// 	i = 0;
// 	while (token != NULL && token[i] != '\0')
// 	{
// 		expanded = ft_strjoin_free(expanded,
// 				ft_substr(token, 0, next_exp(token)));
// 		token = ft_substr_free(token,
// 				next_exp(token), ft_strlen(token));
// 		expanded = ft_strjoin_free(expanded,
// 				expand_varname(get_varname(token)));
// 		token = ft_substr_free(token,
// 				ft_strlen_free(get_varname(token)) + 1, ft_strlen(token));
// 	}
// 	if (unquote)
// 		return (quote_remover(expanded));
// 	return (expanded);
// }

// size_t	next_exp(char *token)
// {
// 	size_t	pos;

// 	pos = 0;
// 	while (token != NULL && token[pos] != '\0')
// 	{
// 		if (token[pos] == '\'')
// 		{
// 			pos += ft_strchr_int(&token[pos + 1], '\'') + 1;
// 		}
// 		else if (token[pos] == '"')
// 		{
// 			while (token[++pos] != '\0' && token[pos] != '"')
// 			{
// 				if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
// 					return (pos);
// 			}
// 		}
// 		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
// 		{
// 			return (pos);
// 		}
// 		if (token[pos] != '\0')
// 			pos++;
// 	}
// 	return (pos);
// }

// int	valid_exp_char(int c, bool first_char)
// {
// 	if (ft_isalnum(c) || c == '_'
// 		|| (first_char == true && (c == '?' || c == '\'' || c == '"')))
// 		return (1);
// 	return (0);
// }

// char	*get_varname(char *token)
// {
// 	size_t	i;

// 	i = 1;
// 	if (token[0] == '\0')
// 		return (NULL);
// 	else if (token[1] == '?')
// 		return (ft_strdup("?"));
// 	else if (token[1] == '\'' || token[1] == '"')
// 		return (ft_substr(token, 1, ft_strchr_int(token + 2, token[1]) + 2));
// 	while (valid_exp_char(token[i], false))
// 		i++;
// 	return (ft_substr(token, 1, i - 1));
// }

// char	*expand_varname(char *varname)
// {
// 	char	*value;

// 	if (varname == NULL)
// 		return (NULL);
// 	else if (ft_strcmp(varname, "?") == 0)
// 		value = ft_itoa(g_msh.exit_code);
// 	else if (varname[0] == '\'' || varname[0] == '\"')
// 		value = ft_strdup(varname);
// 	else
// 		value = get_var((const char *)varname);
// 	value = ft_strjoin_free(ft_strdup(""), value);
// 	value = ft_strjoin_free(value, ft_strdup(""));
// 	ft_free((void **)&varname);
// 	return (value);
// }

