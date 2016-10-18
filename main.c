/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 12:15:26 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/18 12:21:59 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char		*ft_join(char *s1, char *s2)
{
	char	*str;

	str = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strcpy(str, s1);
	ft_strcat(str, "/");
	ft_strcat(str, s2);
	return (str);
}

void		ft_extract_filename(t_env *env)
{
	int		filelen;

	env->data->filename = ft_strinit(env->dir_content->d_name);
	filelen = ft_strlen(env->data->filename);
	if (filelen > env->max_filename_len)
		env->max_filename_len = filelen;
}

void		ft_extract_type(t_env *env)
{
	S_ISDIR(env->stats.st_mode) ? env->data->type = 'd' : 0;
	S_ISREG(env->stats.st_mode) ? env->data->type = '-' : 0;
	S_ISLNK(env->stats.st_mode) ? env->data->type = 'l' : 0;
	S_ISBLK(env->stats.st_mode) ? env->data->type = 'b' : 0;
	S_ISCHR(env->stats.st_mode) ? env->data->type = 'c' : 0;
	S_ISFIFO(env->stats.st_mode) ? env->data->type = 'p' : 0;
	S_ISSOCK(env->stats.st_mode) ? env->data->type = 's' : 0;
}

void		ft_extract_perm(t_env *env)
{
	env->data->usr_r = env->stats.st_mode & S_IRUSR ? 'r' : '-';
	env->data->usr_w = env->stats.st_mode & S_IWUSR ? 'w' : '-';
	env->data->usr_x = env->stats.st_mode & S_IXUSR ? 'x' : '-';

	env->data->grp_r = env->stats.st_mode & S_IRGRP ? 'r' : '-';
	env->data->grp_w = env->stats.st_mode & S_IWGRP ? 'w' : '-';
	env->data->grp_x = env->stats.st_mode & S_IXGRP ? 'x' : '-';

	env->data->oth_r = env->stats.st_mode & S_IROTH ? 'r' : '-';
	env->data->oth_w = env->stats.st_mode & S_IWOTH ? 'w' : '-';
	env->data->oth_x = env->stats.st_mode & S_IXOTH ? 'x' : '-';
	env->stats.st_mode & S_ISVTX ? env->data->oth_x = 't' : 0;
}

void		ft_extract_attributs(t_env *env)
{
	acl_t		acl;
	ssize_t		xattr;

	xattr = listxattr(env->directory, NULL, 0, XATTR_NOFOLLOW);
	acl = acl_get_link_np(env->directory, ACL_TYPE_EXTENDED);

	if (xattr > 0)
		env->data->attrib = '@';
	else if (acl != NULL)
		env->data->attrib = '+';
	else
		env->data->attrib = ' ';
}

void		ft_extract_hard_links(t_env *env)
{
	env->data->hard_link = env->stats.st_nlink;
}

void		ft_extract_owner(t_env *env)
{
	int		ownerlen;

	env->data->owner = ft_strinit(getpwuid(env->stats.st_uid)->pw_name);
	ownerlen = ft_strlen(env->data->owner);
	if (ownerlen > env->max_owner_len)
		env->max_owner_len = ownerlen;
}

void		ft_extract_group(t_env *env)
{
	int		grouplen;

	env->data->group = ft_strinit(getgrgid(env->stats.st_gid)->gr_name);
	grouplen = ft_strlen(env->data->group);
	if (grouplen > env->max_group_len)
		env->max_group_len = grouplen;
}

void		ft_extract_size(t_env *env)
{
	env->data->size = env->stats.st_size;
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
	env->data->timestamp = ft_atoi(date);
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
	env->data->time = ft_strinit(date_new);
}

void		ft_print_list(t_env env)
{
	t_list			*list;
	t_file_data		*data;

	list = env.lst;
	while (list)
	{
		data = ((t_file_data *)list->content);
		ft_printf("%c%c%c%c%c%c%c%c%c%c %c",
		data->type,
		data->usr_r, data->usr_w, data->usr_x,
		data->grp_r, data->grp_w, data->grp_x,
		data->oth_r, data->oth_w, data->oth_x,
		data->attrib);

		ft_printf("%4d %-*s %-*s %d %s",
		data->hard_link,

		env.max_owner_len + 1,
		data->owner,

		env.max_group_len + 1,
		data->group,

		data->size, data->time);

		if (data->type == 'd')
			ft_printf("{117}%s{0}\n", data->filename);
		else if (data->type == 'l')
			ft_printf("{213}%s{0}\n", data->filename);
		else if (data->type == '-' && (data->usr_x == 'x' ||
			data->grp_x == 'x'|| data->oth_x == 'x'))
			ft_printf("{197}%s{0}\n", data->filename);
		else
			ft_printf("%s\n", data->filename);
		list = list->next;
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

void		ft_launcher(t_env env, char *dirname)
{
	while ((env.dir_content = readdir(env.dir_fd)) != NULL)
	{
		env.directory = ft_join(dirname, env.dir_content->d_name);
		lstat(env.directory, &(env.stats));

		ft_extract_filename(&env);
		ft_extract_type(&env);
		ft_extract_perm(&env);
		ft_extract_attributs(&env);
		ft_extract_hard_links(&env);
		ft_extract_owner(&env);
		ft_extract_group(&env);
		ft_extract_size(&env);
		ft_extract_time(&env);

		ft_lstinsert_by(&env.lst, env.data, sizeof(t_file_data));
	}
	ft_printf("MAX FILENAME : [%d]\n", env.max_filename_len);
	ft_printf("MAX OWNER : [%d]\n", env.max_owner_len);
	ft_printf("MAX GROUP : [%d]\n", env.max_group_len);
	ft_print_list(env);
	closedir(env.dir_fd);
}

int			main(int ac, char **av)
{
	t_env	env;
	int		i;

	(!(env.data = (t_file_data *)malloc(sizeof(t_file_data)))) ? exit(1) : 0;
	(!(env.lst = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	env.lst = NULL;
	env.max_filename_len = 0;
	env.max_owner_len = 0;
	env.max_group_len = 0;

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