/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/19 02:37:26 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_list	*ft_ext_data(t_list *o_list, int nb)
{
	t_list		*list;
	int			i;

	list = o_list;
	i = 0;
	while (list && i < nb)
	{
		// ft_printf("[%s]\n", ((t_file_data *)list->content)->filename);
		list = list->next;
		i++;
	}
	if (list)
		return (list);
	else
		return (NULL);
}

#include <sys/ioctl.h>
void		ft_print_list(t_env env)
{
	t_list			*list;
	t_file_data		*data;
	// int				i;

	list = env.lst;
	if (env.flag_l)
	{
		ft_printf("total %d\n", env.blocks);
		while (list)
		{
			data = ((t_file_data *)list->content);
			ft_printf("%c%c%c%c%c%c%c%c%c%c%c",
			data->type,
			data->usr_r, data->usr_w, data->usr_x,
			data->grp_r, data->grp_w, data->grp_x,
			data->oth_r, data->oth_w, data->oth_x,
			data->attrib);

			ft_printf("%*d %-*s %-*s %*d %s %5s ",
			env.max_link_len + 1, data->hard_link,
			env.max_owner_len + 1, data->owner,
			env.max_group_len + 1, data->group,
			env.max_size_len, data->size,
			data->time_day, data->time_hour);

			if (data->type == 'd')
				ft_printf("{117}%s{0}\n", data->filename);
			else if (data->type == 'l')
				ft_printf("{213}%s{0} -> %s\n", data->filename, data->link);
			else if (data->type == '-' && (data->usr_x == 'x' ||
				data->grp_x == 'x'|| data->oth_x == 'x'))
				ft_printf("{197}%s{0}\n", data->filename);
			else
				ft_printf("%s\n", data->filename);
			list = list->next;
		}
	}
	else
	{
		// t_list *test;
		// i = 0;
		// while (list)
		// {
		// 	data = ((t_file_data *)list->content);
		//
		// 	ft_printf("%-*s ",env.max_filename_len, data->filename);
		//
		// 	test = ft_ext_data(list, env.nb_file / 4);
		// 	if (test)
		// 		ft_printf("%-*s ", env.max_filename_len, ((t_file_data *)test->content)->filename);
		//
		// 	test = ft_ext_data(list, env.nb_file / 2);
		// 	if (test)
		// 		ft_printf("%-*s ", env.max_filename_len, ((t_file_data *)test->content)->filename);
		//
		// 	test = ft_ext_data(list, env.nb_file - env.nb_file / 4);
		// 	if (test)
		// 		ft_printf("%-*s\n", env.max_filename_len, ((t_file_data *)test->content)->filename);
		//
		// 	// i++;
		// 	list = ft_ext_data(list, 1);
		// 	i += 4;
		// 	if (i >= env.nb_file)
		// 		break ;
		// }

		// while (list)
		// {
			// data = ((t_file_data *)list->content);
			//
			// ft_printf("%-*s ",env.max_filename_len, data->filename);
			//
			// // i++;
			// list = ft_ext_data(list, 1);
			// i += 4;
			// if (i >= env.nb_file)
			// 	break ;

			// data = ((t_file_data *)list->content);
			// ft_printf("%-*s",env.max_filename_len, data->filename);
			// list = list->next;
			//
			// data = ((t_file_data *)list->content);
			// ft_printf("%-*s",env.max_filename_len, data->filename);
			// list = list->next;
			//
			// data = ((t_file_data *)list->content);
			// ft_printf("%-*s",env.max_filename_len, data->filename);
			// list = list->next;
			//
			// data = ((t_file_data *)list->content);
			// ft_printf("%-*s",env.max_filename_len, data->filename);
			// list = list->next;
		// }

			struct winsize	w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

			// w.ws_row

			int		i = 0;
			int		len = -1;
			// int		top = 0;
			int		nb_column = w.ws_col / env.max_filename_len;
			int		nb_line = env.nb_file / nb_column + ((env.nb_file % nb_column) ? 1 : 0);

			// ft_printf("[%d]\n", w.ws_col / 2);
			// ft_printf("[%d]\n", nb_column);
			// ft_printf("[%d]\n", nb_line);

			ft_printf("\033[s");
			while (i < env.nb_file)
			{
				data = ((t_file_data *)list->content);
				ft_printf("\033[G\033[%dC%s\033[B", len, data->filename);
				list = list->next;
				i++;
				if (i % nb_line == 0 && i != env.nb_file)
				{
					// ft_printf("[%d] [%d]\n", i, nb_line);
					ft_printf("\033[u");
					len += env.nb_file;
				}
			}
			// ft_printf("\033[G");



			// while (list)
			// {
			// 	data = ((t_file_data *)list->content);
			//
			// 	len += ft_strlen(data->filename);
			//
			// 	ft_printf("\033[%dB\033[%dD%s", 0, len, data->filename);
			//
			// 	i++;
			// 	if (i == nb_line)
			// 	{
			// 		ft_printf("\n--------");
			// 		round = 5;
			// 		len = -len;
			// 	}
			// 	list = list->next;
			// }

			// exit(1);

// printf("\033[XA"); // Move up X lines;
// printf("\033[XB"); // Move down X lines;
// printf("\033[XC"); // Move right X column;
// printf("\033[XD"); // Move left X column;
// printf("\033[2J"); // Clear screen

			// ft_printf("\033[7C[coucou]\n");
			//
			// ft_printf("00000000000000000000000000000000000000000000000000000000000000000000001");
			// exit(1);
			// list = list->next;
		// ft_printf("\n");

	}
}

/******************************************************************************/

/*TO ADD LIBFT*/
t_list		*ft_lstinsert(void const *content, size_t c_size, t_list *next)
{
	t_list	*list;

	if (!(list = (t_list *)malloc(sizeof(t_list))))
		return (NULL);

	list->content = (void*)malloc(c_size);
	ft_memmove(list->content, (void *)content, c_size);
	list->content_size = c_size;
	list->next = next;
	return (list);
}
/*!TO ADD LIBFT*/

void		ft_lstinsert_by(t_list **list, void *content, size_t c_size)
{
	t_list	*new_list;
	int		been_inserted;

	been_inserted = 0;
	new_list = *list;
	if (new_list)
	{
		while (new_list->next)
		{
			if (strcmp(((t_file_data *)new_list->next->content)->filename,
				((t_file_data *)content)->filename) > 0)
			{
				new_list->next = ft_lstinsert(content, c_size, new_list->next);
				been_inserted = 1;
				break ;
			}
			new_list = new_list->next;
		}
		if (been_inserted == 0)
			new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

/******************************************************************************/

void		ft_done(t_env env)
{
	ft_print_list(env);
}

void		ft_extract_data(t_env *env, char *dirname, int is_away)
{
	env->directory = ft_strinit(dirname);
	lstat(env->directory, &(env->stats));
	if (env->flag_l)
	{
		ft_extract_filename(env, is_away ? dirname : NULL);
		ft_extract_type(env);
		ft_extract_perm(env);
		ft_extract_attributs(env);
		ft_extract_hard_links(env);
		ft_extract_owner(env);
		ft_extract_group(env);
		ft_extract_size(env);
		ft_extract_time(env);
		ft_extract_blocks(env);
	}
	else
	{
		ft_extract_filename(env, is_away ? dirname : NULL);
		ft_extract_type(env);
		ft_extract_perm(env);
	}
	env->nb_file++;
	ft_lstinsert_by(&env->lst, env->data, sizeof(t_file_data));
}

void		ft_launcher(t_env env, char *dirname)
{
	while ((env.dir_content = readdir(env.dir_fd)) != NULL)
		ft_extract_data(&env, ft_join(dirname, env.dir_content->d_name, "/"), 0);

	if (env.lst)
	{
		if (env.args >= 2)
			ft_printf("\n%s:\n", dirname);
		ft_done(env);
	}
}

void		ft_init_env(t_env *env)
{
	(!(env->data = (t_file_data *)malloc(sizeof(t_file_data)))) ? exit(1) : 0;
	(!(env->lst = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	env->lst = NULL;
	env->max_filename_len = 0;
	env->max_owner_len = 0;
	env->max_group_len = 0;
	env->max_link_len = 0;
	env->max_size_len = 0;
	env->blocks = 0;
	env->nb_file = 0;
}

void		ft_verif_args(t_env env, char *directory)
{
	env.dir_fd = opendir(directory);
	if (env.dir_fd > 0 && ft_is_symb_link(env, directory) == 0)
	{
		ft_launcher(env, directory);
		closedir(env.dir_fd);
	}
	else if (env.dir_fd == 0 && lstat(directory, &(env.stats)) != -1)
	{
		ft_extract_data(&env, directory, 1);
		ft_done(env);
	}
	else
		ft_printf("ls: %s: No such file or directory\n", directory);
}

int			ft_extract_flg(char **av, t_env *env)
{
	int		i;
	int		j;
	int		k;

	i = 1;
	k = 0;
	while (av[i] && av[i][0] == '-' && ft_isalpha(av[i][1]))
	{
		j = 1;
		while (av[i][j] != '\0' && ft_isalpha(av[i][j]))
		{
			av[i][j] == 'l' ? env->flag_l = 1 : 0;
			av[i][j] == 'R' ? env->flag_rec = 1 : 0;
			av[i][j] == 'a' ? env->flag_a = 1 : 0;
			av[i][j] == 'r' ? env->flag_r = 1 : 0;
			av[i][j] == 't' ? env->flag_t = 1 : 0;
			j++;
		}
		k++;
		i++;
	}
	return (k);
}

int			main(int ac, char **av)
{
	t_env	env;
	int		i;
	int		y;

	ft_init_env(&env);
	i = ft_extract_flg(av, &env) + 1;
	env.args = ac - i;

	if (ac - i == 0)
	{
		if ((env.dir_fd = opendir(".")))
			ft_launcher(env, ".");
	}
	else if (ac - i == 1)
		ft_verif_args(env, av[ac - i]);
	else if (ac - 1 > 1)
	{
		y = i;
		while (av[y])
		{
			ft_verif_args(env, av[y]);
			y++;
		}
	}
	return (0);
}
