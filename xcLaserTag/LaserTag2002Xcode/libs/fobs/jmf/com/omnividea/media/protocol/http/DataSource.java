/******************************************************************************
* FOBS Java CrossPlatform JMF PlugIn
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by Jose San Pedro Wandelmer
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

/**
 * Changelong:
 *  2005-08-18:  Jose San Pedro Wandelmer
 *                          Initial version
 */

package com.omnividea.media.protocol.http;

import javax.media.*;
import javax.media.protocol.*;

public class DataSource extends com.omnividea.media.protocol.DataSource
        implements Positionable {
    private String urlString = "";

    public DataSource() {
        super();
    }

    public DataSource (MediaLocator ml) {
        super (ml);
        setLocator(ml);
    }

    public void setLocator (MediaLocator ml) {
        super.setLocator (ml);
        try {
            urlString = ml.toString(); //Old version
			System.out.println("HTTP source: "+urlString);
        }
        catch (Exception e) {
            System.out.println ("Bad URL: " + ml);
        }
    }
    public String getUrlName()
    {
      return urlString;
    }
}
