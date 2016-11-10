/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:36:46 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/10 14:13:03 by tbouder          ###   ########.fr       */
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

int			ft_perm_denied(t_env env, char *directory)
{
	mode_t    mode;

	mode = env.stats.st_mode;
	if (EQU(directory, ".."))
		return (0);
	// if (!(mode & S_IRUSR) && !(mode & S_IWUSR) && !(mode & S_IXUSR))
	// 	return (1);
	// else if (!(mode & S_IRGRP) && !(mode & S_IWGRP) && !(mode & S_IXGRP))
	// 	return (1);
	// else if (!(mode & S_IROTH) && !(mode & S_IWOTH) && !(mode & S_IXOTH))
	// 	return (1);
	if (!(mode & S_IRUSR) || ((mode & S_IRUSR) == 0 && (mode & S_IRGRP) == 0))
		return (1);
	return (0);
}

int			ft_donot_continue(t_env env, char *filename)
{
	if (EQU(filename, "."))
		return (1);
	else if (env.FLAGS['2'] && filename[0] != '.')
		return (0);
	else if (!env.FLAGS['2'] && !env.FLAGS['a'] && filename[0] == '.')
		return (0);
	return (1);
}
