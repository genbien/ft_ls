/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lslstinsert_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/03 15:38:45 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 22:42:24 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#define LSLST_MAX	(*lslst)->max_datas

static void	ft_data_max_init(t_lslst **lslst)
{
	LSLST_MAX.max_filename_len = 0;
	LSLST_MAX.max_group_len = 0;
	LSLST_MAX.max_link_len = 0;
	LSLST_MAX.max_owner_len = 0;
	LSLST_MAX.max_size_len = 0;
	LSLST_MAX.max_minor_len = 0;
	LSLST_MAX.max_major_len = 0;
	LSLST_MAX.nb_file = 0;
}

void		ft_data_max_assign(t_env *env, t_lslst **lslst)
{
	int		value;

	if (!env->flags.a && EQU(env->data->filename, "."))
		return ;
	value = ft_strlen(env->data->filename);
	if (value > LSLST_MAX.max_filename_len)
		LSLST_MAX.max_filename_len = value;
	if (FLAGS.l)
	{
		value = ft_strlen(env->data->group);
		value > LSLST_MAX.max_group_len ? LSLST_MAX.max_group_len = value : 0;

		value = ft_strlen(env->data->owner);
		value > LSLST_MAX.max_owner_len ? LSLST_MAX.max_owner_len = value : 0;

		value = ft_nbrlen(env->data->size);
		value > LSLST_MAX.max_size_len ? LSLST_MAX.max_size_len = value : 0;

		value = ft_nbrlen(env->data->hard_link);
		value > LSLST_MAX.max_link_len ? LSLST_MAX.max_link_len = value : 0;

		value = ft_nbrlen(env->data->major);
		value > LSLST_MAX.max_major_len ? LSLST_MAX.max_major_len = value : 0;

		value = ft_nbrlen(env->data->minor);
		value > LSLST_MAX.max_minor_len ? LSLST_MAX.max_minor_len = value : 0;

	}
	LSLST_MAX.nb_file += 1;
}

t_lslst		*ft_lslstnew(t_env env, char *directory, int is_dir, t_lslst *next)
{
	t_lslst	*list;

	if (!(list = (t_lslst *)malloc(sizeof(t_lslst))))
		return (NULL);
	list->directory = ft_strinit(directory);
	list->is_dir = is_dir;
	ft_data_max_init(&list);
	(!(list->lst = (t_list *)malloc(sizeof(t_list)))) ? exit(1) : 0;
	list->lst = NULL;
	ft_lstinsert_picker(env, list);
	list->next = next;
	return (list);
}

int			ft_loop_part(t_env env, t_lslst *tmp, char *dir, int is_dir)
{
	int		verif;

	verif = 0;
	tmp->next ? lstat(tmp->next->directory, &(env.stats)) : 0;
	if (env.flags.r && env.flags.t && tmp->next
		&& CMP(tmp->next->directory, dir))
		env.stats.st_mtime >= env.data->timestamp ? verif = 1 : 0;
	else if (env.flags.r && tmp->next && CMP(tmp->next->directory, dir) < 0)
		verif = 1;
	else if (env.flags.t && tmp->next && CMP(tmp->next->directory, dir))
		env.stats.st_mtime <= env.data->timestamp ? verif = 1 : 0;
	else if (tmp->next && CMP(tmp->next->directory, dir) > 0)
		verif = 1;
	if (verif == 1)
	{
		tmp->next = ft_lslstnew(env, dir, is_dir, tmp->next);
		ft_data_max_assign(&env, &(tmp->next));
		return (1);
	}
	return (0);
}


t_lslst		*ft_loop_base(t_env env, t_lslst *tmp, char *dir, int is_dir)
{
	while (tmp)
	{
		if (EQU(tmp->directory, dir))
		{
			ft_lstinsert_picker(env, tmp);
			ft_data_max_assign(&env, &tmp);
			return (NULL);
		}
		if (ft_loop_part(env, tmp, dir, is_dir) == 1)
			return (NULL);
		if (!tmp->next)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}
