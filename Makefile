NAME		=	chessbot
TEST		=	tester
RM			=	rm -rf
CC			=	c++
CPPFLAGS	=	-Wall -Wextra -std=c++20 -O3 -flto -march=native -mtune=native \
				-funroll-loops -fomit-frame-pointer -fno-exceptions -fno-rtti #-g #-Werror
HEADERS		=	-I include
SRCDIR		=	src
OBJDIR		=	objs

SRCS		=	Board.cpp \
				BoardBitOperations.cpp \
				BoardCheckChecks.cpp \
				BoardExecute.cpp \
				BoardGenerateMoves.cpp \
				inputLoop.cpp \
				Move.cpp \
				Position.cpp \
				search.cpp \

MAIN		=	main.cpp
TESTMAIN	=	tests.cpp

OBJS		=	$(SRCS:%.cpp=$(OBJDIR)/%.o)
MAINOBJ		=	$(MAIN:%.cpp=$(OBJDIR)/%.o)
T_MAIN		=	$(TESTMAIN:%.cpp=$(OBJDIR)/%.o)

all:	$(NAME)

test:	$(TEST)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS) $(MAINOBJ)
	$(CC) $(CPPFLAGS) $(OBJS) $(MAINOBJ) $(HEADERS) -o $(NAME)

$(TEST): $(OBJDIR) $(OBJS) $(T_MAIN)
	$(CC) $(CPPFLAGS) $(OBJS) $(T_MAIN) $(HEADERS) -o $(TEST)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $^ $(CPPFLAGS) $(HEADERS) -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TEST)

re: fclean all

retest: fclean test

.PHONY: all clean fclean re
