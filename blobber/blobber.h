#include "config.h"
#define DEFAULT_DEVICE "/dev/video0"
// size of the default POI criteria window
#define DEFAULT_CRITERIA_WINDOW_SIZE "15"

#include <glibmm/keyfile.h>

#include <gtkmm/scale.h>
#include <gtkmm/frame.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxentrytext.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/table.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/main.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cerrno>

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/types.h>

#ifdef HAVE_V4L1
#include <linux/videodev.h>
#endif

#ifdef HAVE_V4L2
#include <linux/videodev2.h>
#endif

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/dir.h>


#include "exception.h"
#include "utils.h"
#include "types.h"
#include "configuration.h"
#include "frame.h"
#include "colormap.h" 
#include "framegrabber.h"
#include "projection_window.h"
#include "camarea.h"
#include "options_window.h"
#include "camera_window.h"
#include "mod_interface.h"
#include "application.h"

#include "mod_utils/movable_object.h"
#include "mod_utils/movable_circle.h"
#include "mod_utils/movable_square.h"
#include "mod_utils/movable_crosshairs.h"
#include "mod_utils/sprite.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))
