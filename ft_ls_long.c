/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_long.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/03 23:27:55 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 23:37:08 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#define MAX lslst->max_datas

static void		ft_print_part_1(t_file_data *data)
{
	ft_printf("%c", data->type);
	ft_printf("%c%c%c", data->usr_r, data->usr_w, data->usr_x);
	ft_printf("%c%c%c", data->grp_r, data->grp_w, data->grp_x);
	ft_printf("%c%c%c", data->oth_r, data->oth_w, data->oth_x);
	ft_printf("%c", data->attrib);
}

static void		ft_print_part_2(t_lslst *lslst, t_file_data *data)
{
	ft_printf("%*d ", MAX.max_link_len + 1, data->hard_link);
	ft_printf("%-*s ", MAX.max_owner_len + 1, data->owner);
	ft_printf("%-*s ", MAX.max_group_len + 1, data->group);
	if (MAX.max_major_len == 0 && MAX.max_minor_len == 0)
	{
		ft_printf("%*d ", MAX.max_size_len, data->size);
	}
	else if (data->type == 'c' || data->type == 'b')
	{
		ft_printf("%*d, ", MAX.max_major_len + 1, data->major);
		ft_printf("%*d ", MAX.max_minor_len, data->minor);
	}
	else
	{
		ft_printf("%*d", MAX.max_minor_len + MAX.max_major_len + 3, data->size);
	}
	ft_printf(" %s %5s ", data->time_day, data->time_hour);
}

void			ft_ls_long(t_env env, t_list *list, t_lslst *lslst)
{
	t_file_data	*data;

	if (lslst->is_dir)
		ft_printf("total %d\n", env.blocks);
	while (list)
	{
		data = ((t_file_data *)list->content);
		if (!env.flags.a && EQU(data->filename, "."))
		{
			list = list->next;
			continue ;
		}
		ft_print_part_1(data);
		ft_print_part_2(lslst, data);

		ft_print_color(env, data);
		ft_putchar('\n');
		list = list->next;
	}
}
