// MainWindow class definition

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "MainWindow.h"

MainWindow::MainWindow (JackClient * jc, Configuration * conf) :
	jackClient (jc),
	config(conf),
	indexFrame("Index"),
	programFrame("Program"),
	currentFileFrame("Cued File"),
	bankFrame("Bank"),
	volumeFrame("Volume"),
	positionFrame("Position"),
	vbox1(Gtk::ORIENTATION_VERTICAL, 5),
	vbox2(Gtk::ORIENTATION_VERTICAL, 5),
	vbox3(Gtk::ORIENTATION_VERTICAL, 5),
	vbox4(Gtk::ORIENTATION_VERTICAL, 5),
	hbox1(Gtk::ORIENTATION_HORIZONTAL, 5),
	hbox2(Gtk::ORIENTATION_HORIZONTAL, 5),
	playButton("Play/Pause"),
	newFileButton("New File This Patch"),
	midiButton("Not Learning Midi"),	
	positionScale(Gtk::ORIENTATION_HORIZONTAL),
	volumeScale(Gtk::ORIENTATION_HORIZONTAL) {

	// Set up the window
	set_title("GT-10 Backing Track Cue");
	set_border_width(10);
	set_default_size(640, 280);

	updateText();

	// Setup playback scale
	positionScale.set_draw_value(false);
	positionScale.set_range(0, 1);
	//FIXME: GTK is currently broken for range widget update policies.......
	//positionScale.set_update_policy(Gtk::UPDATE_DELAYED);
	positionScale.signal_change_value().connect(sigc::mem_fun(*this,
		&MainWindow::positionScaleChanged));
	
	// Setup volume scale
	volumeScale.set_draw_value(false);
	volumeScale.set_range(0,1.2);
	volumeScale.set_value(1);
	volumeScale.add_mark(1, Gtk::POS_TOP, "100%");
	//FIXME (SEE ABOVE)
	volumeScale.signal_change_value().connect(sigc::mem_fun(*this,
		&MainWindow::volumeScaleChanged));

	// Start with horiz boxes
	add(vbox3);
	vbox3.pack_start(vbox1, Gtk::PACK_EXPAND_WIDGET);
	vbox3.pack_start(vbox2, Gtk::PACK_EXPAND_WIDGET);	

	// Establish currently playing section
	currentFileFrame.add(currentFileLabel);
	volumeFrame.add(volumeScale);
	positionFrame.add(positionScale);
	vbox1.pack_start(currentFileFrame, Gtk::PACK_EXPAND_WIDGET);
	vbox1.pack_start(volumeFrame, Gtk::PACK_EXPAND_WIDGET);
	vbox1.pack_start(positionFrame, Gtk::PACK_EXPAND_WIDGET);

	// Divide vbox2 vertically
	vbox2.pack_start(hbox1, Gtk::PACK_EXPAND_WIDGET);
	vbox2.pack_start(hbox2, Gtk::PACK_EXPAND_WIDGET);
	
	// Index labels
	indexFrame.add(indexLabel);
	programFrame.add(programLabel);
	hbox1.pack_start(indexFrame, Gtk::PACK_EXPAND_WIDGET);
	hbox1.pack_start(programFrame, Gtk::PACK_EXPAND_WIDGET);

	// Bank Label
	bankFrame.add(bankLabel);
	
	// Set up buttons
	playButton.signal_clicked().connect(sigc::mem_fun(*this,
		&MainWindow::playButtonClicked));
	newFileButton.signal_clicked().connect(sigc::mem_fun(*this,
		&MainWindow::newFileButtonClicked));
	midiButton.signal_clicked().connect(sigc::mem_fun(*this,
		&MainWindow::midiButtonClicked));
	
	// vbox4 will contain bank label and buttons
	hbox2.pack_start(vbox4, Gtk::PACK_EXPAND_WIDGET);
	vbox4.pack_start(bankFrame, Gtk::PACK_EXPAND_WIDGET);
	vbox4.pack_start(playButton, Gtk::PACK_EXPAND_WIDGET);
	vbox4.pack_start(newFileButton, Gtk::PACK_EXPAND_WIDGET);
	vbox4.pack_start(midiButton, Gtk::PACK_EXPAND_WIDGET);

	show_all_children();

	// Update text and scale when idle
	Glib::signal_idle().connect( sigc::mem_fun(*this, &MainWindow::updateText) );
}

MainWindow::~MainWindow () {}


void MainWindow::playButtonClicked() {
	jackClient->togglePlay();
}

void MainWindow::newFileButtonClicked() {
	Gtk::FileChooserDialog dialog("Choose a new file for this patch...",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	// Handle the data
	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK) {
		jackClient->setPause();
		config->programMap[config->currentIndex][config->currentProgram] = dialog.get_filename();
		config->saveConfig();
		jackClient->cueFile(config->programMap[config->currentIndex][config->currentProgram].c_str());
	}
}

void MainWindow::midiButtonClicked() {
	jackClient->toggleLearningMidi();
}

bool MainWindow::positionScaleChanged(Gtk::ScrollType scroll, double value) {
	jackClient->setPosition(value * jackClient->getLength());
}

bool MainWindow::volumeScaleChanged(Gtk::ScrollType scroll, double value) {
	jackClient->setVolume(value);
}

bool MainWindow::updateText() {
	// Update Text
	stringstream a, b, c;
	a<<config->currentIndex;
	b<<config->currentProgram;
	

	indexLabel.set_text(a.str());
	programLabel.set_text(b.str());
	currentFileLabel.set_text(config->programMap[config->currentIndex][config->currentProgram]);
	
	int d[25][4];
	int ctr = 0;

	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 4; j++) {
			d[i][j] = ctr;
			ctr++;
		}
	}		

	c<<"Bank ";
	switch (config->currentIndex) {
		case 0:
			for (int i = 0; i < 25; i++) {
				for (int j = 0; j < 4; j++) {
					if (config->currentProgram == d[i][j])
						c<<"U"<<i+1<<" Patch "<<j+1;
				}
			}
			break;
		case 1:
			for (int i = 0; i < 25; i++) {
				for (int j = 0; j < 4; j++) {
					if (config->currentProgram == d[i][j])
						c<<"U"<<i+26<<" Patch "<<j+1;
				}
			}
			break;
		case 2:
			for (int i = 0; i < 25; i++) {
				for (int j = 0; j < 4; j++) {
					if (config->currentProgram == d[i][j])
						c<<"P"<<i+1<<" Patch "<<j+1;
				}
			}
			break;
		case 3:
			for (int i = 0; i < 25; i++) {
				for (int j = 0; j < 4; j++) {
					if (config->currentProgram == d[i][j])
						c<<"P"<<i+26<<" Patch "<<j+1;
				}
			}
			break;
	}

	bankLabel.set_text(c.str());

	if (jackClient->isLearning()) midiButton.set_label("Learning Midi");
	else midiButton.set_label("Not Learning Midi");

	// Update Scale
	double value = static_cast <double> (jackClient->getPosition()) /
		static_cast <double> (jackClient->getLength());
	positionScale.set_value(value);

	while (Gtk::Main::events_pending())
		Gtk::Main::iteration(false);
}
