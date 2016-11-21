/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 15:50:53 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/21 13:17:28 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_free_data(t_file_data *data)
{
	ft_strdel(&data->filename);
	ft_strdel(&data->link);
	ft_strdel(&data->owner);
	ft_strdel(&data->group);
	ft_strdel(&data->time);
	ft_strdel(&data->time_day);
	ft_strdel(&data->time_hour);
	data->type = 0;
	data->usr_r = 0;
	data->usr_w = 0;
	data->usr_x = 0;
	data->grp_r = 0;
	data->grp_w = 0;
	data->grp_x = 0;
	data->oth_r = 0;
	data->oth_w = 0;
	data->oth_x = 0;
	data->attrib = 0;
	data->hard_link = 0;
	data->size = 0;
	data->timestamp = 0;
	data->major = 0;
	data->minor = 0;
}

void	ft_free_list(t_list **begin_list)
{
	t_list	*free_list;
	t_list	*temp;

	if (begin_list != NULL)
	{
		free_list = *begin_list;
		while (free_list)
		{
			temp = free_list;
			free_list = free_list->next;
			temp->content_size = 0;
			ft_free_data(temp->content);
			free(temp->content);
			free(temp);
		}
		*begin_list = NULL;
	}
}

void	ft_free_env(t_env *env)
{
	free(env->options);
	free(env->data);
	ft_lstclr(&env->lst_dir);
	ft_lstclr(&env->lst_file);
	ft_lstclr(&env->lst_none);
	ft_lstclr(&env->lst);
}
