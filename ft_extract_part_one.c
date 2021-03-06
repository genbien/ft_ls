/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_part_one.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:01:18 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 09:57:14 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		ft_extract_filename(t_env *env, char *dirname)
{
	char	*filename;

	if (dirname == NULL)
		filename = ft_strinit(env->dir_content->d_name);
	else
		filename = ft_strinit(dirname);
	if (ft_check_access(*env, filename))
		env->data->filename = ft_strinit(filename);
	else if (S_ISDIR(env->stats.st_mode) && env->options->flags['p'])
		env->data->filename = ft_strjoin(filename, "/");
	else
		env->data->filename = ft_strinit(filename);
	ft_strdel(&filename);
}

static void		ft_extract_type(t_env *env)
{
	char	*dir;

	dir = ft_strinit(env->directory ? env->directory : env->data->filename);
	if (ft_check_access(*env, env->data->filename))
	{
		ft_strdel(&dir);
		return ;
	}
	S_ISDIR(env->stats.st_mode) ? env->data->type = 'd' : 0;
	S_ISREG(env->stats.st_mode) ? env->data->type = '-' : 0;
	S_ISLNK(env->stats.st_mode) ? env->data->type = 'l' : 0;
	S_ISBLK(env->stats.st_mode) ? env->data->type = 'b' : 0;
	S_ISCHR(env->stats.st_mode) ? env->data->type = 'c' : 0;
	S_ISFIFO(env->stats.st_mode) ? env->data->type = 'p' : 0;
	S_ISSOCK(env->stats.st_mode) ? env->data->type = 's' : 0;
	if (env->data->type == 'l')
	{
		env->data->link = ft_strnew(PATH_MAX);
		readlink(dir, env->data->link, PATH_MAX);
	}
	ft_strdel(&dir);
}

static void		ft_extract_perm(t_env *env)
{
	if (ft_check_access(*env, env->data->filename))
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
	if (env->stats.st_mode & S_ISUID)
		env->data->usr_x = env->stats.st_mode & S_IXUSR ? 's' : 'S';
	if (env->stats.st_mode & S_ISGID)
		env->data->grp_x = env->stats.st_mode & S_IXGRP ? 's' : 'S';
	if (env->stats.st_mode & S_ISVTX)
		env->data->oth_x = env->stats.st_mode & S_IXOTH ? 't' : 'T';
}

static void		ft_extract_attributs(t_env *env)
{
	char		*dir;
	acl_t		acl;
	ssize_t		xattr;

	dir = ft_strinit(env->directory ? env->directory : env->data->filename);
	if (ft_check_access(*env, env->data->filename))
	{
		ft_strdel(&dir);
		return ;
	}
	xattr = listxattr(dir, NULL, 0, XATTR_NOFOLLOW);
	acl = acl_get_link_np(dir, ACL_TYPE_EXTENDED);
	if (xattr > 0)
		env->data->attrib = '@';
	else if (acl != NULL)
		env->data->attrib = '+';
	else
		env->data->attrib = ' ';
	ft_strdel(&dir);
}

void			ft_extract_data(t_env *env, char *filename)
{
	ft_init_data(env);
	ft_extract_filename(env, filename);
	ft_extract_type(env);
	ft_extract_perm(env);
	ft_extract_time(env);
	env->FLAGS['l'] ? ft_extract_attributs(env) : 0;
	env->FLAGS['l'] ? ft_extract_hard_links(env) : 0;
	env->FLAGS['l'] ? ft_extract_users_size(env) : 0;
	env->FLAGS['l'] ? ft_extract_blocks(env) : 0;
	env->data->major = major(env->stats.st_rdev);
	env->data->minor = minor(env->stats.st_rdev);
}
