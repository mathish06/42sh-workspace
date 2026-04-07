##
## EPITECH PROJECT, 2026
## Makefile
## File description:
## makefile
##

MAKEFLAGS += --no-print-directory

CC      =   epiclang

PROJECT =   MYSH
NAME    =   mysh
LIB_NAME =  libmy.a

# 2. SOURCES
SRC     =   main.c							\
			src/exec_command.c				\
			src/manage_list.c				\
			src/mysh.c						\
			src/utils.c						\
			src/builtins.c					\
			src/error_handling.c			\
			src/my_cd.c						\
			src/parsing.c					\
			src/redirects.c					\
			src/pipe.c

LIB_SRC =   lib/my/my_put_nbr.c				\
			lib/my/my_putchar.c				\
			lib/my/my_putstr.c				\
			lib/my/my_str_to_word_array.c	\
			lib/my/my_strcat.c				\
			lib/my/my_strcmp.c				\
			lib/my/my_strcpy.c				\
			lib/my/my_strdup.c				\
			lib/my/my_strlen.c				\
			lib/my/my_strncpy.c				\
			lib/my/my_strndup.c

# 3. OBJETS
TESTS_SRC = tests/unit_tests.c
OBJ     =   $(SRC:.c=.o)
LIB_OBJ =   $(LIB_SRC:.c=.o)

# 4. FLAGS
CFLAGS  =   -I./include -Wall -Wextra
LDFLAGS =   -L. -lmy -lncurses
CSFML = -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lm
TESTS_FLAGS = --coverage -lcriterion

# ─── Macro: Fancy Header ─────────────────────────────────────────────────
define pretty_header
	@echo "$(BOLD)$(H_PURPLE)╔═══════════════════════════════════════════════════════════════╗$(END)"
	@echo "$(BOLD)$(H_PURPLE)║$(END)$(BOLD)$(WHITE)$(1)$(END)"
	@echo "$(BOLD)$(H_PURPLE)╚═══════════════════════════════════════════════════════════════╝$(END)"
endef

# ─── Pattern Rules (Esthétique de compilation) ───────────────────────────
%.o: %.c
	@printf "$(H_BLUE)⚡ $(BOLD)Compiling:$(END) $(H_YELLOW)%-45s$(END) $(H_GREEN)[OK]$(END)\r" $<
	@$(CC) $(CFLAGS) -c $< -o $@

# ─── Rules ───────────────────────────────────────────────────────────────
.PHONY: all clean fclean re tests_run coverage help banner

all: banner $(LIB_NAME) $(NAME)

$(NAME): $(OBJ)
	@printf "\n"
	$(call pretty_header, 🚀 Linking binary : $(NAME) 🚀)
	@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)
	@echo "$(BOLD)$(H_GREEN)✅ $(PROJECT) successfully compiled!$(END)"

$(LIB_NAME): $(LIB_OBJ)
	@printf "\n"
	$(call pretty_header, 📚 Creating library : $(LIB_NAME) 📚)
	@ar rc $(LIB_NAME) $(LIB_OBJ)
	@cp $(LIB_NAME) lib/my/
	@echo "$(BOLD)$(H_GREEN)✅ Library compiled!$(END)"

banner:
	@echo "$(H_CYAN)                                                              $(END)"
	@echo "$(H_CYAN) ███╗   ███╗██╗   ██╗███████╗██╗  ██╗                        $(END)"
	@echo "$(H_CYAN) ████╗ ████║╚██╗ ██╔╝██╔════╝██║  ██║                        $(END)"
	@echo "$(H_CYAN) ██╔████╔██║ ╚████╔╝ ███████╗███████║                        $(END)"
	@echo "$(H_CYAN) ██║╚██╔╝██║  ╚██╔╝  ╚════██║██╔══██║                        $(END)"
	@echo "$(H_CYAN) ██║ ╚═╝ ██║   ██║   ███████║██║  ██║                        $(END)"
	@echo "$(H_CYAN) ╚═╝     ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝                        $(END)"
	@echo "$(H_CYAN)                                                              $(END)"

clean:
	$(call pretty_header, 🧹 Cleaning object files 🧹)
	@rm -f $(OBJ)
	@rm -f $(LIB_OBJ)
	@rm -f *.gcno *.gcda

fclean: clean
	$(call pretty_header, 🗑️ Full clean: objects binary tests 🗑️)
	@rm -f $(NAME)
	@rm -f $(LIB_NAME)
	@rm -f lib/my/$(LIB_NAME)
	@rm -f unit_tests
	@rm -f coverage.html

re: fclean all

# ─── Tests Rules ─────────────────────────────────────────────────────────
tests_run: fclean $(LIB_NAME)
	$(call pretty_header, 🧪 Compiling and running unit tests 🧪)
	@$(CC) -o unit_tests $(filter-out main.c, $(SRC)) $(TESTS_SRC) $(CFLAGS) $(LDFLAGS) $(TESTS_FLAGS)
	@./unit_tests
	@gcovr --exclude tests/
	@gcovr --branches --exclude tests/

coverage: tests_run
	@gcovr --html-details -o coverage.html --exclude tests/
	@xdg-open coverage.html || echo "Coverage generated in coverage.html"

# ─── Help Rule ───────────────────────────────────────────────────────────
help:
	@echo ""
	@echo "$(BOLD)$(CYAN)╔══════════════════════════════════════════════════════════════════════════╗$(END)"
	@echo "$(BOLD)$(CYAN)║                          $(H_WHITE)MAKEFILE HELP MENU$(CYAN)                              ║$(END)"
	@echo "$(BOLD)$(CYAN)╠══════════════════════════════════════════════════════════════════════════╣$(END)"
	@echo "$(BOLD)$(CYAN)║$(END) $(BOLD)$(H_GREEN)Project:$(END) $(YELLOW)$(PROJECT)$(END)                                                           $(BOLD)$(CYAN) ║$(END)"
	@echo "$(BOLD)$(CYAN)╚══════════════════════════════════════════════════════════════════════════╝$(END)"
	@echo ""
	@echo "  $(H_GREEN)make$(END)           Build the library and the executable"
	@echo "  $(H_GREEN)make clean$(END)     Remove object files"
	@echo "  $(H_GREEN)make fclean$(END)    Remove object files and binaries"
	@echo "  $(H_GREEN)make re$(END)        Rebuild completely"
	@echo "  $(H_GREEN)make tests_run$(END) Run unit tests with Criterion"
	@echo "  $(H_GREEN)make coverage$(END)  Generate HTML coverage report"
	@echo ""

# ─── Styles & Colors ─────────────────────────────────────────────────────
END = \033[0m
BOLD = \033[1m
H_GREEN = \033[92m
H_YELLOW = \033[93m
H_BLUE = \033[94m
H_PURPLE = \033[95m
H_CYAN = \033[96m
WHITE = \033[37m
CYAN = \033[36m
YELLOW = \033[33m