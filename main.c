/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 13:46:22 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Bonus :
** -G
** -1
** -d
** -U
** ACL
** -c
** -p
** -n
*/

char			*ft_join(char *s1, char *s2, char *divider)
{
	char	*str;

	if (s1[ft_strlen(s1) - 1] == '/' && EQU(divider, "/"))
		divider = "";
	str = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strcpy(str, s1);
	ft_strcat(str, divider);
	ft_strcat(str, s2);
	return (str);
}

static void		ft_process_none(t_env *env)
{
	while (env->lst_none)
	{
		if (EQU(env->lst_none->content, ""))
		{
			ft_printf("ft_ls: %s: No such file or directory\n",
			"fts_open");
			return ;
		}
		else
			ft_printf("ft_ls: %s: No such file or directory\n",
			env->lst_none->content);
		env->lst_none = env->lst_none->next;
		if (!env->lst_none)
		{
			if (env->lst_file || env->lst_dir)
				ft_putchar('\n');
		}
	}
}

static void		ft_process_files(t_env *env)
{
	if (env->lst_file)
	{
		env->basedir = ft_strinit(env->lst_file->content);
		ft_manage_file(env, env->lst_file);
		if (env->lst_dir)
			ft_putchar('\n');
		ft_strdel(&env->basedir);
	}
}

static void		ft_process_dir(t_env *env)
{
	DIR		*to_explore;
	char	*directory;
	t_list	*list;

	list = env->lst_dir;
	while (list)
	{
		errno = 0;
		directory = ft_strinit(list->content);
		env->basedir = ft_strinit(list->content);
		(env->args >= 2) ? ft_printf("%s:\n", directory) : 0;
		if ((to_explore = opendir(directory)) && errno != 0)
			ft_print_errno(directory);
		else
		{
			ft_manage_dir(env, directory, opendir(directory), 0);
			ft_recur_launcher(opendir(directory), env, directory);
		}
		closedir(to_explore);
		ft_strdel(&directory);
		ft_strdel(&env->basedir);
		env->blocks = 0;
		list = list->next;
		list ? ft_putchar('\n') : 0;
	}
}

int				main(int ac, char **av)
{
	t_env	env;
	int		i;

	ft_init_env(&env);
	env.options = (t_options *)malloc(sizeof(t_options));
	i = ft_extract_options_ls(av, &env);
	env.args = ac - i;
	if (i == ac)
	{
		ft_sort_args(&env, av, 0);
		ft_process_files(&env);
		ft_process_dir(&env);
	}
	else if (ac - 1 >= 1)
	{
		ft_sort_args(&env, av, i);
		ft_process_none(&env);
		if (!env.lst_none)
		{
			ft_process_files(&env);
			ft_process_dir(&env);
		}
	}
	ft_free_env(&env);
	return (1);
}
