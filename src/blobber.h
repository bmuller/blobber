#include "config.h"

#include <gtkmm.h>
#include <cairomm/cairomm.h>
#include <glib.h>

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

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

#include "exception.h"
#include "utils.h"
#include "types.h"
#include "frame.h"
#include "framegrabber.h"
#include "projection_window.h"
#include "camarea.h"
#include "camera_window.h"
#include "options_window.h"
#include "mods.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))
