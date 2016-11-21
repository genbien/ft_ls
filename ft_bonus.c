/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/21 13:36:39 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 13:48:48 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	*ft_select_time_sort(t_env *env)
{
	char	*date;

	if (env->FLAGS['U'])
	{
		date = ft_strinit(ctime(&env->stats.st_birthtime));
		env->data->timestamp = env->stats.st_birthtime;
	}
	if (env->FLAGS['u'])
	{
		date = ft_strinit(ctime(&env->stats.st_atime));
		env->data->timestamp = env->stats.st_atime;
	}
	else if (env->FLAGS['c'])
	{
		date = ft_strinit(ctime(&env->stats.st_ctime));
		env->data->timestamp = env->stats.st_ctime;
	}
	else
	{
		date = ft_strinit(ctime(&env->stats.st_mtime));
		env->data->timestamp = env->stats.st_mtime;
	}
	return (date);
}
