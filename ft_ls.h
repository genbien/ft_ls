/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/26 15:30:10 by tbouder           #+#    #+#             */
/*   Updated: 2016/10/20 15:51:41 by tbouder          ###   ########.fr       */
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
# include <sys/ioctl.h> //BONUS affichage colones
# include <termios.h> //BONUS affichage colones

# include "libft/libft.h"

# define FLAGS env->flags


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

typedef struct			s_ls_flag
{
	int					l;
	int					rec;
	int					a;
	int					r;
	int					t;
	int					one;
	int					two; //HIDDEN ONLY
}						t_ls_flag;

typedef struct			s_env
{
	int					args;
	char				*directory;
	DIR					*dir_fd;
	struct dirent		*dir_content;
	struct s_file_data	*data;
	struct stat			stats;

	t_list				*lst;
	t_list				*lst_dir;
	t_list				*lst_file;
	t_list				*lst_none;

	t_ls_flag			flags;

	int					max_filename_len;
	int					max_owner_len;
	int					max_group_len;
	int					max_link_len;
	int					max_size_len;
	int					blocks;
	int					nb_file;
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
void					ft_print_color(t_file_data *data);
t_list					*ft_lstinsert(void const *content, size_t c_size, t_list *next);

/*
** SIMPLE
*/
void					ft_ls_short(t_env env, t_list *list);
void					ft_ls_one(t_list *list);

/*
** ARGS
*/
int						ft_extract_flags(char **av, t_env *env);
void					ft_sort_args(t_env *env, char **av, int ac);

/*
** INIT
*/
void					ft_init_env(t_env *env);

#endif
