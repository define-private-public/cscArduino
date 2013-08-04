// File:         rgb_led_driver.cpp
// Author:       Benjamin  Summerton  <define-private-public@github.com>
// Description:  Sends three bytes to the Arduino to control an RGB.
//
//               Requires gtkmm to compile and run.


#include <gtkmm.h>
#include "csc_arduino.h"


// RGB Window class
class RGBWindow : public Gtk::Window {
public:
	RGBWindow(char *port, int baudrate);
	virtual ~RGBWindow();

protected:
	// Member widgets
	Gtk::Button quit_button;
	Gtk::HScale r_scale;
	Gtk::HScale g_scale;
	Gtk::HScale b_scale;
	Gtk::Label r_label;
	Gtk::Label g_label;
	Gtk::Label b_label;
	Gtk::Label hex_color_label;

	// Signal handlers
	void on_button_quit();
	void on_scale_change();
	
	// Layout
	Gtk::Grid grid;;

	// Arduino
	csc::Arduino arduino;
};


RGBWindow::RGBWindow(char *port, int baudrate) :
		quit_button("Quit"),
		r_label("R: "),
		g_label("G: "),
		b_label("B: "),
		hex_color_label("#000000"),
		arduino(port, baudrate) {
	// Setp the application

	// The scales
	r_scale.set_range(0, 255);
	r_scale.set_increments(1, 5);
	r_scale.set_digits(0);
	r_scale.set_size_request(256, 20);

	g_scale.set_range(0, 255);
	g_scale.set_increments(1, 5);
	g_scale.set_digits(0);
	g_scale.set_size_request(256, 20);

	b_scale.set_range(0, 255);
	b_scale.set_increments(1, 5);
	b_scale.set_digits(0);
	b_scale.set_size_request(256, 20);

	// Connect the signals
	quit_button.signal_clicked().connect(sigc::mem_fun(*this,
		&RGBWindow::on_button_quit));
	r_scale.signal_value_changed().connect(sigc::mem_fun(*this,
		&RGBWindow::on_scale_change));
	g_scale.signal_value_changed().connect(sigc::mem_fun(*this,
		&RGBWindow::on_scale_change));
	b_scale.signal_value_changed().connect(sigc::mem_fun(*this,
		&RGBWindow::on_scale_change));

	// Add them to the layout
	grid.set_row_spacing(20);
	grid.set_column_spacing(20);
	grid.add(r_label);
	grid.attach_next_to(r_scale, r_label, Gtk::POS_RIGHT, 1, 1);
	grid.attach_next_to(g_label, r_label, Gtk::POS_BOTTOM, 1, 1);
	grid.attach_next_to(g_scale, g_label, Gtk::POS_RIGHT, 1, 1);
	grid.attach_next_to(b_label, g_label, Gtk::POS_BOTTOM, 1, 1);
	grid.attach_next_to(b_scale, b_label, Gtk::POS_RIGHT, 1, 1);
	grid.attach_next_to(hex_color_label, b_label, Gtk::POS_BOTTOM, 2, 1);
	grid.attach_next_to(quit_button, hex_color_label, Gtk::POS_BOTTOM, 2, 1);
	add(grid);
	set_border_width(20);


	/*== Ardino Code ==*/
	// We are going to skip error checking here
	// We also need to call sleep because of TTY delay from Arduino chip.
	arduino.startConnection();
	sleep(1);
	
	// Show the widgets
	show_all_children();
	sleep(1);
}


RGBWindow::~RGBWindow() {
	// Arduino deconstructor should get called automatically
}

void RGBWindow::on_button_quit() {
	hide();
}

void RGBWindow::on_scale_change() {
	// Build the packet
	unsigned char buffer[3];
	buffer[0] = (unsigned char)(r_scale.get_value());
	buffer[1] = (unsigned char)(g_scale.get_value());
	buffer[2] = (unsigned char)(b_scale.get_value());

	// Send it
	arduino.send(buffer, 3);

	// Change the hex color label
	char str[8] = "";
	sprintf(str, "#%02X%02X%02X", buffer[0], buffer[1], buffer[2]);
	hex_color_label.set_label(str);
}




// Main program routine
int main(int argc, char *argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(
		argc, argv, "net.sixteen_bpp.cscArduino.examples");
	
	// I'm a terrible person for hard-coding in these vaules, but it's easier
	RGBWindow rgb_w("/dev/ttyACM0", 9600);
	return app->run(rgb_w);
}

