#include "blobber.h"

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
#ifdef GLIBMM_EXCEPTIONS_ENABLED
try
{
  m_UIManager->add_ui_from_string(ui_info);
}
catch(const Glib::Error &ex)
{
  std::cerr << "building menus failed: " << ex.what();
};
#else
std::auto_ptr<Glib::Error> ex;
m_UIManager->add_ui_from_strying(ui_info, ex);
if(ex.get())
  std::cerr << "building menus failed: " << ex->what();
#endif //GLIBMM_EXCEPTIONS_ENABLED

  // get the menubar and toolbar widgets, and add them to a container widget:
  Gtk::Widget *pMenubar = m_UIManager->get_widget("/MenuBar");
  if(pMenubar)
  {
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

void CameraWindow::about() 
{
  cout << "weeeeeeee\n";
};

void CameraWindow::options()
{
  cout << "waaaaaaaaa\n";
};
