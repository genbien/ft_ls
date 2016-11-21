/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recur.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:04:31 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 11:20:20 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_manage_file(t_env *env, t_list *lst)
{
	t_data_max	*data_max;
	t_list		*list;

	data_max = NULL;
	list = NULL;
	data_max = (t_data_max *)malloc(sizeof(t_data_max));
	ft_init_data_max(data_max);
	while (lst)
	{
		FULL_PATH = ft_strinit(lst->content);
		lstat(FULL_PATH, &(env->stats));
		ft_extract_data(env, FULL_PATH);
		ft_lstinsert_picker(*env, &list);
		ft_assign_data_max(env, data_max);
		lst = lst->next;
		ft_strdel(&FULL_PATH);
	}
	ft_print_ls(env, list, data_max, 0);
	ft_free_list(&list);
	free(data_max);
}

void		ft_manage_dir(t_env *env, char *directory, DIR *cur_dir, int booh)
{
	t_data_max	*data_max;
	t_list		*list;

	data_max = NULL;
	list = NULL;
	lstat(directory, &(env->stats));
	data_max = (t_data_max *)malloc(sizeof(t_data_max));
	ft_init_data_max(data_max);
	while ((env->dir_content = readdir(cur_dir)) != NULL)
	{
		FULL_PATH = ft_join(directory, env->dir_content->d_name, "/");
		lstat(FULL_PATH, &(env->stats));
		if (ft_donot_continue(*env, FILENAME) != 0)
			ft_recur_helper(env, &list, data_max);
		ft_strdel(&FULL_PATH);
	}
	closedir(cur_dir);
	if (booh)
		ft_printf("%s:\n", directory);
	ft_print_ls(env, list, data_max, 1);
	ft_free_list(&list);
	env->blocks = 0;
	free(data_max);
}

void		ft_explore_dir(t_env *env, t_btree *cur_dir)
{
	DIR			*to_explore;
	char		*dirname;

	to_explore = NULL;
	errno = 0;
	if (cur_dir)
	{
		!env->FLAGS['r'] ? ft_putchar('\n') : 0;
		ft_explore_dir(env, cur_dir->left);
		env->FLAGS['r'] ? ft_putchar('\n') : 0;
		dirname = ft_strinit((char *)cur_dir->content);
		lstat(dirname, &(env->stats));
		to_explore = opendir(dirname);
		if (errno != 0)
			ft_print_errno(dirname);
		else
		{
			ft_manage_dir(env, dirname, to_explore, 1);
			ft_recur_launcher(opendir(dirname), env, dirname);
		}
		ft_strdel(&dirname);
		ft_explore_dir(env, cur_dir->right);
	}
}

t_btree		*ft_get_dir(DIR *cur_dir, t_env *env, char *directory)
{
	t_btree		*dir_here;

	dir_here = NULL;
	while ((env->dir_content = readdir(cur_dir)) != NULL)
	{
		FULL_PATH = ft_join(directory, env->dir_content->d_name, "/");
		lstat(FULL_PATH, &(env->stats));
		if (IS_DIRECTORY && IS_NOT_DOT && IS_NOT_DOTDOT)
		{
			if (env->FLAGS['a'] && env->dir_content->d_name[0] == '.')
				ft_cmp(env, &dir_here, FULL_PATH, ft_strlen(FULL_PATH) + 1);
			else if (env->dir_content->d_name[0] != '.')
				ft_cmp(env, &dir_here, FULL_PATH, ft_strlen(FULL_PATH) + 1);
		}
		ft_strdel(&FULL_PATH);
	}
	return (dir_here);
}

void		ft_recur_launcher(DIR *cur_dir, t_env *env, char *directory)
{
	t_btree		*directories_here;

	directories_here = NULL;
	lstat(directory, &(env->stats));
	if (IS_DIRECTORY)
	{
		if (env->FLAGS['R'])
			directories_here = ft_get_dir(cur_dir, env, directory);
		ft_explore_dir(env, directories_here);
		ft_btreedel(directories_here);
	}
	if (cur_dir != NULL)
		closedir(cur_dir);
}
