/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 12:10:14 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/17 11:50:18 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#define MAX lslst->max_datas

int			ft_print_perm_denied(t_env env, char *directory)
{
	lstat(directory, &(env.stats));
	if (ft_perm_denied(env, directory) == 1)
	{
		ft_printf("ft_ls: %s: Permission denied\n", directory);
		return (1);
	}
	return (0);
}

void		ft_print_color(t_env env, t_file_data *data)
{
	if (env.FLAGS['G'])
	{
		if (data->type == 'd')
			ft_printf("{117}%s{0}", data->filename);
		else if (env.FLAGS['l'] && data->type == 'l' && !env.FLAGS['1'])
			ft_printf("{213}%s{0} -> %s", data->filename, data->link);
		else if (data->type == 'l')
			ft_printf("{213}%s{0}", data->filename);
		else if (data->type == '-'
			&& (data->usr_x == 'x' || data->grp_x == 'x' || data->oth_x == 'x'))
			ft_printf("{197}%s{0}", data->filename);
		else if (data->type == 'c')
			ft_printf("{47}%s{0}", data->filename);
		else if (data->type == 'b')
			ft_printf("{191}%s{0}", data->filename);
		else
			ft_printf("%s", data->filename);
	}
	else
	{
		if (env.FLAGS['l'] && data->type == 'l' && !env.FLAGS['1'])
			ft_printf("%s -> %s", data->filename, data->link);
		else
			ft_printf("%s", data->filename);
	}
}

void		ft_print_ls(t_env *env, t_list *list, t_data_max *max, int is_dir)
{
	if (env->FLAGS['1'])
		ft_ls_one(*env, list);
	else if (env->FLAGS['l'])
		ft_ls_long(*env, list, is_dir, *max);
	else
		ft_ls_short(*env, list, *max);
	max = NULL;
}

void		ft_print_errno(char *dirname)
{
	char	*filename;
	char	*end;

	end = ft_strrchr(dirname, '/');
	if (end)
	{
		filename = ft_strinit(end + 1);
		ft_printf("%s:\n", dirname);
	}
	else
		filename = ft_strinit(dirname);
	// ft_printf("{10}%s{0}:\n", dirname);
	ft_printf("ft_ls: %s: %s\n", filename, strerror(errno));
	ft_strdel(&filename);
}
