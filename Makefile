# **************************************************************************** #
#																			  #
#														 :::	  ::::::::	#
#	Makefile										   :+:	  :+:	:+:	#
#													 +:+ +:+		 +:+	  #
#	By: luwargni <luwargni@student.42.fr>		  +#+  +:+	   +#+		 #
#												 +#+#+#+#+#+   +#+			#
#	Created: 2018/11/06 19:24:01 by lomasse		   #+#	#+#			  #
#	Updated: 2019/12/08 14:58:52 by luwargni		 ###   ########.fr		#
#																			  #
# **************************************************************************** #

violetfonce=\033[0;35m
violetclair=\033[1;35m
neutre=\033[0m
cyanfonce=\033[0;36m
cyanclair=\033[1;36m
vertfonce=\033[0;32m
vertclair=\033[1;32m
rouge=\033[31m

NAME			= Gif_reader

HEADER 			= $(shell find Includes -type f) $(shell find libraries/include -type f 2>/dev/null || true)

SRC_PATH		= $(shell find Sources -type d)

INC_PATH 		= $(shell find Includes -type d) $(shell find libraries/include -type d 2>/dev/null || true) \

OBJ_PATH		= OBJ

SRC				=	main.c										\
					gif_main.c									\
					init_gif.c									\
					data.c										\
					gif_loop.c									\
					header.c									\

OBJ 			= $(addprefix $(OBJ_PATH)/, $(SRC:%.c=%.o))

LIBS 			= SDL2

LIB_PATH 		= ./libraries/lib \

FRAMEWORK 		= OpenGL AppKit

CC 				= gcc -g -std=c99

vpath %.c $(foreach dir, $(SRC_PATH), $(dir):)

IFLAG			= $(foreach dir, $(INC_PATH), -I$(dir) )

CFLAG 			= -Wall -Wextra -Werror -pthread

LFLAG 			= $(foreach dir, $(LIB_PATH), -L $(dir) ) $(foreach lib, $(LIBS), -l$(lib) ) $(foreach fmw, $(FRAMEWORK), -framework $(fmw) )

IMAGE 			= ./libraries \

DEBUG			= -g3 -fsanitize=address

all: $(NAME)

$(NAME): $(IMAGE) $(OBJ)
	@echo "${vertfonce}Compiling $@ ...${neutre}\c"
	@$(CC) $(CFLAG) -g -o $(NAME) $(OBJ) $(LFLAG) $(DEBUG)
	@echo "${vertclair}DONE${neutre}"

$(OBJ_PATH)/%.o: %.c $(HEADER)
	@mkdir -p $(OBJ_PATH)
	@echo "${violetfonce}Creating $@ ...${neutre}\c"
	@$(CC) $(CFLAG) -o $@ -c $< $(IFLAG)
	@echo "${rose}DONE${neutre}"

$(IMAGE): FORCE
	@if [ -d "./libraries" ]; then \
		echo "${vertfonce}SDL2 is installed.${neutre}"; \
	else \
		make image; \
	fi

FORCE:

clean :
	@echo "${rouge}Cleaning the project ...${neutre}\c"
	@rm -rf $(OBJ_PATH)
	@echo "${rose}DONE${neutre}"

fclean : clean
	@echo "${rouge}Fcleaning the project ...${neutre}\c"
	@rm -rf $(NAME)
	@echo "${rose}DONE${neutre}"

re : fclean all

resdl:
	@echo "${cyanfonce}Deleting ./libraries ...${neutre}\c"
	@rm -rf ./libraries
	@echo "${cyanclair}DONE${neutre}"
	@make image

image: libraries/lib/libSDL2.dylib

libraries/lib/libSDL2.dylib:
	@echo "${cyanfonce}Installing SDL2 ...${neutre}\c"
	@mkdir -p libraries
	@curl -s https://www.libsdl.org/release/SDL2-2.0.8.tar.gz -o libraries/SDL2-2.0.8.tar.gz
	@tar -xf libraries/SDL2-2.0.8.tar.gz -C libraries
	@cd libraries/SDL2-2.0.8 ; ./configure --prefix=$(shell pwd)/libraries
	@make -C ./libraries/SDL2-2.0.8
	@make -C ./libraries/SDL2-2.0.8 install
	@echo "${cyanclair}DONE${neutre}"

.PHONY: all clean fclean re image
