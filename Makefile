NAME		=	chessbot
RM			=	rm -rf
CC			=	c++
CPPFLAGS	=	-Wall  -Wextra -std=c++20 -g #-Werror
HEADERS		=	-I include
SRCS		=	Board.cpp \
				generateMoves.cpp \
				inputLoop.cpp \
				main.cpp \
				search.cpp \
				utils.cpp \

OBJS		=	$(SRCS:%.cpp=$(OBJDIR)/%.o)
SRCDIR		=	src
OBJDIR		=	objs

all:	$(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $^ $(CPPFLAGS) $(HEADERS) -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
