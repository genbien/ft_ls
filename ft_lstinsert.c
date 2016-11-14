/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/02 17:01:37 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/14 15:13:12 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#define DATA_NL 	((t_file_data *)new_list->content)
#define DATA_NNL	((t_file_data *)new_list->next->content)
#define DATA_CU		((t_file_data *)content)

static void		ft_lstinsert_ascii(t_list **list, void *content, size_t c_size)
{
	t_list	*new_list;

	new_list = *list;
	if (new_list && CMP(DATA_NL->filename, DATA_CU->filename) > 0)
		*list = ft_lstinsert(content, c_size, new_list);
	else if (new_list)
	{
		while (new_list->next)
		{
			if (CMP(DATA_NNL->filename, DATA_CU->filename) > 0)
			{
				new_list->next = ft_lstinsert(content, c_size, new_list->next);
				return ;
			}
			new_list = new_list->next;
		}
		new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

static void		ft_lstinsert_time(t_list **list, void *content, size_t c_size)
{
	//IF TIME EQU, ON TEST ASCII
	t_list	*new_list;

	new_list = *list;
	if (new_list && DATA_NL->timestamp <= DATA_CU->timestamp)
	{
		if (DATA_NL->timestamp < DATA_CU->timestamp)
		{
			*list = ft_lstinsert(content, c_size, new_list);
			return ;
		}
		if (CMP(DATA_NL->filename, DATA_CU->filename) > 0)
		{
			*list = ft_lstinsert(content, c_size, new_list);
			return ;
		}
	}
	if (new_list)
	{
		while (new_list->next)
		{
			if (DATA_NNL->timestamp < DATA_CU->timestamp)
			{
				new_list->next = ft_lstinsert(content, c_size, new_list->next);
				return ;
			}
			if (DATA_NNL->timestamp == DATA_CU->timestamp)
			{
				if (CMP(DATA_NNL->filename, DATA_CU->filename) > 0)
				{
					new_list->next = ft_lstinsert(content, c_size, new_list->next);
					return ;
				}
			}
			new_list = new_list->next;
		}
		new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

static void		ft_lstinsert_rev(t_list **list, void *content, size_t c_size)
{
	t_list	*new_list;

	new_list = *list;
	if (new_list && CMP(DATA_NL->filename, DATA_CU->filename) < 0)
		*list = ft_lstinsert(content, c_size, new_list);
	else if (new_list)
	{
		while (new_list->next)
		{
			if (CMP(DATA_NNL->filename, DATA_CU->filename) < 0)
			{
				new_list->next = ft_lstinsert(content, c_size, new_list->next);
				return ;
			}
			new_list = new_list->next;
		}
		new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

static void		ft_lstinsert_revtm(t_list **list, void *content, size_t c_size)
{
	t_list	*new_list;

	new_list = *list;
	if (new_list && DATA_NL->timestamp >= DATA_CU->timestamp)
		*list = ft_lstinsert(content, c_size, new_list);
	else if (new_list)
	{
		while (new_list->next)
		{
			if (DATA_NNL->timestamp >= DATA_CU->timestamp)
			{
				new_list->next = ft_lstinsert(content, c_size, new_list->next);
				return ;
			}
			new_list = new_list->next;
		}
		new_list->next = ft_lstnew(content, c_size);
	}
	else
		*list = ft_lstnew(content, c_size);
}

void			ft_lstinsert_picker(t_env env, t_list **list)
{
	if (env.FLAGS['r'] && env.FLAGS['t'])
		ft_lstinsert_revtm(list, env.data, sizeof(t_file_data));
	else if (env.FLAGS['r'])
		ft_lstinsert_rev(list, env.data, sizeof(t_file_data));
	else if (env.FLAGS['t'])
	{
		// ft_printf("{10}%s{0} : {11}%ld{0}\n", env.data->filename, env.data->timestamp);
		ft_lstinsert_time(list, env.data, sizeof(t_file_data));
	}
	else
		ft_lstinsert_ascii(list, env.data, sizeof(t_file_data));
}
