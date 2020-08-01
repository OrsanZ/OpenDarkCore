all: ./source/*.c
	gcc -lusb-1.0 -o ./opendarkcore ./source/*.c
