/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "camera_window.h"
#include "exception.h"

#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>

namespace blobber {
  using namespace std;

  CameraWindow::CameraWindow() : area(), table(2, 15), options_window(&area), description() { 
    set_default_size(area.dimensions.width, area.dimensions.height+50);
    add(m_Box);

    //Create actions for menus and toolbars:
    m_ActionGroup = Gtk::ActionGroup::create();

    //File menu:
    m_ActionGroup->add(Gtk::Action::create("FileMenu", "File"));
    m_ActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT), 
		       sigc::mem_fun(*this, &CameraWindow::exit));
    m_ActionGroup->add(Gtk::Action::create("FileOptions", Gtk::Stock::PREFERENCES),
		       sigc::mem_fun(*this, &CameraWindow::options));

    //Help menu:
    m_ActionGroup->add( Gtk::Action::create("HelpMenu", "Help"));
    m_ActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
		       sigc::mem_fun(*this, &CameraWindow::about));
    m_ActionGroup->add(Gtk::Action::create("HelpContents", Gtk::Stock::HELP),
		       sigc::mem_fun(*this, &CameraWindow::help));

    m_UIManager = Gtk::UIManager::create();
    m_UIManager->insert_action_group(m_ActionGroup);

    //Layout the actions in a menubar and toolbar:
    Glib::ustring ui_info = 
      "<ui>"
      "  <menubar name='MenuBar'>"
      "    <menu action='FileMenu'>"
      "      <separator/>"
      "      <menuitem action='FileOptions'/>"
      "      <menuitem action='FileQuit'/>"
      "    </menu>"
      "    <menu action='HelpMenu'>"
      "      <menuitem action='HelpContents'/>"
      "      <menuitem action='HelpAbout'/>"
      "    </menu>"
      "  </menubar>"
      "</ui>";

    try {
      m_UIManager->add_ui_from_string(ui_info);
    } catch(const Glib::Error &ex) {
      throw GUIException("building menus failed: " + string(ex.what()));
    };

    // get the menubar and toolbar widgets, and add them to a container widget:
    Gtk::Widget *pMenubar = m_UIManager->get_widget("/MenuBar");
    if(pMenubar) {
      m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);
      //	table.attach(*pMenubar, 0,0,0,0);
    };
    m_Box.pack_start(table);
    table.attach(area, 0, 2, 0, 14);
    description.set_label("Insert some meaningful text here.");
    table.attach(description, 0, 1, 14, 15, Gtk::EXPAND, Gtk::EXPAND); 
    show_all_children();
  };

  void CameraWindow::exit() {
    hide();
  };

  void CameraWindow::about() {
    Glib::ustring strVersion = string(PACKAGE_STRING);
    Gtk::MessageDialog cmdAbout(*this, strVersion);
    cmdAbout.set_secondary_text("Blobber tracks lights/colors with an attached webcam and "
				"then projects their \"reactions\" using an attached projector.\n\n"
				"Blobber is open source software brougt to you by csclug.org.\n"
				"For more information see " + string(PACKAGE_URL));
    cmdAbout.set_title("About " + string(PACKAGE_NAME));
    cmdAbout.run();
  };

  void CameraWindow::help() {
    Glib::ustring strVersion = string(PACKAGE_STRING) + " Help";
    Gtk::MessageDialog cmdHelp(*this, strVersion);
    cmdHelp.set_title(string(PACKAGE_NAME) + " Help");
    cmdHelp.set_secondary_text("You can find help for blobber at:\n" + string(PACKAGE_URL));
    cmdHelp.run();
  };

  void CameraWindow::options() {
    // LIES! LIES! set_transient_for() does not follow gtkmm.org docs
    //set_transient_for(options_window);
    options_window.show();
  };

  bool CameraWindow::on_key_press_event(GdkEventKey* eventData) {
    string ks;
    num_to_string((int) eventData->keyval, ks);
    debug("Key pressed on camera window: "+ks);

    switch(eventData->keyval) {
    case 115: // s
	save_to_file();
	break;
    };
  };

  void CameraWindow::save_to_file() {
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    string filepath;
    time_t rawTime;
    string nameTime;

    // make sure directory exists
    filepath =  Glib::build_filename(Glib::get_user_data_dir(), "blobber");
    try {
      Glib::Dir::Dir dirio(filepath);
    } catch(Glib::FileError fe) {
      if(mkdir(filepath.c_str(), S_IRWXU) != 0)
	throw ConfigurationException("Could not make configuration directory " + filepath);
    };

    rawTime = time(NULL);
    nameTime = string(ctime(&rawTime));
    filepath = Glib::build_filename(filepath, nameTime.substr(0, nameTime.length()-1) + "_camera.png");
    debug("Saving camera screen capture to " + filepath);
    try {
      area.getSurface()->write_to_png(filepath);
      debug("Image saved to " + filepath);
    } catch(exception &e) {
      error(e.what());
    }
#else
    throw NoSuchFeatureException("Cairo must be compiled with PNG support to save screen captures")
#endif	
  };
};
