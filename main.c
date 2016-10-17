/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/17 21:05:12 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	*ft_join(char *s1, char *s2)
{
	char	*str;

	str = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strcpy(str, s1);
	ft_strcat(str, "/");
	ft_strcat(str, s2);
	return (str);
}

void		ft_extract_type(t_env *env)
{
	S_ISDIR(env->stats.st_mode) ? env->datas->type = 'd' : 0;
	S_ISREG(env->stats.st_mode) ? env->datas->type = '-' : 0;
	S_ISLNK(env->stats.st_mode) ? env->datas->type = 'l' : 0;
	S_ISBLK(env->stats.st_mode) ? env->datas->type = 'b' : 0;
	S_ISCHR(env->stats.st_mode) ? env->datas->type = 'c' : 0;
	S_ISFIFO(env->stats.st_mode) ? env->datas->type = 'p' : 0;
	S_ISSOCK(env->stats.st_mode) ? env->datas->type = 's' : 0;
}

void		ft_extract_perm(t_env *env)
{
	env->datas->usr_r = env->stats.st_mode & S_IRUSR ? 'r' : '-';
	env->datas->usr_w = env->stats.st_mode & S_IWUSR ? 'w' : '-';
	env->datas->usr_x = env->stats.st_mode & S_IXUSR ? 'x' : '-';

	env->datas->grp_r = env->stats.st_mode & S_IRGRP ? 'r' : '-';
	env->datas->grp_w = env->stats.st_mode & S_IWGRP ? 'w' : '-';
	env->datas->grp_x = env->stats.st_mode & S_IXGRP ? 'x' : '-';

	env->datas->oth_r = env->stats.st_mode & S_IROTH ? 'r' : '-';
	env->datas->oth_w = env->stats.st_mode & S_IWOTH ? 'w' : '-';
	env->datas->oth_x = env->stats.st_mode & S_IXOTH ? 'x' : '-';
	env->stats.st_mode & S_ISVTX ? env->datas->oth_x = 't' : 0;
}

void		ft_extract_attributs(t_env *env)
{
	acl_t		acl;
	ssize_t		xattr;

	xattr = listxattr(env->directory, NULL, 0, XATTR_NOFOLLOW);
	acl = acl_get_link_np(env->directory, ACL_TYPE_EXTENDED);

	if (xattr > 0)
		env->datas->attrib = '@';
	else if (acl != NULL)
		env->datas->attrib = '+';
	else
		env->datas->attrib = ' ';
}

void		ft_extract_hard_links(t_env *env)
{
	env->datas->hard_link = env->stats.st_nlink;
}

void		ft_extract_owner(t_env *env)
{
	env->datas->owner = ft_strinit(getpwuid(env->stats.st_uid)->pw_name);
}

void		ft_extract_group(t_env *env)
{
	env->datas->group = ft_strinit(getgrgid(env->stats.st_gid)->gr_name);
}

void		ft_extract_size(t_env *env)
{
	env->datas->size = env->stats.st_size;
}

void		ft_extract_time(t_env *env)
{
	char			*date;
	char			*date_new;
	int				i;
	int				y;

	i = 0;
	y = 0;
	date = ctime(&env->stats.st_mtime);
	env->datas->timestamp = ft_atoi(date);
	date_new = ft_strnew(ft_strlen(date));
	while (date[i++] != ' ')
		;
	while (date[i] != ' ')
		date_new[y++] = date[i++];
	date_new[y++] = date[i++];
	while (date[i] != ' ')
		date_new[y++] = date[i++];
	date_new[y++] = date[i++];
	while (date[i] != ':')
		date_new[y++] = date[i++];
		date_new[y++] = date[i++];
	while (date[i] != ':')
		date_new[y++] = date[i++];
	env->datas->time = ft_strinit(date_new);
}

void		ft_print_result(t_env env)
{
	// %11s => MAX SIZE + 1
	ft_printf("%c%c%c%c%c%c%c%c%c%c%c",
	env.datas->type,
	env.datas->usr_r, env.datas->usr_w, env.datas->usr_x,
	env.datas->grp_r, env.datas->grp_w, env.datas->grp_x,
	env.datas->oth_r, env.datas->oth_w, env.datas->oth_x,
	env.datas->attrib);

	ft_printf("%4d %s %11s %6d %s ",
	env.datas->hard_link,
	env.datas->owner,
	env.datas->group,
	env.datas->size,
	env.datas->time);

	if (env.datas->type == 'd')
		ft_printf("{117}%s{0}\n", env.dir_content->d_name);
	else if (env.datas->type == 'l')
		ft_printf("{213}%s{0}\n", env.dir_content->d_name);
	else if (env.datas->type == '-' && (env.datas->usr_x == 'x' ||
		env.datas->grp_x == 'x'|| env.datas->oth_x == 'x'))
		ft_printf("{197}%s{0}\n", env.dir_content->d_name);
	else
		ft_printf("%s\n", env.dir_content->d_name);
}


void		ft_print_list(t_env env)
{
	t_list	*list;

	list = env.lst;
	while (list)
	{
		ft_printf("[%s]\n", ((t_file_datas *)list->content)->file_name);
		list = list->next;
	}
}

t_list		*ft_lstmiddle(void const *content, size_t c_size, t_list *next)
{
	t_list	*list;

	if (!(list = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	if (content == NULL)
	{
		list->content = NULL;
		list->content_size = 0;
	}
	else
	{
		list->content = (void*)malloc(sizeof(c_size));
		ft_memmove(list->content, (void *)content, c_size);
		list->content_size = c_size;
	}
	list->next = next;
	return (list);
}

void		ft_lstinsert_by(t_list **list, void *content, size_t c_size)
{
	t_list	*new_list;
	char	*content_to_compare;
	char	*content_to_add;
	int		been_inserted = 0;

	content_to_add = ((t_file_datas *)content)->file_name;

	new_list = *list;
	if (new_list)
	{
		while (new_list->next)
		{
			content_to_compare = ((t_file_datas *)new_list->next->content)->file_name;
			if (strcmp(content_to_compare, content_to_add) > 0)
			{
				new_list->next = ft_lstmiddle(content, c_size, new_list->next);
				been_inserted = 1;
				break;
			}
			new_list = new_list->next;
		}
		if (been_inserted == 0)
			new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

void		ft_launcher(t_env env, char *dirname)
{
	while ((env.dir_content = readdir(env.dir_fd)) != NULL)
	{
		env.directory = ft_join(dirname, env.dir_content->d_name);
		lstat(env.directory, &(env.stats));
		env.datas->file_name = ft_strinit(env.dir_content->d_name);

		ft_extract_type(&env);
		ft_extract_perm(&env);
		ft_extract_attributs(&env);
		ft_extract_hard_links(&env);
		ft_extract_owner(&env);
		ft_extract_group(&env);
		ft_extract_size(&env);
		ft_extract_time(&env);
		// ft_print_result(env);
		// ft_lstinsert_by(&env.lst, env.datas, sizeof(env.datas));
		ft_lstinsert_by(&env.lst, env.datas, sizeof(env.datas));
		// ft_print_list(env);

		// ft_printf("----- END LOOP : PUSH {205}%s{0} -----\n", env.datas->file_name);
	}
	ft_print_list(env);
	closedir(env.dir_fd);
}

int			main(int ac, char **av)
{
	t_env	env;
	int		i;

	(!(env.datas = (t_file_datas *)malloc(sizeof(t_file_datas)))) ? exit(1) : 0;
	(!(env.lst = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	env.lst = NULL;

	if (ac == 1)
	{
		env.dir_fd = opendir(".");
		ft_launcher(env, ".");
	}
	else if (ac == 2)
	{
		env.dir_fd = opendir(av[1]);
		ft_launcher(env, av[1]);
	}
	else if (ac > 1)
	{
		i = 1;
		while (i < ac)
		{
			env.dir_fd = opendir(av[i]);
			ft_printf("%s:\n", av[i]);
			ft_launcher(env, av[i]);
			ft_printf("\n");
			i++;
		}
	}
	return (0);
}
