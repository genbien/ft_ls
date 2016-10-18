/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_part_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:01:18 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/18 16:02:48 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_extract_owner(t_env *env)
{
	int		ownerlen;

	env->data->owner = ft_strinit(getpwuid(env->stats.st_uid)->pw_name);
	ownerlen = ft_strlen(env->data->owner);
	if (ownerlen > env->max_owner_len)
		env->max_owner_len = ownerlen;
}

void		ft_extract_group(t_env *env)
{
	int		grouplen;

	env->data->group = ft_strinit(getgrgid(env->stats.st_gid)->gr_name);
	grouplen = ft_strlen(env->data->group);
	if (grouplen > env->max_group_len)
		env->max_group_len = grouplen;
}

void		ft_extract_size(t_env *env)
{
	int		sizelen;

	env->data->size = env->stats.st_size;
	sizelen = ft_nbrlen(env->data->size);
	if (sizelen > env->max_size_len)
		env->max_size_len = sizelen;
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
