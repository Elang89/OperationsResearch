all: application

application:
	gcc -g -o BST main.c bst_view.c bst_model.c save_module.c file_handler.c dialogs.c -Wall -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf *o application
