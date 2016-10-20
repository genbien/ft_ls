/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/20 17:29:25 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_print_list(t_env env)
{
	t_list			*list;
	t_file_data		*data;

	list = env.lst;
	data = NULL;
	if (env.flags.l)
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

			ft_print_color(env, data);
			ft_printf("\n");
			list = list->next;
		}
	}
	else if (env.flags.one)
		ft_ls_one(env, list);
	else
		ft_ls_short(env, list);
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
	if (new_list && strcmp(((t_file_data *)new_list->content)->filename, ((t_file_data *)content)->filename) > 0)
		*list = ft_lstinsert(content, c_size, new_list);
	else if (new_list)
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

void		ft_extract_data(t_env *env, char *dirname, int is_away)
{
	ft_extract_filename(env, is_away ? dirname : NULL);

	if (FLAGS.two && env->data->filename[0] != '.')
		return ;
	if (!FLAGS.two && !FLAGS.a && env->data->filename[0] == '.')
		return ;

	ft_extract_type(env);
	ft_extract_perm(env);
	FLAGS.l ? ft_extract_attributs(env) : 0;
	FLAGS.l ? ft_extract_hard_links(env) : 0;
	FLAGS.l ? ft_extract_owner(env) : 0;
	FLAGS.l ? ft_extract_group(env) : 0;
	FLAGS.l ? ft_extract_size(env) : 0;
	FLAGS.l ? ft_extract_time(env) : 0;
	FLAGS.l ? ft_extract_blocks(env) : 0;

	env->nb_file++;
	ft_lstinsert_by(&env->lst, env->data, sizeof(t_file_data));
}

void		ft_extract_launcher(t_env *env, char *dirname, int is_away)
{
	env->directory = ft_strinit(dirname);
	lstat(env->directory, &(env->stats));
	ft_extract_data(env, dirname, is_away);
}


void		ft_launcher(t_env env, char *directory)
{
	if (lstat(directory, &(env.stats)) == -1)
		ft_printf("ft_ls: %s: No such file or directory\n", directory);
	else
	{
		if (S_ISDIR(env.stats.st_mode) || S_ISLNK(env.stats.st_mode))
		{
			env.dir_fd = opendir(directory);
			while ((env.dir_content = readdir(env.dir_fd)) != NULL)
				ft_extract_launcher(&env, ft_join(directory,
					env.dir_content->d_name, "/"), 0);
			(env.args >= 2) ? ft_printf("%s:\n", directory) : 0;
			(env.lst) ? ft_print_list(env) : 0;
			closedir(env.dir_fd);
		}
		else
		{
			ft_extract_launcher(&env, directory, 1);
			ft_print_list(env);
		}
	}
}


int			main(int ac, char **av)
{
	t_env	env;
	int		i;

	ft_init_env(&env);
	i = ft_extract_flags(av, &env);
	env.args = ac - i;

	if (i == ac)
	{
		if ((env.dir_fd = opendir(".")))
			ft_launcher(env, ".");
	}
	else if (ac - 1 >= 1)
	{
		ft_sort_args(&env, av, i);
		while (env.lst_none)
		{
			ft_launcher(env, env.lst_none->content);
			env.lst_none = env.lst_none->next;
		}
		while (env.lst_file)
		{
			ft_launcher(env, env.lst_file->content);
			env.lst_file = env.lst_file->next;
		}
		while (env.lst_dir)
		{
			ft_launcher(env, env.lst_dir->content);
			env.lst_dir = env.lst_dir->next;
			env.lst_dir ? ft_printf("\n") : 0;
		}
	}
	return (0);
}
