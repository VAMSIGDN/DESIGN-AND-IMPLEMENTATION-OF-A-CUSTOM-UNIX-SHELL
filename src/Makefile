CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myshell


OBJ = main.o parser.o executor.o \
      cmd_ls.o cmd_cd.o cmd_pwd.o cmd_cat.o \
      cmd_dir.o cmd_cp.o cmd_mv.o cmd_rm.o \
      cmd_touch.o cmd_nano.o cmd_vi.o \
      cmd_tar.o cmd_zip.o cmd_gzip.o \
      cmd_stat.o cmd_file.o cmd_help.o cmd_exit.o \
      cmd_search.o cmd_pager.o cmd_man.o cmd_text_utils.o \
      cmd_process.o cmd_watch.o cmd_signals.o cmd_net.o cmd_alias.o cmd_suggest.o cmd_storage.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) shell_audit.log

re: clean all

.PHONY: all clean re

