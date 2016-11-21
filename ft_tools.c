/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:36:46 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 11:21:26 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#define DATA ((t_file_data *)list->content)

int			ft_perm_denied(t_env env, char *directory)
{
	mode_t	mode;

	mode = env.stats.st_mode;
	if (EQU(directory, ".."))
		return (0);
	if (!(mode & S_IRUSR) || ((mode & S_IRUSR) == 0 && (mode & S_IRGRP) == 0))
		return (1);
	return (0);
}

int			ft_donot_continue(t_env env, char *filename)
{
	if (EQU(filename, "."))
		return (1);
	else if (env.FLAGS['2'] && filename[0] != '.')
		return (0);
	else if (!env.FLAGS['2'] && !env.FLAGS['a'] && filename[0] == '.')
		return (0);
	return (1);
}

int			ft_check_access(t_env env, char *filename)
{
	if (env.FLAGS['d'] && EQU(env.basedir, ".") && EQU(filename, "."))
		return (0);
	else if (!env.FLAGS['a'] && EQU(filename, "."))
		return (1);
	return (0);
}

void		ft_lstinsert_picker(t_env env, t_list **list)
{
	if (env.FLAGS['r'] && (env.FLAGS['t'] || env.FLAGS['U']))
		ft_lstinsert_revtm(list, env.data, sizeof(t_file_data));
	else if (env.FLAGS['r'])
		ft_lstinsert_rev(list, env.data, sizeof(t_file_data));
	else if (env.FLAGS['t'] || env.FLAGS['U'])
		ft_lstinsert_time(list, env.data, sizeof(t_file_data));
	else
		ft_lstinsert_ascii(list, env.data, sizeof(t_file_data));
}

void		ft_recur_helper(t_env *env, t_list **list, t_data_max *data_max)
{
	ft_extract_data(env, FILENAME);
	ft_lstinsert_picker(*env, list);
	ft_assign_data_max(env, data_max);
}
