NAME =			famine

CC =			gcc

FLAGS =			 -nostdlib -m64 -Wall -Wextra -Wno-unused-variable -Wno-return-type -Wno-builtin-declaration-mismatch

HEADERS =		-I ./inc


SRC_DIR =		./src/

OBJ_DIR_NAME =	obj

OBJ_DIR =		./obj/

FILENAMES =	famine

OBJ_PATHS :=	$(addsuffix .o,$(FILENAMES))
OBJ_PATHS :=	$(addprefix $(OBJ_DIR),$(OBJ_PATHS))


.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_PATHS)
	@$(CC) $(OBJ_PATHS) -o $(NAME) $(FLAGS)
	@echo "\033[2K\r\033[0;32m[OK] \033[0m       \033[0;33m $(NAME) created ✅\033[0m"

$(OBJ_PATHS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@/bin/mkdir -p $(OBJ_DIR)
	@echo "\033[2K\r\033[0;32m[OK] \033[0m       \033[0;33m $(NAME) Compiling ✅\033[0m"
	@$(CC) -c $(FLAGS) $< -o $@


clean:
	@echo "\033[0;32m[OK] \033[0m       \033[0;33m Deleting objects in:\033[0m $(NAME)" 
	@/bin/rm -rf $(OBJ_DIR)
	@/usr/bin/find . -name "$(OBJ_DIR_NAME)" -maxdepth 1 -type d -empty -delete

fclean:
	@echo "\033[0;32m[OK] \033[0m       \033[0;33m Deleting objects in:\033[0m $(NAME)" 
	@/bin/rm -rf $(OBJ_DIR)
	@echo "\033[2K\r\033[0;32m[OK] \033[0m       \033[0;33m $(NAME) deleted ✅\033[0m"
	@/bin/rm -f $(NAME)

re: fclean all
