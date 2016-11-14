/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree_sort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/13 19:22:32 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/14 14:23:29 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void ft_btreecmp_revtime(t_btree **tree, void const *content,
			size_t c_size, long timestamp)
{
	struct stat			stats;

    if (!(*tree))
		(*tree) = ft_btreenew(content, c_size);
	else
	{
		lstat((char *)(*tree)->content, &(stats));
	    if (timestamp < stats.st_mtime)
	        ft_btreecmp_revtime(&(*tree)->left, content, c_size, timestamp);
		else if (timestamp >= stats.st_mtime)
	        ft_btreecmp_revtime(&(*tree)->right, content, c_size, timestamp);
	}
}

static void ft_btreecmp_time(t_btree **tree, void const *content,
			size_t c_size, long timestamp)
{
	struct stat			stats;

    if (!(*tree))
		(*tree) = ft_btreenew(content, c_size);
	else
	{
		lstat((char *)(*tree)->content, &(stats));
	    if (timestamp > stats.st_mtime)
	        ft_btreecmp_time(&(*tree)->left, content, c_size, timestamp);
		else if (timestamp <= stats.st_mtime)
	        ft_btreecmp_time(&(*tree)->right, content, c_size, timestamp);
	}
}

void 		ft_cmp(t_env *env, t_btree **btree, void const *content,
			size_t c_size)
{
	if (env->FLAGS['r'] && env->FLAGS['t'])
	{
		lstat((char *)content, &(env->stats));
		ft_btreecmp_revtime(btree, content, c_size, env->stats.st_mtime);
	}
	else if (env->FLAGS['t'])
	{
		lstat((char *)content, &(env->stats));
		ft_btreecmp_time(btree, content, c_size, env->stats.st_mtime);
	}
	else if (env->FLAGS['r'])
		ft_btreecmp_rev(btree, content, c_size);
	else
		ft_btreecmp(btree, content, c_size);
}
