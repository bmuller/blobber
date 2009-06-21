#ifndef __UTILS_H
#define __UTILS_H

#include <string>
#include <vector>

#include <glibmm/random.h>

namespace blobber {
  using namespace std;

  void debug(string msg);
  void error(string msg);
  void alert(string msg);
  void num_to_string(int i, string& s);
  void num_to_string(double i, string& s);
  int string_to_int(string s);
  double string_to_double(string s);
  void int_to_string(int i, string& s);
  // join a vector of strings with a bit of joiner text
  void join(vector<string> v, string joiner, string& result);
  // break up a string into parts whereever e occurs
  vector<string> explode(string s, string e);
  // return true if s ends with end, false otherwise
  bool ends_with(string s, string end);
  int restrict_range(int x, int lower, int upper);

  /** 
   * Pick a random element in this vector and assign it to pick.
   * 
   * @param values The vector to look at
   * @param pick The variable to assign a random element to
   */
  template <class T> void random_choice(vector<T> &values, T &pick) {
    Glib::Rand rand;
    int random_index = rand.get_int_range(0, values.size());
    pick = values.at(random_index);
  };
  
  /** 
   * Determine if the given value is in this vector or not.
   * 
   * @param values The vector to look at
   * @param value The value to search for
   * 
   * @return True if value is in the vector, false otherwise
   */
  template <class T> bool includes(vector<T> &values, T value) {
    for(unsigned int i=0; i < values.size(); i++)
      if(values.at(i) == value)
	return true;
    return false;
  };
  
  /** 
   * Remove all of the instances of a value from the vector
   * 
   * @param values The vector to look at
   * @param value The value to remove
   */
  template <class T> void remove(vector<T> &values, T value) {
    // terribly inefficient ... fix this later
    for(unsigned int i=0; i < values.size(); i++) {
      if(values.at(i) == value) {
	values.erase(values.begin() + i);
	remove(value);
      }
    }
  };
  
  /** 
   * Count the number of times value occurs in this vector
   * 
   * @param values The vector to look at
   * @param value The value to look for
   * 
   * @return A total count of the number of times the value occurs
   */
  template <class T> int count(vector<T> &values, T value) {
    int count = 0;
    for(unsigned int i=0; i < values->size(); i++)
	if(values.at(i) == value)
	  count++;
    return count;
  };
  
  /** 
   * Perform union operation with another vector
   * 
   * @param these Add elements from others to this one if not already present
   * @param others The other vector to join to this one
   */
  template <class T> void unionize(vector<T> &these, vector<T> &others) {
    for(unsigned int i=0; i < others.size(); i++)
      if(!includes(these, others[i]))
	these.push_back(others[i]);
  };
  
  /** 
   * Return a new vector that contains the intersection
   * of this one and another
   * 
   * @param these One vector to intersect
   * @param others The other vector to intersect
   * 
   * @return The intersection of the two vectors
   */
  template <class T> vector<T> intersects(vector<T> &these, vector<T> &others) {
    vector<T> joined;
    for(unsigned int i=0; i < others.size(); i++)
      if(includes(these, others[i]))
	joined.push_back(others[i]);
    return joined;
  };
  
};

#endif /* __UTILS_H */
