# Nombre del ejecutable
TARGET = interprete

# Archivos fuente
SRCS = main.c lexer.c parser.c eval.c

# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -std=c99

# Regla por defecto
all: $(TARGET)

# Compilar ejecutable a partir de objetos
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Compilar cada archivo .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJS) $(TARGET)