/******************************************************************************
* FOBS C++ wrapper code 
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by José San Pedro Wandelmer
*
*    This file is part of FOBS.
*
*    FOBS is free software; you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation; either version 2.1 
*    of the License, or (at your option) any later version.
*
*    FOBS is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with FOBS; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/
#include "Error.h"
#include <stdarg.h>
#include <stdio.h>
using namespace omnividea::fobs;

namespace omnividea
{
namespace fobs
{

char *errorMessageArray[]={
"Ok",
"Codec generic error",
"Format is not supported",
"No video stream found",
"Codec unsupported",
"Video end reached",
"Frame action error",
"Decoder generic error",
"Encoder generic error",
"Frame buffer is empty",
"Memory error",
"Not initialized",
"Generic error",
"File open error",
"Incorrect arguments",
"Already initialized",
"File write error",
"No media stream found",
"No frame"
};

bool isOk(ReturnCode error){return error == OkCode;}
bool isError(ReturnCode error){return error != OkCode;}
char *getErrorMessage(ReturnCode error)
{
    return errorMessageArray[error];
}

static LogLevel logLevel= NoLog;
void log(LogLevel level, char *pattern, ...)
{
    if(level & logLevel) {
       va_list ap;
       va_start (ap, pattern);
       vfprintf (stderr, pattern, ap);
       va_end (ap);
    }
}
void setLogLevel(LogLevel level)
{
    logLevel = level;
}


}
}