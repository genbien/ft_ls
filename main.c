/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/17 23:25:55 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	ft_extract_options_ls_helper(t_options *options)
{
	int		k;

	k = 0;
	while (k < 122)
	{
		options->flags[k] = FALSE;
		k++;
	}
}
int			ft_extract_options_ls(char **av, t_options *options)
{
	int		i;
	int		j;
	int		is_one;
	int		value;

	i = 1;
	ft_extract_options_ls_helper(options);
	while (av[i] && av[i][0] == '-')
	{
		j = 1;
		if (av[i][j] == '\0')
			return (i);
		while (av[i][j] != '\0')
		{
			value = av[i][j];
			if (value == '-')
				return (i + 1);
			is_one == TRUE && value == 'l' ? options->flags['1'] = FALSE : 0;
			value == '1' ? is_one = TRUE : 0;
			options->flags[value] = TRUE;
			j++;
		}
		i++;
	}
	return (i);
}

void		ft_process_none(t_env *env)
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

void		ft_process_files(t_env *env)
{
	if (env->lst_file)
	{
		ft_manage_file(env, env->lst_file);
		if (env->lst_dir)
			ft_putchar('\n');
	}
}

void		ft_process_dir(t_env *env)
{
	DIR		*to_explore;
	char	*directory;
	t_list	*list;

	list = env->lst_dir;
	while (list)
	{
		errno = 0;
		directory = ft_strinit(list->content);
		(env->args >= 2) ? ft_printf("%s:\n", directory) : 0;
		to_explore = opendir(directory);
		if (errno != 0)
			ft_print_errno(directory);
		else
		{
			ft_manage_dir(env, directory, opendir(directory), 0);
			ft_recur_launcher(opendir(directory), env, directory);
		}
		ft_strdel(&directory);

		//NEED TO RESET THE O_LIST + THE BLOCK
		env->blocks = 0;
		list = list->next;
		list ? ft_putchar('\n') : 0;
	}
}


void		ft_clear_env(t_env *env)
{
	free(env->options);
	free(env->data);
	ft_lstclr(&env->lst_dir);
	ft_lstclr(&env->lst_file);
	ft_lstclr(&env->lst_none);
	ft_lstclr(&env->lst);
}

int			main(int ac, char **av)
{
	t_env	env;
	int		i;

	ft_init_env(&env);
	env.options = (t_options *)malloc(sizeof(t_options));
	i = ft_extract_options_ls(av, env.options);
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
		if (!env.lst_none)
		{
			ft_process_files(&env);
			ft_process_dir(&env);
		}
	}
	ft_clear_env(&env);
	return (0);
}
