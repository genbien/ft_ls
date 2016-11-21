/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_short.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 19:35:33 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 11:42:35 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_file_data	**ft_short_helper(t_list *lst, size_t size, int is_a)
{
	t_file_data		**buffer;
	size_t			i;

	buffer = NULL;
	if (!(buffer = (t_file_data **)malloc(sizeof(t_file_data *) * (size + 1))))
		return (NULL);
	i = 0;
	while (i < size && lst)
	{
		if (is_a == FALSE && EQU(((t_file_data *)lst->content)->filename, "."))
		{
			lst = lst->next;
			continue;
		}
		buffer[i] = (t_file_data *)lst->content;
		i++;
		lst = lst->next;
	}
	return (buffer);
}

static void			ft_print_color_len(t_env env, t_file_data *data, int len)
{
	if (env.FLAGS['G'])
	{
		if (data->type == 'd')
			ft_printf("{117}%-*s{0} ", len, data->filename);
		else if (data->type == 'l')
			ft_printf("{213}%-*s{0} ", len, data->filename);
		else if (data->type == '-'
			&& (data->usr_x == 'x' || data->grp_x == 'x' || data->oth_x == 'x'))
			ft_printf("{197}%-*s{0} ", len, data->filename);
		else if (data->type == 'c')
			ft_printf("{47}%-*s{0} ", len, data->filename);
		else if (data->type == 'b')
			ft_printf("{191}%-*s{0} ", len, data->filename);
		else
			ft_printf("%-*s ", len, data->filename);
	}
	else
		ft_printf("%-*s ", len, data->filename);
}

static void			ft_print_short(t_env e, t_data_max max, t_list *lst, int l)
{
	t_file_data		**all_data;
	int				i;
	int				x;

	all_data = ft_short_helper(lst, max.nb_file, e.FLAGS['a']);
	i = 0;
	x = 0;
	while (i < l)
	{
		if (all_data && all_data[x])
		{
			ft_print_color_len(e, all_data[x], max.max_filename_len);
			ft_free_data(all_data[x]);
		}
		x += l;
		if (x >= max.nb_file)
		{
			ft_printf("\n");
			i++;
			x = i;
		}
	}
	free(all_data);
}

void				ft_ls_short(t_env env, t_list *list, t_data_max max)
{
	int				nb_column;
	int				elem_line;

	if (max.nb_file == 0)
		return ;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &env.w);
	nb_column = (env.w.ws_col - 3) / max.max_filename_len;
	nb_column == 0 ? nb_column = 1 : 0;
	elem_line = max.nb_file / nb_column + ((max.nb_file % nb_column) ? 1 : 0);
	ft_print_short(env, max, list, elem_line);
}
