/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/26 15:30:10 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/18 00:31:43 by tbouder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H


# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>

# include <time.h>
# include <uuid/uuid.h>
# include <dirent.h>

# include <sys/xattr.h>
# include <sys/acl.h> //BONUS

# include "libft/libft.h"

typedef struct			s_file_data
{
	char				*filename;
	char				type;

	char				usr_r;
	char				usr_w;
	char				usr_x;
	char				grp_r;
	char				grp_w;
	char				grp_x;
	char				oth_r;
	char				oth_w;
	char				oth_x;
	char				attrib;

	int					hard_link;

	char				*owner;
	char				*group;

	int					size;
	char				*time;
	unsigned long		timestamp;
}						t_file_data;

typedef struct			s_env
{
	char				*directory;
	DIR					*dir_fd;
	struct dirent		*dir_content;
	struct s_file_data	*data;
	struct stat			stats;

	t_list				*lst;
}						t_env;

#endif
