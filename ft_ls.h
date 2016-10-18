/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/26 15:30:10 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/18 18:24:30 by tbouder          ###   ########.fr       */
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
	char				*link;

	char				*owner;
	char				*group;

	int					size;
	char				*time;
	char				*time_day;
	char				*time_hour;
	long				timestamp;
}						t_file_data;

typedef struct			s_env
{
	int					args;
	char				*directory;
	DIR					*dir_fd;
	struct dirent		*dir_content;
	struct s_file_data	*data;
	struct stat			stats;
	t_list				*lst;

	int					max_filename_len;
	int					max_owner_len;
	int					max_group_len;
	int					max_link_len;
	int					max_size_len;
	int					blocks;
	int					nb_file;

	int					flag_l;
	int					flag_rec;
	int					flag_a;
	int					flag_r;
	int					flag_t;
}						t_env;

void					ft_extract_filename(t_env *env, char *dirname);
void					ft_extract_type(t_env *env);
void					ft_extract_perm(t_env *env);
void					ft_extract_attributs(t_env *env);
void					ft_extract_hard_links(t_env *env);
void					ft_extract_owner(t_env *env);
void					ft_extract_group(t_env *env);
void					ft_extract_size(t_env *env);
void					ft_extract_time(t_env *env);
void					ft_extract_blocks(t_env *env);

/*
** TOOLS
*/
char					*ft_join(char *s1, char *s2, char *divider);
int						ft_is_symb_link(t_env env, char *dirname);
int						ft_is_dir(t_env env, char *dirname);


#endif
