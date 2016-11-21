/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 14:18:21 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 13:32:32 by tbouder          ###   ########.fr       */
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

static void		ft_sort_args_helper(t_env *env, char **av, int i)
{
	char	*str;

	str = ft_strnew(PATH_MAX);
	readlink(av[i], str, PATH_MAX);
	lstat(str, &(env->stats));
	if (S_ISDIR(env->stats.st_mode))
		ft_lstinsert_args(&env->lst_dir, av[i], ft_strlen(av[i]));
	else
		ft_lstinsert_args(&env->lst_file, av[i], ft_strlen(av[i]));
	ft_strdel(&str);
}

void			ft_sort_args(t_env *env, char **av, int ac)
{
	if (ac == 0)
	{
		if (!env->FLAGS['d'])
			ft_lstinsert_args(&env->lst_dir, ".", 1);
		else
			ft_lstinsert_args(&env->lst_file, ".", 1);
		return ;
	}
	while (av[ac])
	{
		if (lstat(av[ac], &(env->stats)) == -1)
			ft_lstinsert_args(&env->lst_none, av[ac], ft_strlen(av[ac]));
		else
		{
			if (!env->FLAGS['d'] && S_ISDIR(env->stats.st_mode))
				ft_lstinsert_args(&env->lst_dir, av[ac], ft_strlen(av[ac]));
			else if (!env->FLAGS['d'] && S_ISLNK(env->stats.st_mode))
				ft_sort_args_helper(env, av, ac);
			else
				ft_lstinsert_args(&env->lst_file, av[ac], ft_strlen(av[ac]));
		}
		ac++;
	}
}

static int		ft_handle_errors(t_env *env, int value)
{
	int		ret;

	ret = 0;
	if (value <= 32)
		ret = 0;
	else if (value <= 47)
		ret = -1;
	else if (value <= 57)
		ret = 1;
	else if (value <= 64)
		ret = -1;
	else if (value <= 90)
		ret = 1;
	else if (value <= 96)
		ret = -1;
	else if (value <= 122)
		ret = 1;
	if (ret == -1)
	{
		ft_printf("./ft_ls: illegal option -- %c\n", value);
		ft_free_env(env);
		exit(1);
	}
	return (ret);
}

int				ft_extract_options_ls(char **av, t_env *env)
{
	int		i;
	int		j;
	int		is_one;

	i = 1;
	ft_nbrinit(env->FLAGS, 121);
	while (av[i] && av[i][0] == '-' && av[i][1] != '\0')
	{
		j = 1;
		while (av[i][j] != '\0')
		{
			if (av[i][j] == '-')
				return (i + 1);
			if (ft_handle_errors(env, av[i][j]) == 1)
			{
				is_one && av[i][j] == 'l' ? env->FLAGS['1'] = 0 : 0;
				av[i][j] == '1' ? is_one = TRUE : 0;
				env->FLAGS[(int)av[i][j]] = TRUE;
			}
			j++;
		}
		i++;
	}
	return (i);
}
