/******************************************************************************
* FOBS Video API test application 
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

#include "Transcoder.h"
#include <time.h>
#include <iostream>
#include <unistd.h>

using namespace omnividea::fobs;
using namespace std;


int main(int argc, char *argv[])
{

    if(argc != 3) {
        cout << "Syntax: " << argv[0] << " inputfile outputfile" << endl;
	return -1;
    }
    Transcoder d(argv[1], argv[2]);
    
    ReturnCode error = d.chooseFormat("avi");

    cout << "Loop begin...\n";
    clock_t time0 = clock();
    d.transcode();
    if(isError(error)) cout << "Error: " << getErrorMessage(error)<< endl;
    clock_t timef = clock();
    cout << "Loop end...Time spent= " << ((timef-time0)/(double)CLOCKS_PER_SEC) << endl;
    
    
    
    return 0;

}