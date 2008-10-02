
namespace blobber {

  using namespace std;

  class CameraWindow : public Gtk::Window {
  public:
    Camarea area;
    OptionsWindow options_window;
    CameraWindow();
    void exit();
    void about();
    void options();
    bool on_key_press_event(GdkEventKey* eventData);
  protected:
    Gtk::Button exitButton;
    Gtk::Table table;
    Gtk::VBox m_Box;
    Glib::RefPtr<Gtk::UIManager> m_UIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_ActionGroup;
  private:
    void save_to_file();
  };

};
