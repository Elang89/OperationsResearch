all: application

application:
	gcc -g -o ApplicationChooser Main_Window/main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o Floyd Shortest_Paths/shortest_paths.c Shortest_Paths/graph.c Shortest_Paths/open_file.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o PendingW Pending/pending.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o Knapsack Knapsack_Problem/main.c Knapsack_Problem/knapsack_backend.c Knapsack_Problem/file_handler.c Knapsack_Problem/dialogs.c Knapsack_Problem/results.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o gcc -g -o Equipment Machine_Maintenance/main.c Machine_Maintenance/dialogs.c Machine_Maintenance/equipment_backend.c Machine_Maintenance/file_handler.c Machine_Maintenance/stack.c Machine_Maintenance/results.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o Series Sports_Series/main.c Sports_Series/series_view.c Sports_Series/series_model.c Sports_Series/dialogs.c Sports_Series/file_handler.c Sports_Series/results.c  `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o BST Optimal_BSTs/main.c Optimal_BSTs/bst_view.c Optimal_BSTs/bst_model.c Optimal_BSTs/save_module.c Optimal_BSTs/file_handler.c Optimal_BSTs/dialogs.c -Wall -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o MatMult Matrix_Multiplication/main.c Matrix_Multiplication/matrix_model.c Matrix_Multiplication/save_module.c Matrix_Multiplication/load_module.c Matrix_Multiplication/results.c Matrix_Multiplication/dialogs.c -Wall -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	rm -rf *o application
