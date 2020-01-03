all: control.o
	write.o
	gcc -o write write.o
	gcc -o control control.o

control.o: control.c
	gcc -c control.c

write.o: write.c
	gcc -c write.c

clean:
	rm *.o
	rm write
    rm control