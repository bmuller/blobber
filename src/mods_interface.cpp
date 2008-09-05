#include "blobber.h"

ModInterface::ModInterface(string n) : name(n) { 
  debug("Module \"" + n + "\" just created"); 
};

void ModInterface::register_poi_criteria(Camarea &area, CRANGE crange) {
  area.register_poi_criteria(name, crange);
};

void ModInterface::get_poi(Camarea &area, vector<COORD> &modpoi) {
  area.get_poi(name, modpoi);
};
