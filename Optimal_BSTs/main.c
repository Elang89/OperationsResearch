#include <gtk/gtk.h>
#include "bst_view.h"


int main(int argc, char * argv[])
{
  gtk_init(NULL, NULL);
  assign_and_display_objects();
  gtk_main();
}
