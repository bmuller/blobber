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

#include "blobber.h"

namespace blobber {
  using namespace std;

  CameraWindow::CameraWindow(string device) : exitButton("Exit"), area(device), table(2, 15) { 
    set_default_size(area.width, area.height+50);
    add(m_Box);
    exitButton.signal_clicked().connect(sigc::mem_fun(*this, &CameraWindow::exit) );

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
    table.attach(exitButton, 0, 1, 14, 15, Gtk::EXPAND, Gtk::EXPAND); 
    show_all_children();
  };

  void CameraWindow::exit() {
    hide();
  };

  void CameraWindow::about() {
    Glib::ustring strVersion = "Blobber version " + string(VERSION);
    Gtk::MessageDialog cmdAbout(*this, strVersion);
    cmdAbout.set_secondary_text("Blobber tracks lights/colors with an attached webcam and "
				"then projects their \"reactions\" using an attached projector.\n\n"
				"Blobber is open source software brougt to you by csclug.org.");
    cmdAbout.run();
  };

  void CameraWindow::options() {
    set_transient_for(options_window);
    options_window.show();
  };

};
