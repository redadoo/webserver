# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edoardo <edoardo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/23 15:54:22 by edoardo           #+#    #+#              #
#    Updated: 2024/08/04 18:15:19 by edoardo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserver

CC          = c++ -std=c++98
FLAGS       = -Wall -Wextra -Werror
RM          = rm -rf

OBJDIR      = .objFiles

# Paths for source and header files
SRCDIR      = src
LIBDIR      = include

# Specify source files
FILES       = main WebServer ServerInfo
SRC         = $(addprefix $(SRCDIR)/, $(FILES:=.cpp))
OBJ         = $(addprefix $(OBJDIR)/, $(FILES:=.o))

# Specify header files
HEADER      = $(LIBDIR)/WebServer.hpp $(LIBDIR)/ServerInfo.hpp

# Include the lib/ directory for header files
INC         = -I$(LIBDIR)

NONE        = "\033[0m"
GREEN       = "\033[32m"
GRAY        = "\033[2;37m"
CURSIVE     = "\033[3m"
YELLOW      = "\033[1;33"

.PHONY: all clean fclean re leaks

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@echo $(CURSIVE)$(GRAY) "     - Making object files..." $(NONE)
	@echo $(CURSIVE) $(GRAY) "     - Compiling $(NAME)..." $(NONE)
	@$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)
	@echo $(GREEN)"- Compiled -"$(NONE)

# Rule to compile .o files from .cpp files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(INC) -c $< -o $@

leaks: re
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=leaks.txt ./$(NAME)

	
leaks_with_args: re
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=leaks.txt ./$(NAME) 

clean:
	@$(RM) $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
