/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 12:10:14 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/20 12:14:49 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_print_color(t_file_data *data)
{
	if (data->type == 'd')
		ft_printf("{117}%s{0}", data->filename);
	else if (data->type == 'l')
		ft_printf("{213}%s{0}", data->filename);
	else if (data->type == '-'
		&& (data->usr_x == 'x' || data->grp_x == 'x' || data->oth_x == 'x'))
		ft_printf("{197}%s{0}", data->filename);
	else
		ft_printf("%s", data->filename);
}
