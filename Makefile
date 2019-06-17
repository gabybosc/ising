#Primer script:
#se corre poniendo "make" o "make ising" o "make perc" o "make all"
.PHONY: ising perc all #el primero, ising, es el default, el que corre si solo pongo "make"
ising: ising.e
perc: perc.e
all: ising.e perc.e

	#ahora viene la regla: la regla dice que si le pido un .e, me lo haga a partir de un .c
%.e: %.c
	gcc -Wall -O3 $^ -o $@ -lm


#segundo script
# .PHONY: all clean
# SOURCE = $(wildcard *.c) #ising es el directorio, después dice "todos los .c en el directorio ising"
# EXES = $(pathsubst %.c, %.e, $(SOURCE) )
#
# all: $(EXES) #para cada archivo .c en source, va a crear los .e
# %.e: %.c #la regla pueden ser múltiples líneas
# 	gcc -Wall -O3 $^ -o $@ -lm
#
# clean:
# 	rm $(EXES) #si corro make clean, va a borrar todos los exes.
