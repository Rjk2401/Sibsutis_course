# Название компилятора
CC = gcc
# Текущий каталог
TARGET=$(shell basename `pwd`)
#Исходники: в данном случае - все файлы текущего каталога с расширением .c.
SOURCES=$(wildcard *.c)
# Имена объектных файлов получены текстовой заменой в именах исходников
OBJECTS=$(SOURCES:%.c=%.o)
# Флаги для компилятора
CFLAGS =-std=c99 -Wall -Werror -Wdeclaration-after-statement
# Флаги компоновщика (линковщика)
LDFLAGS=-lncurses
# Статические библиотеки
STATIC_LIB=

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o my_manager $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf *.o my_manager
