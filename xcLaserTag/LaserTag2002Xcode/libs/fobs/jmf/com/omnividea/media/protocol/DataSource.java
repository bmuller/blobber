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
  *  2005-08-19:  Jose San Pedro Wandelmer
 *                          Initial version
 */

package com.omnividea.media.protocol;

import javax.media.*;
import javax.media.protocol.*;

abstract public class DataSource extends javax.media.protocol.DataSource
        implements Positionable {
	public static final Object[] controls = {};
    public DataSource() {
        super();
    }
    public DataSource (MediaLocator ml) {
        super (ml);
        setLocator(ml);
    }

    public void setLocator (MediaLocator ml)
	{
		super.setLocator(ml);
	}
    abstract public String getUrlName();
    public String getContentType() {
        return "video.ffmpeg";
    }

    public void connect() throws java.io.IOException {}
    public void disconnect()   {}
    public void start()  throws java.io.IOException {}
    public void stop() throws java.io.IOException {}

    public Time getDuration() {
        return new Time(0);
    }
    public Object[] getControls() {
        return controls;
    }
    public Object getControl (String controlType) {
        return null;
    }
    public boolean isRandomAccess() {
      boolean randomAccess = false;
      return randomAccess;
    }
    public Time setPosition (Time where, int rounding) {
        Time newPosition = where;
        return newPosition;
    }
}
