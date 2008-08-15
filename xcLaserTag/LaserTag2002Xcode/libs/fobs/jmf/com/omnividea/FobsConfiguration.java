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
 *  2005-02-21:  Jose San Pedro Wandelmer
 *                          Initial version
 */

package com.omnividea;

public class FobsConfiguration 
{
    public final static int RGBA = 0;
    public final static int YUV420 = 1;
    
    public static int videoFrameFormat = RGBA;
    public static boolean useNativeBuffers = true;
    public static java.util.Hashtable properties = new java.util.Hashtable();
    
    static
    {
        reset();
    }
    
    public static void reset()
    {
        videoFrameFormat = RGBA;
        useNativeBuffers = true;
        properties.clear();  
    }
	
	public static void log(String s) {
		//System.out.println(s+"\n");
	}
}
