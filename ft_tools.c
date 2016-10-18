/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:36:46 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/18 16:37:14 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char		*ft_join(char *s1, char *s2, char *divider)
{
	char	*str;

	str = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strcpy(str, s1);
	ft_strcat(str, divider);
	ft_strcat(str, s2);
	return (str);
}

int			ft_is_symb_link(t_env env, char *dirname)
{
	lstat(dirname, &(env.stats));
	if (S_ISLNK(env.stats.st_mode))
		return (1);
	return (0);
}

int			ft_is_dir(t_env env, char *dirname)
{

	lstat(dirname, &(env.stats));
	if (S_ISDIR(env.stats.st_mode))
		return (1);
	return (0);
}
