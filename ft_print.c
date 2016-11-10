/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 12:10:14 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/10 14:21:38 by tbouder          ###   ########.fr       */
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
	if (data->type == 'd')
		ft_printf("{117}%s{0}", data->filename);
	else if (env.FLAGS['l'] && data->type == 'l')
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
