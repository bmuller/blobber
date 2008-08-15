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
 *  2004-12-17: Jose San Pedro Wandelmer (fobs@mac.com)
 *                          Simpler solution for the *indows filename bug.
 *                          Just replace file://drive/path with file:drive/path
 *                          JMF accepts windows paths built using slashes :)
 * 	2004-09-15:	Christian Berger (c.berger@tu-braunschweig.de):
 * 							Handle multiple leading "/" in a URL. 
 * 
 * 	2004-09-13:	Christian Berger (c.berger@tu-braunschweig.de):
 * 							Fixing a bug in the *indows distribution preventing fobs4jmf
 * 							playing a video file in a simple player environment.
 *  2004-06-6:  Jose San Pedro Wandelmer
 *                          Initial version
 */

package com.omnividea.media.protocol.file;

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
    }

    public void setLocator (MediaLocator ml) {
      super.setLocator (ml);
    }
    
    public String getUrlName()
    {
      MediaLocator ml = this.getLocator();
      if(ml == null) throw new NullPointerException("No Medialocator found"); 
      String urlString = ""; 
      try { 
        if (ml != null) { 
          if (System.getProperty("file.separator").equals("\\") && ml.getProtocol().equals("file")) 
          { 
            String modifiedSource = ml.getURL().toExternalForm(); 
            urlString = modifiedSource.replaceFirst("file:/","file:"); 
            while(modifiedSource.equals(urlString) == false) 
            { 
              modifiedSource = urlString; 
              urlString = modifiedSource.replaceFirst("file:/","file:"); 
            } 
          } 
          else 
          { 
            try{ 
              urlString = ml.getURL().toString(); 
            }catch(Exception e){ 
              e.printStackTrace(); 
            } 
          } 
        } 
      } 
      catch (Exception e) { 
        System.out.println ("Bad URL: " + urlString); 
      } 
      return urlString;
    }
}
