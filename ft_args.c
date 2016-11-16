/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 14:18:21 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/15 11:55:47 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		ft_lstinsert_args(t_list **list, void *content, size_t c_size)
{
	t_list	*new_list;
	int		been_inserted;

	been_inserted = 0;
	new_list = *list;
	c_size += 1;
	if (new_list && ft_strcmp(new_list->content, content) > 0)
		*list = ft_lstinsert(content, c_size, new_list);
	else if (new_list)
	{
		while (new_list->next)
		{
			if (ft_strcmp(new_list->next->content, content) > 0)
			{
				new_list->next = ft_lstinsert(content, c_size, new_list->next);
				been_inserted = 1;
				break ;
			}
			new_list = new_list->next;
		}
		if (been_inserted == 0)
			new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

void			ft_sort_args(t_env *env, char **av, int ac)
{
	int		i;

	i = ac;
	if (i == 0)
		ft_lstinsert_args(&env->lst_dir, ".", 1);
	while (av[i])
	{
		if (lstat(av[i], &(env->stats)) == -1)
			ft_lstinsert_args(&env->lst_none, av[i], ft_strlen(av[i]));
		else
		{
			if (S_ISDIR(env->stats.st_mode))
				ft_lstinsert_args(&env->lst_dir, av[i], ft_strlen(av[i]));
			else
				ft_lstinsert_args(&env->lst_file, av[i], ft_strlen(av[i]));
		}
		i++;
	}
}
