DIR_Config   = ./lib/Config
DIR_EPD      = ./lib/LCD
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_PROGRAM  = ./program
DIR_MENUS    = ./program/menus
DIR_BIN      = ./bin

OBJ_C = $(wildcard ${DIR_EPD}/*.c ${DIR_Config}/*.c ${DIR_GUI}/*.c ${DIR_PROGRAM}/*.c ${DIR_MENUS}/*.c ${DIR_FONTS}/*.c)

OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = homelabDisplay

LIB = -lgpiod -lm
CC = aarch64-unknown-linux-gnu-gcc # Replace this with aarch64-unknown-linux-gnu-gcc if cross compiling
CFLAGS = -g -O2 -Wall -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) -I $(DIR_PROGRAM) -I $(DIR_MENUS)

${TARGET}: ${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB)

${DIR_BIN}/%.o: ${DIR_EPD}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_Config}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_GUI}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_PROGRAM}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_MENUS}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_FONTS}/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(DIR_BIN)/*.* $(TARGET)
