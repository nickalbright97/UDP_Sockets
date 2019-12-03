all: user.c
	gcc message.c user.c -o user
	gcc message.c calculator.c -o calc

clean:
	rm -f user calc

