/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_part_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:01:18 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/20 19:39:28 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_extract_hard_links(t_env *env)
{
	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
	env->data->hard_link = env->stats.st_nlink;
}

void		ft_extract_users_size(t_env *env)
{
	env->data->owner = ft_strinit(getpwuid(env->stats.st_uid)->pw_name);
	env->data->group = ft_strinit(getgrgid(env->stats.st_gid)->gr_name);
	env->data->size = env->stats.st_size;
}

void		ft_extract_time_helper(t_env *env, char *date)
{
	char	*tmp;

	tmp = ft_strtrim(date + 20);
	env->data->time_hour = ft_strjoin(" ", tmp);
	ft_strdel(&tmp);
}

void		ft_extract_time(t_env *env)
{
	char		*date;
	char		*tmp1;
	char		*tmp2;
	time_t		current_time;

	date = ft_strinit(ctime(&env->stats.st_mtime));
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
		ft_extract_time_helper(env, date);
	ft_strdel(&date);
}

void		ft_extract_blocks(t_env *env)
{
	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
	env->blocks += env->stats.st_blocks;
}
