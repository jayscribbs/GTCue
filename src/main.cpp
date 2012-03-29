// GTKMM-3.0 is in a state of change that will result in some
// changes to this file!

#include <gtkmm.h>
#include "JackClient/JackClient.h"
#include "Gui/MainWindow.h"
#include "Configuration.h"

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  Configuration * config = new Configuration("GTCueConfig");

  JackClient * jc= new JackClient("GTCue", JackNullOption, NULL, config);
  jc->cueFile("/home/josh/Desktop/MProj/Backing/ImYours.wav");
  jc->setPlay();

  MainWindow window;

  Gtk::Main::run(window);

  return EXIT_SUCCESS;
}
