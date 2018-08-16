all: application

application:
	gcc -g -o MatMult main.c matrix_model.c save_module.c load_module.c results.c dialogs.c -Wall -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf *o application
