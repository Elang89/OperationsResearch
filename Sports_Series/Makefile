all: application

application:
	gcc -g -o Series main.c series_view.c series_model.c dialogs.c file_handler.c results.c  `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf *o application
