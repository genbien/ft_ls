/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 15:50:53 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 18:09:32 by tbouder          ###   ########.fr       */
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
	(!(env->o_list = (t_lslst *)malloc(sizeof(t_lslst)))) ? exit(1) : 0;
	env->o_list = NULL;
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
	env->blocks = 0;
}
