#include <gtk/gtk.h>
#include "series_view.h"


int main(int argc, char * argv[])
{
  gtk_init(NULL, NULL);
  assign_and_display_objects();
  gtk_main();
}
