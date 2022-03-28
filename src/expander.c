#include "../inc/minishell.h"

extern t_minishell	g_msh;

size_t	next_exp(char *token, size_t pos, bool print);
int		valid_exp_char(int c, bool first_char);
char	*get_varname(char *token);
char	*expand_varname(char *varname);

// run with this command!!!
// echo "$PWD"'$USER'"$USER""'$PWD'"

void	print_int_codes(char *str);

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
		expanded = ft_strjoin_free(expanded, ft_substr(token, i, next_exp(token, i, true) - i));
		i = next_exp(token, i, false);
		expanded = ft_strjoin_free(expanded, expand_varname(get_varname(&token[next_exp(token, i, true)])));
		i += ft_strlen_free(get_varname(&token[next_exp(token, i, false)]));
	}
	char * q_removed = quote_remover(expanded);
	// print_int_codes(q_removed);
	return (q_removed);
}

void	print_int_codes(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		printf("%i ", str[i++]);
	}
}

size_t	next_exp(char *token, size_t pos, bool print)
{
	size_t	i;
	int		s;
	int		d;

	i = 0;
	s = -1;
	d = -1;
	while (token != NULL && token[i] != '\0' && i < pos)
	{
		if (token[i] == '\'' && d < 0)
		{
			s *= -1;
		}
		if (token[i] == '\"' && s < 0)
		{
			d *= -1;
		}
		i++;
	}
	while (token != NULL && pos < ft_strlen(token))
	{
		if (token[pos] == '\'' && d < 0)
		{
			pos++;
			if (s > 0)
				s = -1;
			else
			{	
				while (token[pos] != '\0' && token[pos] != '\'')
					pos++;
				pos++;
			}
		}
		else if (token[pos] == '\"' && s < 0)
		{
			pos++;
			if (d > 0)
				d = -1;
			else
			{
				while (token[pos] != '\0' && token[pos] != '\"')
				{
					if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
					{
						return (pos);
					}
					pos++;
				}
				pos++;
			}
		}
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true) && (s < 0))
		{
			return (pos);
		}
		else
			pos++;
	}
	return (pos);
}

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalpha(c) || c == '_' || (first_char == true && (c == '?' || c == '\'' || c == '"')) || (first_char == false && ft_isdigit(c)))
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
	size_t	j;

	i = 1;
	if (token == NULL || token[0] != '$')
		return (NULL);
	if (token[1] == '?')
		return (ft_strdup("$?"));
	else if (ft_isdigit(token[1]))
		return (ft_chrdup(token[1]));
	else if (token[1] == '\'' || token[1] == '"')
		return (ft_substr(token, 0, ft_strchr_int(token + 2, token[1]) + 1));
	j = 0;
	while ((valid_exp_char(token[i], true) && j == 0) || valid_exp_char(token[i], false))
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
