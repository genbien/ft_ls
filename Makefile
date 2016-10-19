# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbouder <tbouder@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/04/22 12:12:36 by tbouder           #+#    #+#              #
#    Updated: 2016/10/19 19:38:49 by tbouder          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= 	ft_ls

CC			= 	gcc
CFLAGS		= 	-Wall -Werror -Wextra -g
OPTI		=	-g -O3 -O2

SRC			=	main.c ft_extract_part_one.c ft_extract_part_two.c ft_tools.c ft_ls_short.c
HEAD		=	libft/libft.h

# LIB			=	-Lft_printf -lftprintf
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
