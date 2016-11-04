/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lslstinsert.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/03 15:32:58 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 16:18:51 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	ft_lslstend_ascii(t_env env, t_lslst **list, char *dir, int is_dir)
{
	t_lslst		*tmp;

	tmp = *list;
	if (tmp && CMP(tmp->directory, dir) > 0)
	{
		*list = ft_lslstnew(env, dir, is_dir, tmp);
		ft_data_max_assign(&env, list);
		return ;
	}
	else if (tmp)
	{
		if ((tmp = ft_loop_base(env, tmp, dir, is_dir)) != NULL)
		{
			tmp->next = ft_lslstnew(env, dir, is_dir, NULL);
			ft_data_max_assign(&env, &(tmp->next));
		}
	}
	else
	{
		*list = ft_lslstnew(env, dir, is_dir, NULL);
		ft_data_max_assign(&env, list);
	}
}

static void	ft_lslstend_time(t_env env, t_lslst **list, char *dir, int is_dir)
{
	t_lslst		*tmp;

	tmp = *list;
	if (tmp && CMP(tmp->directory, dir) > 0
		&& tmp->timestamp <= env.data->timestamp)
	{
		*list = ft_lslstnew(env, dir, is_dir, tmp);
		ft_data_max_assign(&env, list);
	}
	else if (tmp)
	{
		if ((tmp = ft_loop_base(env, tmp, dir, is_dir)) != NULL)
		{
			tmp->next = ft_lslstnew(env, dir, is_dir, NULL);
			ft_data_max_assign(&env, &(tmp->next));
		}
	}
	else
	{
		*list = ft_lslstnew(env, dir, is_dir, NULL);
		ft_data_max_assign(&env, list);
	}
}

static void	ft_lslstend_revtime(t_env env, t_lslst **list, char *dir, int is_dir)
{
	t_lslst		*tmp;

	tmp = *list;
	if (tmp && CMP(tmp->directory, dir) > 0
		&& tmp->timestamp >= env.data->timestamp)
	{
		*list = ft_lslstnew(env, dir, is_dir, tmp);
		ft_data_max_assign(&env, list);
	}
	else if (tmp)
	{
		if ((tmp = ft_loop_base(env, tmp, dir, is_dir)) != NULL)
		{
			tmp->next = ft_lslstnew(env, dir, is_dir, NULL);
			ft_data_max_assign(&env, &(tmp->next));
		}
	}
	else
	{
		*list = ft_lslstnew(env, dir, is_dir, NULL);
		ft_data_max_assign(&env, list);
	}
}

static void	ft_lslstend_reverse(t_env env, t_lslst **list, char *dir, int is_dir)
{
	t_lslst		*tmp;

	tmp = *list;
	if (tmp)
	{
		if ((tmp = ft_loop_base(env, tmp, dir, is_dir)) != NULL)
		{
			tmp->next = ft_lslstnew(env, dir, is_dir, NULL);
			ft_data_max_assign(&env, &(tmp->next));
		}
	}
	else
	{
		*list = ft_lslstnew(env, dir, is_dir, NULL);
		ft_data_max_assign(&env, list);
	}
}

void		ft_lslstinsert_picker(t_env env, t_lslst **list, char *dir, int is_dir)
{
	if (env.flags.r && env.flags.t)
		ft_lslstend_revtime(env, list, dir, is_dir);
	else if (env.flags.r)
		ft_lslstend_reverse(env, list, dir, is_dir);
	else if (env.flags.t)
		ft_lslstend_time(env, list, dir, is_dir);
	else
		ft_lslstend_ascii(env, list, dir, is_dir);
}
