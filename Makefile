# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edoardo <edoardo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/23 15:54:22 by edoardo           #+#    #+#              #
#    Updated: 2024/08/24 20:32:02 by edoardo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        		= 	webserver

CC          		= 	c++ -std=c++98
FLAGS       		= 	-Wall -Wextra -Werror
RM          		= 	rm -rf

OBJDIR      		= 	.objFiles

# Paths for source and header files
SRCDIR      		= 	src
LIBDIR      		= 	include
LIBSERVER 			= 	include/ServerConfig
LIBCLIENT 			= 	include/Client
LIBLOGGER 			= 	include/Logger
LIBSIGNAL 			= 	include/Signal
LIBCONFIGANALYSIS 	= 	include/Parsing
LIBEXCEPTION 		= 	include/Exception
LIBUTILS 			= 	include/Utils
LIBHTTP 			= 	include/Http

# Specify source files
FILES       		= 	main WebServer ServerConfig/Server Parsing/Parser Parsing/Lexer \
						Utils/StringUtils Utils/NetworkUtils Utils/FIleUtils Signal/HandleSignal \
						Client/Client Utils/EpollUtils Logger/Logger ServerConfig/ServerConfig ServerConfig/Port \
						ServerConfig/ClientBodySize ServerConfig/Location ServerConfig/CodePath Http/HttpMessage Http/HttpResponse
				
SRC         		= 	$(addprefix $(SRCDIR)/, $(FILES:=.cpp))
OBJ         		= 	$(addprefix $(OBJDIR)/, $(FILES:=.o))

# Specify header files
HEADER      		= 	$(LIBDIR)/WebServer.hpp $(LIBSERVER)/Server.hpp  $(LIBUTILS)/FIleUtils.hpp \
						$(LIBCONFIGANALYSIS)/Parser.hpp $(LIBCONFIGANALYSIS)/Lexer.hpp  \
						$(LIBEXCEPTION)/ErrnoException.hpp $(LIBLOGGER)/Logger.hpp $(LIBSIGNAL)/HandleSignal.hpp \
						$(LIBCLIENT)/Client.hpp $(LIBUTILS)/EpollUtils.hpp $(LIBSERVER)/ServerConfig.hpp \
						$(LIBSERVER)/ClientBodySize.hpp $(LIBSERVER)/CodePath.hpp $(LIBSERVER)/Location.hpp \
						$(LIBUTILS)/NetworkUtils.hpp $(LIBUTILS)/StringUtils.hpp $(LIBSERVER)/Port.hpp \
						$(LIBCLIENT)/ClientConfig.hpp $(LIBHTTP)/HttpStatusCode.hpp $(LIBHTTP)/HttpMessage.hpp \
						$(LIBHTTP)/HttpResponse.hpp

# Include the lib/ directory for header files
INC         		= 	-I$(LIBDIR) -I$(LIBSERVER) -I$(LIBCLIENT) -I$(LIBLOGGER) -I$(LIBSIGNAL) -I$(LIBCONFIGANALYSIS) -I$(LIBEXCEPTION) -I$(LIBUTILS) -I$(LIBHTTP)

NONE        		= "\033[0m"
GREEN       		= "\033[32m"
GRAY        		= "\033[2;37m"
CURSIVE     		= "\033[3m"
YELLOW      		= "\033[1;33m"

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

run: re
	./$(NAME)
	
leaks: re
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=leaks.txt ./$(NAME)

leaks_with_args: re
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=leaks.txt ./$(NAME) 

clean:
	@$(RM) $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)
	@$(RM) leaks.txt
	@$(RM) webserver.log

re: fclean all
