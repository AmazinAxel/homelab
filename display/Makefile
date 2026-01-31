DIR_LIB      = ./lib
DIR_FONTS    = ./lib/fonts
DIR_PROGRAM  = ./program
DIR_MENUS    = ./program/menus
DIR_BIN      = ./bin

OBJ_C = $(wildcard ${DIR_LIB}/*.c ${DIR_FONTS}/*.c ${DIR_PROGRAM}/*.c ${DIR_MENUS}/*.c ${DIR_BIN}/*.c)
OBJ_O = $(patsubst %.c, ${DIR_BIN}/%.o, $(notdir ${OBJ_C}))
TARGET = homelabDisplay

LIB = -lgpiod -lm
CC = gcc # Replace this with aarch64-unknown-linux-gnu-gcc if cross compiling
CFLAGS = -g -O2 -Wall -I $(DIR_LIB) -I $(DIR_FONTS) -I $(DIR_PROGRAM) -I $(DIR_MENUS)

${TARGET}: ${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB)

${DIR_BIN}/%.o: ${DIR_LIB}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_FONTS}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_PROGRAM}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_MENUS}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(DIR_BIN)/*.* $(TARGET)
