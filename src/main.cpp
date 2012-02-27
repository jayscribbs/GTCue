#include<gtkmm.h>
#include"JackClient/JackClient.h"
int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  Gtk::Window window;

  Gtk::Main::run(window);

  return EXIT_SUCCESS;
}
