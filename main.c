/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 18:33:08 by tbouder          ###   ########.fr       */
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
	while (env->lst_file)
	{
		ft_recur_start(opendir(env->lst_file->content), env, env->lst_file->content, &env->o_list);
		ft_print_list(*env);

		//NEED TO RESET THE O_LIST + THE BLOCK
		env->blocks = 0;
		env->o_list = NULL;

		env->lst_file = env->lst_file->next;
		env->lst_file ? ft_putchar('\n') : 0;
	}
}

void		ft_process_dir(t_env *env)
{
	while (env->lst_dir)
	{
		(env->args >= 2) ? ft_printf("%s:\n", env->lst_dir->content) : 0;
		ft_recur_start(opendir(env->lst_dir->content), env, env->lst_dir->content, &env->o_list);
		ft_print_list(*env);

		//NEED TO RESET THE O_LIST + THE BLOCK
		env->blocks = 0;
		env->o_list = NULL;

		env->lst_dir = env->lst_dir->next;
		env->lst_dir ? ft_putchar('\n') : 0;
	}
}

int			main(int ac, char **av)
{
	t_env	env;
	int		i;

	ft_init_env(&env);
	i = ft_extract_flags(av, &env);
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
