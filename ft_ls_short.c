/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_short.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 19:35:33 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/16 01:30:49 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			ft_read_buffer(void)
{
	unsigned char   buffer[4];

	if (read(STDIN_FILENO, buffer, 1) > (ssize_t)0)
		return buffer[0];
	return (0);
}

int			ft_get_current_line(void)
{
	int				rows;
	int				result;
	struct termios	saved;
	struct termios	temporary;

	rows = 0;
	tcgetattr(STDIN_FILENO, &saved);
	tcgetattr(STDIN_FILENO, &temporary);
    temporary.c_lflag &= ~(ICANON | ECHO | CREAD);
	tcsetattr(STDIN_FILENO, TCSANOW, &temporary);
	write(STDIN_FILENO, "\033[6n", 4);
	ft_read_buffer();
	ft_read_buffer();
	result = ft_read_buffer();
	while (result >= '0' && result <= '9')
	{
		rows = 10 * rows + result - '0';
		result = ft_read_buffer();
	}
	result = ft_read_buffer();
	while (result >= '0' && result <= '9')
		result = ft_read_buffer();
	tcsetattr(STDIN_FILENO, TCSANOW, &saved);
	return (rows);
}

void		ft_scroll_down(int row, int pos, int elem_line)
{
	int		to_scroll;

	to_scroll = row;
	while (to_scroll <= (pos + elem_line))
	{
		ft_printf("\033D");
		to_scroll++;
	}
	to_scroll = row;
	while (to_scroll <= (pos + elem_line))
	{
		ft_printf("\033[A");
		to_scroll++;
	}
	if ((pos + elem_line) % 2 == 0)
	{
		ft_printf("\033D");
		ft_printf("\033[A");
	}

}

void		ft_scroll_up(int elem_line, int row)
{
	int		pos;
	int		x;

	pos = ft_get_current_line();
	x = 0;
	while ((pos + elem_line + x) > row)
	{
		ft_printf("\033[A");
		x--;
	}
}

void		ft_align_cursor(int elem_line, int nb_file)
{
	int		i;
	int		nb_elem_last_row;

	i = 0;
	nb_elem_last_row = elem_line - (nb_file % elem_line);
	if (nb_elem_last_row != elem_line)
	{
		while (i < nb_elem_last_row)
		{
			ft_printf("\033[B");
			i++;
		}
	}
}

void		ft_print_elem(t_env env, t_file_data *data, int len)
{
	ft_printf(len == 0 ? "\033[500000D" : "\033[500000D\033[%dC", len);
	ft_print_color(env, data);
	ft_printf("\n");
}

void		ft_display_short(t_env env, t_list *list, int elem_line, t_data_max max)
{
	int			i;
	int			len;

	i = 0;
	len = 0;

	while (list)
	{
		if (!env.FLAGS['a'] && EQU(((t_file_data *)list->content)->filename, "."))
		{
			list = list->next;
			continue ;
		}
		ft_print_elem(env, ((t_file_data *)list->content), len);
		list = list->next;
		i++;
		if (i % elem_line == 0 && i != max.nb_file)
		{
			len += max.max_filename_len + 1;
			if (list)
				ft_printf("\033[u");
			ft_scroll_up(elem_line, env.w.ws_row);
		}
	}
}

void		ft_display_short_alte(t_env env, t_list *list, int elem_line, t_data_max max, int buffer)
{
	int			i;
	int			len;

	i = 0;
	len = 0;
	while (list)
	{
		if (!env.FLAGS['a'] && EQU(((t_file_data *)list->content)->filename, "."))
		{
			list = list->next;
			continue ;
		}
		if (i >= buffer && i < buffer + elem_line - (elem_line - env.w.ws_row + 1))
			ft_print_elem(env, ((t_file_data *)list->content), len);
		list = list->next;
		i++;
		if (i == elem_line)
		{
			i = 0;
			len += max.max_filename_len + 1;
			if (list)
				ft_printf("\033[u");
			ft_printf("\033[%dA", elem_line - buffer);
		}
	}
	ft_printf("\033[%dB", elem_line - buffer);
}

void		ft_ls_short_old(t_env env, t_list *list, t_data_max max)
{
	int				nb_column;
	int				elem_line;
	int				pos;
	int				buffer;

	if (max.nb_file == 0)
		return ;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &env.w);
	nb_column = (env.w.ws_col - 3) / max.max_filename_len;
	nb_column == 0 ? nb_column = 1 : 0;
	elem_line = max.nb_file / nb_column + ((max.nb_file % nb_column) ? 1 : 0);
	pos = ft_get_current_line();
	buffer = 0;

	ft_printf("\033[s");
	if (elem_line > env.w.ws_row)
	{
		while (buffer < elem_line)
		{
			ft_display_short_alte(env, list, elem_line, max, buffer);
			ft_printf("\033[s");
			buffer += env.w.ws_row;
		}
	}
	else
	{
		if (pos + elem_line >= env.w.ws_row)
		{
			ft_scroll_down(env.w.ws_row, pos, elem_line);
			pos = ft_get_current_line();
		}
		ft_printf(pos > (env.w.ws_row - elem_line) ? "\033[A\033[s\033[B" : "\033[s");
		ft_display_short(env, list, elem_line, max);
	}
	ft_align_cursor(elem_line, max.nb_file);
}
/******************************************************************************/

t_file_data	**ft_short_helper(t_list *lst, size_t size, int is_a)
{
	t_file_data		**buffer;
	size_t		i;

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
		buffer[i] = (t_file_data *)malloc(sizeof(t_file_data));
		buffer[i] = (t_file_data *)lst->content;
		i++;
		lst = lst->next;
	}
	return (buffer);
}

void		ft_print_color_len(t_file_data *data, int len)
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

void		ft_ls_short(t_env env, t_list *list, t_data_max max)
{
	int				nb_column;
	int				elem_line;
	t_file_data		**all_data;

	if (max.nb_file == 0)
		return ;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &env.w);
	nb_column = (env.w.ws_col - 3) / max.max_filename_len;
	nb_column == 0 ? nb_column = 1 : 0;
	elem_line = max.nb_file / nb_column + ((max.nb_file % nb_column) ? 1 : 0);

	all_data = ft_short_helper(list, max.nb_file, env.FLAGS['a']);
	int		x = 0;
	int		i = 0;
	while (i < elem_line )
	{
		if (all_data && all_data[x])
			ft_print_color_len(all_data[x], max.max_filename_len);
		x += elem_line;
		if (x >= max.nb_file)
		{
			ft_printf("\n");
			i++;
			x = i;
		}
	}
}
