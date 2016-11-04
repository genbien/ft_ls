/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_short.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 19:35:33 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 22:22:50 by tbouder          ###   ########.fr       */
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
		if (!env.flags.a && EQU(((t_file_data *)list->content)->filename, "."))
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
		if (!env.flags.a && EQU(((t_file_data *)list->content)->filename, "."))
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

void		ft_ls_short(t_env env, t_list *list, t_data_max max)
{
	int				nb_column;
	int				elem_line;
	int				pos;
	int				buffer;

	if (max.nb_file == 0)
		return ;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &env.w);
	nb_column = env.w.ws_col / max.max_filename_len;
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
