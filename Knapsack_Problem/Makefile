all: application

application:
	gcc -g -o Knapsack main.c knapsack_backend.c file_handler.c dialogs.c results.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf *o application
