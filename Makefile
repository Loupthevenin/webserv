# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/08 17:19:45 by ltheveni          #+#    #+#              #
#    Updated: 2025/03/12 10:24:17 by ltheveni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
_GREY	= \033[30m
_RED	= \033[31m
_GREEN	= \033[32m
_YELLOW	= \033[33m
_BLUE	= \033[34m
_PURPLE	= \033[35m
_CYAN	= \033[36m
_WHITE	= \033[37m
_END	= \033[0m

NAME = webserv
SRC_DIR = srcs/
OBJ_DIR = objs/

# Config
SHELL = /bin/bash
CXX = c++
CXX_INCLUDE = -I includes
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 $(CXX_INCLUDE)
RM = rm -rf
DEBUG_FLAGS = -g3

SRCS = $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Recipe
all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(_END)\nCompiled source files\n"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@printf "$(_GREEN)Finish compiling $(NAME)!\n"
	@printf "Try \"./$(NAME)\" to use$(_END)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "$(_GREEN)█$(_END)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@printf "$(_YELLOW)Removing object files ...$(_END)\n"
	@$(RM) $(OBJ_DIR)

fclean:
	@printf "$(_RED)Removing object files and program ...$(_END)\n"
	@$(RM) $(NAME) $(OBJ_DIR)

re: fclean all

debug: CXXFLAGS += -fsanitize=address $(DEBUG_FLAGS)
debug: re
	@printf "$(_BLUE)Debug build done$(_END)\n"

leak: CXXFLAGS += $(DEBUG_FLAGS)
leak: re
	@printf "$(_BLUE)Leak check build done$(_END)\n"

.PHONY: all clean fclean re debug leak
