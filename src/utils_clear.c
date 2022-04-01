#include "../inc/minishell.h"

void	del_content(void *ptr)
{
	ft_free((void **)&ptr);
}

t_table	*get_head(void)
{
	static t_table		head;

	return (&head);
}

static void	redi_clear(t_redirection **redi)
{
	t_redirection	*p;
	t_redirection	*next;

	p = *redi;
	if (p == NULL)
		return ;
	while (p != NULL)
	{
		next = p->next;
		ft_free((void **)&(p->name));
		ft_free((void **)&p);
		p = next;
	}
	*redi = NULL;
}

void	table_clear(t_table **table)
{
	t_table	*p;
	t_table	*next;

	p = *table;
	if (p == NULL)
		return ;
	while (p != NULL)
	{
		next = p->next;
		ft_free_array(&(p->arguments));
		p->arguments = NULL;
		redi_clear(&(p->redirections));
		ft_free((void **)&p);
		p = next;
	}
	*table = NULL;
}

void	ft_atexit(int status)
{
	t_table		*head;

	head = get_head()->next;
	table_clear(&head);
	ft_free_array(&g_msh.env);
	exit(status);
}
