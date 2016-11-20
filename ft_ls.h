/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/26 15:30:10 by tbouder           #+#    #+#             */
/*   Updated: 2016/11/20 22:41:15 by tbouder          ###   ########.fr       */
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

# include <errno.h>

# include <sys/xattr.h>
# include <sys/acl.h> //BONUS ACL
# include <sys/ioctl.h> //BONUS affichage colones
# include <termios.h> //BONUS affichage colones

# include "libft/libft.h"

# define FLAGS 			options->flags
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
	long				size;
	char				*time;
	char				*time_day;
	char				*time_hour;
	long 				timestamp;
	long				major;
	long				minor;
}						t_file_data;

typedef struct			s_data_max
{
	int					max_filename_len;
	int					max_owner_len;
	int					max_group_len;
	int					max_link_len;
	long				max_size_len;
	int					max_major_len;
	int					max_minor_len;
	long				blocks;
	int					nb_file;
}						t_data_max;

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
	char				*basedir;
	char				*directory;
	DIR					*dir_fd;
	struct dirent		*dir_content;
	struct s_file_data	*data;
	struct stat			stats;
	struct winsize		w;

	t_btree				*btree;

	t_list				*lst;
	t_list				*lst_dir;
	t_list				*lst_file;
	t_list				*lst_none;

	t_options			*options;

	int					blocks;
}						t_env;

/*
** ft_init.c && ft_free.c
*/
void					ft_init_data(t_env *env);
void					ft_init_env(t_env *env);
void					ft_init_data_max(t_data_max *max);
void					ft_free_data(t_file_data *data);
void					ft_free_list(t_list **begin_list);
void					ft_free_env(t_env *env);


/*
** ft_extract_part : Extract all the needed datas
*/
void					ft_extract_hard_links(t_env *env);
void					ft_extract_users_size(t_env *env);
void					ft_extract_time(t_env *env);
void					ft_extract_blocks(t_env *env);

/*
** Recur : Start the main thread
*/
void					ft_recur_launcher(DIR *cur_dir, t_env *env, char *directory);
void					ft_manage_dir(t_env *env, char *directory, DIR *cur_dir, int booh);
void					ft_manage_file(t_env *env, t_list *lst);

/*
** Btree_cmp
*/
void 					ft_cmp(t_env *env, t_btree **btree, void const *content, size_t c_size);

/*
** lstinsert : Insert each file in each matching directory_list
*/
void		ft_lstinsert_picker(t_env env, t_list **list);

/*
** ft_tools.c
*/
char					*ft_join(char *s1, char *s2, char *divider);
int						ft_perm_denied(t_env env, char *directory);
int						ft_donot_continue(t_env env, char *filename);
int						ft_check_access(t_env env, char *filename);

/*
** ft_get_data.c
*/
void					ft_extract_data(t_env *env, char *filename);
void					ft_assign_data_max(t_env *env, t_data_max *max);


/*
** PRINT
*/
int						ft_print_perm_denied(t_env env, char *directory);
void					ft_print_color(t_env env, t_file_data *data);
void					ft_print_ls(t_env *env, t_list *list, t_data_max *max, int is_dir);
void					ft_print_errno(char *dirname);


/*
** SIMPLE
*/
void					ft_ls_short(t_env env, t_list *list, t_data_max max);
void					ft_ls_one(t_env env, t_list *list);
void					ft_ls_long(t_env env, t_list *list, int is_dir, t_data_max max);

/*
** ARGS
*/
void					ft_sort_args(t_env *env, char **av, int ac);
int						ft_extract_options_ls(char **av, t_env *env);

#endif
