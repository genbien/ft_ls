# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/04/22 12:12:36 by tbouder           #+#    #+#              #
#    Updated: 2016/11/20 19:42:13 by tbouder          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= 	ft_ls

CC			= 	gcc
CFLAGS		= 	-Wall -Werror -Wextra
OPTI		=	-g -O3

SRC			=	main.c \
				ft_free.c ft_init.c\
				ft_extract_part_one.c ft_extract_part_two.c ft_tools.c \
				ft_ls_short.c ft_ls_one.c ft_ls_long.c\
				ft_print.c ft_args.c \
				ft_recur.c \
				ft_lstinsert.c\
				ft_btree_sort.c


HEAD		=	libft/libft.h

LIB			=	libft/libft.a

OBJ			=	$(patsubst %.c,%.o, $(SRC))

JUNCK		=	$(wildcard *.gch */*.gch .DS_Store */.DS_Store)
DIRJUNCK	=	ft_ls.dSYM


all: $(NAME)

.SILENT : $(NAME) $(OBJ)
$(NAME): $(OBJ) $(HEAD)
	# $(MAKE) re -C libft
	$(CC) $(CFLAGS) $(OPTI) -c $(SRC)
	$(CC) -o $@ $(OBJ) $(LIB)

.SILENT : clean
clean:
	# $(MAKE) clean -C libft
	rm -f $(notdir $(OBJ)) $(OBJ) $(JUNCK)
	rm -rf $(DIRJUNCK)

.SILENT : fclean
fclean: clean
	# $(MAKE) fclean -C libft
	rm -f $(NAME)

re: fclean all
