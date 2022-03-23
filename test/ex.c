#include "inc/minishell.h"

char	*without_quotes(char *token, int *index, int old_index)
{
	char	*string;

	*index += 1;
	while (token[*index] != '\0' && token[*index] != '\'' && token[*index] != '\"')
		*index += 1;
	string = ft_substr(token, old_index, *index - old_index);
	return (string);
}

char	*with_quotes(char *token, int *index, int old_index, char c)
{
	char	*string;

	*index += 1;
	while (token[*index] != '\0' && token[*index] != c)
		*index += 1;
	string = ft_substr(token, old_index, *index - old_index + 1);
	*index += 1;
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
	while (token[index] != '\0')
	{
		old_index = index;
		if (token[index] != '\'' && token[index] != '\"')
			string = without_quotes(token, &index, old_index);
		else
			string = with_quotes(token, &index, old_index, token[index]);
		// index++;
		printf("string: >%s<\n", string);

	}
	return (NULL);
}

int	main(void)
{
	char	*str;
	char	*str1;
	char	*str2;
	char	*str3;

	str = "what$thefuck\"$PWD\"Imnotthat$\'$USER\'\"$USER\"\"\'$PWD\'\"";
	str1 = "\"ashdfa j \' hsdjkfhska jfas \'fjaskhf sakf hsalhdf as";
	str2 = "";
	str3 = NULL;
	expander(str, false);
	write(1, "\n", 1);
	expander(str1, false);
	write(1, "\n", 1);
	expander(str2, false);
	write(1, "\n", 1);
	expander(str3, false);
	write(1, "\n", 1);
	return (0);
}
