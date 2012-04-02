// gtkmm top level window
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>

#include "../JackClient/JackClient.h"
#include "../Configuration.h"

class MainWindow : public Gtk::Window {

	public:
		MainWindow(JackClient * jc, Configuration * conf);
		~MainWindow();

		// Update Text
		bool updateText();

		// Button clicked methods
		void playButtonClicked();
		void newFileButtonClicked();
		void midiButtonClicked();

	protected:
		JackClient * jackClient;
		Configuration * config;

		// Buttons
		Gtk::Button 	playButton,
				newFileButton,
				midiButton;

		// Frames
		Gtk::Frame 	indexFrame,
				programFrame,
				currentFileFrame,
				buttonFrame,
				bankFrame;
		
		// Boxes
		Gtk::Box	vbox1,
				vbox2,
				vbox3,
				vbox4,
				hbox1,
				hbox2;

		// Labels
		Gtk::Label	indexLabel,
				programLabel,
				currentFileLabel,
				bankLabel;
};

#endif
