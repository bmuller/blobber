namespace blobber {
  using namespace std;

  void debug(string msg);
  void num_to_string(int i, std::string& s);
  void num_to_string(double i, std::string& s);
  int string_to_int(string s);
  void int_to_string(int i, std::string& s);
  template <class T> void random_choice(vector<T> things, T &pick);
  // join a vector of strings with a bit of joiner text
  void join(vector<std::string> v, string joiner, string& result);
  // break up a string into parts whereever e occurs
  vector<string> explode(string s, string e);
};

