/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 15:50:53 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/20 15:55:18 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		ft_init_flags(t_env *env)
{
	FLAGS.l = 0;
	FLAGS.rec = 0;
	FLAGS.a = 0;
	FLAGS.r = 0;
	FLAGS.t = 0;
	FLAGS.one = 0;
	FLAGS.two = 0;
}

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

void			ft_init_env(t_env *env)
{
	(!(env->data = (t_file_data *)malloc(sizeof(t_file_data)))) ? exit(1) : 0;
	ft_init_flags(env);
	ft_init_lst(env);
	env->args = 0;
	env->directory = NULL;
	env->dir_fd = NULL;
	env->dir_content = NULL;
	env->max_filename_len = 0;
	env->max_owner_len = 0;
	env->max_group_len = 0;
	env->max_link_len = 0;
	env->max_size_len = 0;
	env->blocks = 0;
	env->nb_file = 0;
}
