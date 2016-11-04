/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/26 15:30:10 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/03 23:28:50 by tbouder          ###   ########.fr       */
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

# define FLAGS 			env->flags
# define IS_DIRECTORY	S_ISDIR(env->stats.st_mode)
# define IS_LINK		S_ISLNK(env->stats.st_mode)
# define IS_NOT_DOTDOT	ft_strcmp(env->dir_content->d_name, "..")
# define IS_NOT_DOT		ft_strcmp(env->dir_content->d_name, ".")
# define FILENAME		env->dir_content->d_name
# define FULL_PATH		env->directory

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
	int					major;
	int					minor;
}						t_file_data;

typedef struct			s_data_max
{
	int					max_filename_len;
	int					max_owner_len;
	int					max_group_len;
	int					max_link_len;
	int					max_size_len;
	int					max_major_len;
	int					max_minor_len;
	int					blocks;
	int					nb_file;
}						t_data_max;

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

typedef struct			s_lslst
{
	char				*directory;
	int					is_dir;
	long				timestamp;
	struct s_data_max	max_datas;
	t_list				*lst;
	struct s_lslst		*next;
}						t_lslst;

typedef struct			s_env
{
	int					args;
	char				*directory;
	DIR					*dir_fd;
	struct dirent		*dir_content;
	struct s_file_data	*data;
	struct stat			stats;
	struct winsize		w;

	t_lslst				*o_list;

	t_list				*lst;
	t_list				*lst_dir;
	t_list				*lst_file;
	t_list				*lst_none;

	t_ls_flag			flags;

	int					blocks;
}						t_env;

/*
** Recur : Start the main thread
*/
void		ft_recur_start(DIR *current_directory, t_env *env, char *directory, t_lslst **o_list);

/*
** lstinsert : Insert each file in each matching directory_list
*/
void		ft_lstinsert_picker(t_env env, t_lslst *list);

/*
** lslstinsert + helper : Create new directory_list if new directory
*/
void		ft_lslstinsert_picker(t_env env, t_lslst **list, char *dir, int is_dir);
t_lslst		*ft_lslstnew(t_env env, char *directory, int is_dir, t_lslst *next);
void		ft_data_max_assign(t_env *env, t_lslst **lslst);
int			ft_loop_part(t_env env, t_lslst *tmp, char *dir, int is_dir);
t_lslst		*ft_loop_base(t_env env, t_lslst *tmp, char *dir, int is_dir);

/*
** extract_part : Extract all the needed datas
*/
void		ft_extract_filename(t_env *env, char *dirname);
void		ft_extract_type(t_env *env);
void		ft_extract_perm(t_env *env);
void		ft_extract_attributs(t_env *env);
void		ft_extract_hard_links(t_env *env);
void		ft_extract_owner(t_env *env);
void		ft_extract_group(t_env *env);
void		ft_extract_size(t_env *env);
void		ft_extract_time(t_env *env);
void		ft_extract_blocks(t_env *env);

/*
** tools : Various tools
*/
t_list					*ft_lstinsert(void const *content, size_t c_size, t_list *next);
char					*ft_join(char *s1, char *s2, char *divider);
int						ft_perm_denied(t_env env, char *directory);
int						ft_donot_continue(t_env env, char *filename);
t_lslst					*ft_navigate_o_list(t_lslst **o_list, char *dirtogo);

/*
** PRINT
*/
int						ft_print_perm_denied(t_env env, char *directory);
void					ft_print_color(t_env env, t_file_data *data);
void					ft_print_list(t_env env);

/*
** SIMPLE
*/
void					ft_ls_short(t_env env, t_list *list, t_data_max max);
void					ft_ls_one(t_env env, t_list *list);
void					ft_ls_long(t_env env, t_list *list, t_lslst *lslst);

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
