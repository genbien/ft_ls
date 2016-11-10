/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recur.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:04:31 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/10 15:16:33 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	ft_extract_data(t_env *env, char *filename)
{
	ft_init_data(env);
	ft_extract_filename(env, filename);
	ft_extract_type(env);
	ft_extract_perm(env);
	ft_extract_time(env);
	env->FLAGS['l'] ? ft_extract_attributs(env) : 0;
	env->FLAGS['l'] ? ft_extract_hard_links(env) : 0;
	env->FLAGS['l'] ? ft_extract_owner(env) : 0;
	env->FLAGS['l'] ? ft_extract_group(env) : 0;
	env->FLAGS['l'] ? ft_extract_size(env) : 0;
	env->FLAGS['l'] ? ft_extract_blocks(env) : 0;
	env->data->major = major(env->stats.st_rdev);
	env->data->minor = minor(env->stats.st_rdev);
}


/******************************************************************************/

void		ft_data_max_init(t_data_max *max)
{
	max->max_filename_len = 0;
	max->max_group_len = 0;
	max->max_link_len = 0;
	max->max_owner_len = 0;
	max->max_size_len = 0;
	max->max_minor_len = 0;
	max->max_major_len = 0;
	max->nb_file = 0;
}

void		ft_data_max_assign(t_env *env, t_data_max *max)
{
	int		value;

	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
	value = ft_strlen(env->data->filename);
	value > max->max_filename_len ? max->max_filename_len = value : 0;
	if (env->FLAGS['l'])
	{
		value = ft_strlen(env->data->group);
		value > max->max_group_len ? max->max_group_len = value : 0;
		value = ft_strlen(env->data->owner);
		value > max->max_owner_len ? max->max_owner_len = value : 0;
		value = ft_nbrlen(env->data->size);
		value > max->max_size_len ? max->max_size_len = value : 0;
		value = ft_nbrlen(env->data->hard_link);
		value > max->max_link_len ? max->max_link_len = value : 0;
		value = ft_nbrlen(env->data->major);
		value > max->max_major_len ? max->max_major_len = value : 0;
		value = ft_nbrlen(env->data->minor);
		value > max->max_minor_len ? max->max_minor_len = value : 0;
	}
	max->nb_file += 1;
}

void		ft_print_cont(t_env *env, t_list *list, t_data_max *max, int is_dir)
{
	if (env->FLAGS['1'])
		ft_ls_one(*env, list);
	else if (env->FLAGS['l'])
		ft_ls_long(*env, list, is_dir, *max);
	else
		ft_ls_short(*env, list, *max);
	max = NULL;
	if (!list->next)
		is_dir == 1 ? ft_putchar('\n') : 0;
}

void 		ft_cmp(t_env *env, t_btree **btree, void const *content, size_t c_size)
{
	if (env->FLAGS['r'])
		ft_btreecmp_rev(btree, content, c_size);
	else
		ft_btreecmp(btree, content, c_size);
}

/******************************************************************************/

void		ft_manage_file(t_env *env, char *directory)
{
	t_data_max	*data_max;
	t_list		*list;
	char		*TMP;

	data_max = NULL;
	list = NULL;

	TMP = ft_strinit(directory);
	lstat(TMP, &(env->stats));
	ft_extract_data(env, TMP);
	ft_lstinsert_picker(*env, &list);
	if (!data_max)
	{
		data_max = (t_data_max *)malloc(sizeof(t_data_max));
		ft_data_max_init(data_max);
	}
	ft_data_max_assign(env, data_max);

	ft_print_cont(env, list, data_max, 0);
}

void		ft_manage_dir(t_env *env, char *directory, DIR *cur_dir, int booh)
{
	t_data_max	*data_max;
	t_list		*list;
	char		*TMP;

	data_max = NULL;
	list = NULL;
	while ((env->dir_content = readdir(cur_dir)) != NULL)
	{
		TMP = ft_strinit(ft_join(directory, env->dir_content->d_name, "/"));
		lstat(TMP, &(env->stats));
		if (ft_donot_continue(*env, FILENAME) == 0)
			continue ;
		env->directory = ft_strinit(TMP);
		ft_extract_data(env, FILENAME);
		ft_lstinsert_picker(*env, &list);
		if (!data_max)
		{
			data_max = (t_data_max *)malloc(sizeof(t_data_max));
			ft_data_max_init(data_max);
		}
		ft_data_max_assign(env, data_max);
	}
	closedir(cur_dir);
	if (booh)
		ft_printf("{10}%s{0}:\n", directory);
	ft_print_cont(env, list, data_max, 1);
}

void		ft_explore_dir_here(t_env *env, t_btree *cur_dir)
{
	DIR			*to_explore;
	char		*dirname;

	to_explore = NULL;
	errno = 0;
	if (cur_dir)
	{
		ft_explore_dir_here(env, cur_dir->left);

			dirname = ft_strinit((char *)cur_dir->content);
			lstat(dirname, &(env->stats));
			to_explore = opendir(dirname);
			if (errno != 0)
			{
				char	*filename;

				filename = ft_strinit(ft_strrchr(dirname, '/') + 1);
				ft_printf("{10}%s{0}:\n", dirname);
				ft_printf("ft_ls: %s: %s\n\n", filename, strerror(errno));
				ft_strdel(&filename);
			}
			else
			{
				ft_manage_dir(env, dirname, to_explore, 1);
				ft_recur_launcher(opendir(dirname), env, dirname);
			}
			ft_strdel(&dirname);

		ft_explore_dir_here(env, cur_dir->right);
	}
}

t_btree		*ft_get_dir_here(DIR *cur_dir, t_env *env, char *directory)
{
	t_btree		*dir_here;

	dir_here = NULL;
	while ((env->dir_content = readdir(cur_dir)) != NULL)
	{
		FULL_PATH = ft_strinit(ft_join(directory, env->dir_content->d_name, "/"));
		lstat(FULL_PATH, &(env->stats));
		if (env->FLAGS['R'] && IS_DIRECTORY && IS_NOT_DOT && IS_NOT_DOTDOT)
		{
			if (env->FLAGS['a'] && env->dir_content->d_name[0] == '.')
				ft_cmp(env, &dir_here, FULL_PATH, ft_strlen(FULL_PATH) + 1);
			else if (env->dir_content->d_name[0] != '.')
				ft_cmp(env, &dir_here, FULL_PATH, ft_strlen(FULL_PATH) + 1);
		}
	}
	ft_strdel(&FULL_PATH);
	return (dir_here);
}

void		ft_recur_launcher(DIR *cur_dir, t_env *env, char *directory)
{
	t_btree		*directories_here = NULL;

	lstat(directory, &(env->stats));
 	if (IS_DIRECTORY)
	{
		directories_here = ft_get_dir_here(cur_dir, env, directory);
		ft_explore_dir_here(env, directories_here);
		ft_btreedel(directories_here);
	}
	if (cur_dir != NULL)
		closedir(cur_dir);
}
