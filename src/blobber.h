#define DEBUG 1

// Percentage of a ushort
#define USHORT_PERCENT(n)    ((n)*65535/100)
#define US50PC                 USHORT_PERCENT(50)

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

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/videodev.h>

#include <sys/mman.h>
#include <sys/ioctl.h>

#include "exception.h"
#include "types.h"
#include "utils.h"
#include "frame.h"
#include "framegrabber.h"
#include "projection_window.h"
#include "camarea.h"
#include "options_window.h"
#include "mods.h"
