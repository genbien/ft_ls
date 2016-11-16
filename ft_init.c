/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 15:50:53 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/15 11:40:14 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		ft_init_lst(t_env *env)
{
	(!(env->lst = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	(!(env->lst_dir = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	(!(env->lst_file = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	(!(env->lst_none = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	env->lst = NULL;
	env->lst_dir = NULL;
	env->lst_file = NULL;
	env->lst_none = NULL;
}

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
	ft_init_lst(env);
	env->args = 0;
	env->directory = NULL;
	env->dir_fd = NULL;
	env->dir_content = NULL;
	env->blocks = 0;
}
