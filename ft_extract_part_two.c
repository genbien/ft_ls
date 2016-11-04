/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_part_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:01:18 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/28 12:46:49 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_extract_owner(t_env *env)
{
	env->data->owner = ft_strinit(getpwuid(env->stats.st_uid)->pw_name);
}

void		ft_extract_group(t_env *env)
{
	env->data->group = ft_strinit(getgrgid(env->stats.st_gid)->gr_name);
}

void		ft_extract_size(t_env *env)
{
	env->data->size = env->stats.st_size;
}

void		ft_extract_time(t_env *env)
{
	char		*date;
	char		*tmp1;
	char		*tmp2;
	time_t		current_time;

	date = ctime(&env->stats.st_mtime);
	env->data->timestamp = env->stats.st_mtime;
	current_time = time(NULL);
	tmp1 = ft_strsub(date, 4, 3);
	tmp2 = ft_strsub(date, 8, 2);
	env->data->time_day = ft_join(tmp1, tmp2, " ");
	ft_strdel(&tmp1);
	ft_strdel(&tmp2);
	if (current_time - env->data->timestamp < 15724800
		&& current_time >= env->data->timestamp)
	{
		tmp1 = ft_strsub(date, 11, 2);
		tmp2 = ft_strsub(date, 14, 2);
		env->data->time_hour = ft_join(tmp1, tmp2, ":");
		ft_strdel(&tmp1);
		ft_strdel(&tmp2);
	}
	else
		env->data->time_hour = ft_strsub(date, 20, 4);
}

void		ft_extract_blocks(t_env *env)
{
	env->blocks += env->stats.st_blocks;
}
