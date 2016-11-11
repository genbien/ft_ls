/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_part_one.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:01:18 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/11 23:17:59 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_extract_filename(t_env *env, char *dirname)
{
	if (dirname == NULL)
		env->data->filename = ft_strinit(env->dir_content->d_name);
	else
		env->data->filename = ft_strinit(dirname);
}

void		ft_extract_type(t_env *env)
{
	char	*dir;

	dir = ft_strinit(env->directory ? env->directory : env->data->filename);
	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
	S_ISDIR(env->stats.st_mode) ? env->data->type = 'd' : 0;
	S_ISREG(env->stats.st_mode) ? env->data->type = '-' : 0;
	S_ISLNK(env->stats.st_mode) ? env->data->type = 'l' : 0;
	S_ISBLK(env->stats.st_mode) ? env->data->type = 'b' : 0;
	S_ISCHR(env->stats.st_mode) ? env->data->type = 'c' : 0;
	S_ISFIFO(env->stats.st_mode) ? env->data->type = 'p' : 0;
	S_ISSOCK(env->stats.st_mode) ? env->data->type = 's' : 0;
	if (env->data->type == 'l')
	{
		env->data->link = ft_strnew(env->stats.st_size + 1);
		readlink(dir, env->data->link, SSIZE_MAX);
	}
}

void		ft_extract_perm(t_env *env)
{
	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
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
	char	*dir;
	acl_t		acl;
	ssize_t		xattr;

	dir = ft_strinit(env->directory ? env->directory : env->data->filename);
	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
	xattr = listxattr(dir, NULL, 0, XATTR_NOFOLLOW);
	acl = acl_get_link_np(dir, ACL_TYPE_EXTENDED);
	if (xattr > 0)
		env->data->attrib = '@';
	else if (acl != NULL)
		env->data->attrib = '+';
	else
		env->data->attrib = ' ';
}

void		ft_extract_hard_links(t_env *env)
{
	if (!env->FLAGS['a'] && EQU(env->data->filename, "."))
		return ;
	env->data->hard_link = env->stats.st_nlink;
}
