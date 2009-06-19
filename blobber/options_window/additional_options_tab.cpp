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

#include "additional_options_tab.h"

namespace blobber {
  using namespace std;

  AdditionalOptionsTab::AdditionalOptionsTab() : lblSavedImagesDir("Saved images location: ") {
    description.set_text("These are additional configuration options.  Generally defaults "
			 "should suffice.");
    description.set_line_wrap(true);
    description.set_line_wrap_mode(Pango::WRAP_WORD);
    set_spacing(10);
    pack_start(description, Gtk::PACK_SHRINK, 20);

    config = Configuration::get_config();

    savedImagesDirBox.pack_start(lblSavedImagesDir, Gtk::PACK_SHRINK);
    savedImagesDir.set_width_chars(28);
    savedImagesDirBox.pack_end(savedImagesDir, Gtk::PACK_SHRINK);

    pack_start(savedImagesDirBox, Gtk::PACK_SHRINK);    

    string saved_images_directory;
    string default_filename = Glib::build_filename(Glib::get_user_data_dir(), "blobber");
    config->get_set("saved_images_directory", saved_images_directory, default_filename);
    savedImagesDir.set_text(saved_images_directory);
  };

  void AdditionalOptionsTab::save() {
    config->set("saved_images_directory", savedImagesDir.get_text());
  };
};
