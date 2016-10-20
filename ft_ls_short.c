/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_short.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 19:35:33 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/20 17:06:58 by tbouder          ###   ########.fr       */
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

void		ft_display_short(t_env env, t_list *list, int elem_line)
{
	t_file_data	*data;
	int			i;
	int			len;

	i = 0;
	len = 0;
	while (i < env.nb_file)
	{
		data = ((t_file_data *)list->content);

		if (len == 0)
			ft_printf("\033[500000D");
		else
			ft_printf("\033[500000D\033[%dC", len);
		ft_print_color(data);
		ft_printf("\n");
		list = list->next;
		i++;
		if (i % elem_line == 0 && i != env.nb_file)
		{
			ft_printf("\033[u");
			len += env.max_filename_len + 1;
		}
	}
}

void		ft_ls_short(t_env env, t_list *list)
{
	struct winsize	w;
	int				nb_column;
	int				elem_line;
	int				pos;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	nb_column = w.ws_col / env.max_filename_len;
	elem_line = env.nb_file / nb_column + ((env.nb_file % nb_column) ? 1 : 0);
	pos = ft_get_current_line();
	if (pos + elem_line >= w.ws_row)
	{
		ft_scroll_down(w.ws_row, pos, elem_line);
		pos = ft_get_current_line();
	}
	if (pos > (w.ws_row - elem_line))
		ft_printf("\033[A\033[s\033[B");
	else
		ft_printf("\033[s");
	ft_display_short(env, list, elem_line);
	ft_align_cursor(elem_line, env.nb_file);
}
