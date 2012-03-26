// GTKMM-3.0 is in a state of change that will result in some
// changes to this file!

#include<gtkmm.h>
#include"JackClient/JackClient.h"
#include"Gui/MainWindow.h"

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  JackClient jc("GTCue", JackNullOption, NULL);
  jc.cueFile("/home/josh/Desktop/MProj/Backing/ImYours.wav");
  jc.setPlay();

  MainWindow window;

  Gtk::Main::run(window);

  return EXIT_SUCCESS;
}
