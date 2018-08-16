all: application

application:
	gcc -g -o ApplicationChooser Main_Window/main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o Floyd Shortest_Paths/shortest_paths.c Shortest_Paths/graph.c Shortest_Paths/open_file.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o PendingW Pending/pending.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o Knapsack Knapsack_Problem/main.c Knapsack_Problem/knapsack_backend.c Knapsack_Problem/file_handler.c Knapsack_Problem/dialogs.c Knapsack_Problem/results.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -g -o MachineM Machine_Maintenance/main.c Machine_Maintenance/dialogs.c Machine_Maintenance/equipment_backend.c Machine_Maintenance/file_handler.c Machine_Maintenance/hashmap.c Machine_Maintenance/results.c Machine_Maintenance/stack.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	rm -rf *o application
