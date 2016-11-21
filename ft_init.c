/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 15:50:53 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 13:27:43 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			ft_init_data(t_env *env)
{
	env->data->filename = NULL;
	env->data->type = 0;
	env->data->usr_r = 0;
	env->data->usr_w = 0;
	env->data->usr_x = 0;
	env->data->grp_r = 0;
	env->data->grp_w = 0;
	env->data->grp_x = 0;
	env->data->oth_r = 0;
	env->data->oth_w = 0;
	env->data->oth_x = 0;
	env->data->attrib = 0;
	env->data->hard_link = 0;
	env->data->link = NULL;
	env->data->owner = NULL;
	env->data->group = NULL;
	env->data->size = 0;
	env->data->time = NULL;
	env->data->time_day = NULL;
	env->data->time_hour = NULL;
	env->data->timestamp = 0;
	env->data->major = 0;
	env->data->minor = 0;
}

void			ft_init_env(t_env *env)
{
	(!(env->data = (t_file_data *)malloc(sizeof(t_file_data)))) ? exit(1) : 0;
	env->lst = NULL;
	env->lst_dir = NULL;
	env->lst_file = NULL;
	env->lst_none = NULL;
	env->args = 0;
	env->directory = NULL;
	env->dir_fd = NULL;
	env->dir_content = NULL;
	env->basedir = NULL;
	env->blocks = 0;
}

void			ft_init_data_max(t_data_max *max)
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

void			ft_assign_data_max(t_env *env, t_data_max *max)
{
	int		value;

	if (ft_check_access(*env, env->data->filename))
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
