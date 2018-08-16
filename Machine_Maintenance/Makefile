all: application

application:
	gcc -g -o Equipment main.c dialogs.c equipment_backend.c file_handler.c stack.c results.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf *o application
