/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recur.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:04:31 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 23:21:54 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	ft_extract_data(t_env *env, char *filename)
{
	ft_extract_filename(env, filename);
	ft_extract_type(env);
	ft_extract_perm(env);
	ft_extract_time(env);
	FLAGS.l ? ft_extract_attributs(env) : 0;
	FLAGS.l ? ft_extract_hard_links(env) : 0;
	FLAGS.l ? ft_extract_owner(env) : 0;
	FLAGS.l ? ft_extract_group(env) : 0;
	FLAGS.l ? ft_extract_size(env) : 0;
	FLAGS.l ? ft_extract_blocks(env) : 0;
	env->data->major = major(env->stats.st_rdev);
	env->data->minor = minor(env->stats.st_rdev);
}

void		ft_recur_start(DIR *cur_dir, t_env *env, char *directory, t_lslst **lslst)
{
	t_lslst		*list_node;

	lstat(directory, &(env->stats));
	if (ft_perm_denied(*env, directory) == 1)
	{
		ft_extract_data(env, directory);
		ft_lslstinsert_picker(*env, lslst, directory, 0);
		return ;
	}
 	if (IS_DIRECTORY)
	{
		while ((env->dir_content = readdir(cur_dir)) != NULL)
		{
			FULL_PATH = ft_strinit(ft_join(directory, FILENAME, "/"));
			lstat(FULL_PATH, &(env->stats));
			if (ft_donot_continue(*env, FILENAME) == 0)
				continue ;
			ft_extract_data(env, FILENAME);
			ft_lslstinsert_picker(*env, lslst, directory, 1);
			if (env->flags.rec && IS_DIRECTORY && IS_NOT_DOTDOT && IS_NOT_DOT)
			{
				list_node = ft_navigate_o_list(lslst, directory);
				list_node->timestamp = env->data->timestamp;
				ft_recur_start(opendir(FULL_PATH), env, FULL_PATH, &list_node);
			}
		}
		closedir(cur_dir);
	}
	else
	{
		ft_extract_data(env, directory);
		ft_lslstinsert_picker(*env, lslst, directory, 0);
	}
}
