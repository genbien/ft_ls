/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/10 14:48:52 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_process_none(t_env *env)
{
	while (env->lst_none)
	{
		ft_printf("ft_ls: %s: No such file or directory\n",
			env->lst_none->content);
		env->lst_none = env->lst_none->next;
		if (!env->lst_none)
			ft_putchar('\n');
	}
}

void		ft_process_files(t_env *env)
{
	char	*directory;

	while (env->lst_file)
	{
		directory = ft_strinit(env->lst_file->content);
		ft_manage_file(env, directory);
		ft_strdel(&directory);

		//NEED TO RESET THE O_LIST + THE BLOCK
		env->blocks = 0;

		env->lst_file = env->lst_file->next;
		!env->lst_file ? ft_putchar('\n') : 0;
	}
}

void		ft_process_dir(t_env *env)
{
	char	*directory;

	while (env->lst_dir)
	{
		directory = ft_strinit(env->lst_dir->content);
		(env->args >= 2) ? ft_printf("%s:\n", directory) : 0;
		ft_manage_dir(env, directory, opendir(directory), 0);
		ft_recur_launcher(opendir(directory), env, directory);
		ft_strdel(&directory);

		//NEED TO RESET THE O_LIST + THE BLOCK
		env->blocks = 0;
		env->lst_dir = env->lst_dir->next;
		env->lst_dir ? ft_putchar('\n') : 0;
	}
}

int			main(int ac, char **av)
{
	t_env	env;
	int		i;

	ft_init_env(&env);
	env.options = (t_options *)malloc(sizeof(t_options));
	i = ft_extract_options(av, env.options);
	env.args = ac - i;
	if (i == ac)
	{
		ft_sort_args(&env, av, 0);
		ft_process_dir(&env);
	}
	else if (ac - 1 >= 1)
	{
		ft_sort_args(&env, av, i);
		ft_process_none(&env);
		ft_process_files(&env);
		ft_process_dir(&env);
	}
	return (0);
}
