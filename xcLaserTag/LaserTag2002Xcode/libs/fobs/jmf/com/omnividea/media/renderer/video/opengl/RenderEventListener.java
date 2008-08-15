/******************************************************************************
* FOBS Java CrossPlatform JMF PlugIn
* Copyright (c) 2004 Omnividea Multimedia S.L
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

/*
 * Created on 06.02.2005 by Robert Binna
 * ChangeLog:
 *  Jose San Pedro Wandelmer 2005/02/21 
 *      - Package changed to fit Fobs package naming
 *      - Automatic Generated comments removed
 *      - Header included with license information
 */

package com.omnividea.media.renderer.video.opengl;


import java.awt.event.MouseMotionListener;

import net.java.games.jogl.GLEventListener;

public interface RenderEventListener extends GLEventListener,MouseMotionListener {

    public void setRenderingData(int[] data);
    
    public void setFormat(int videoWidth, int videoHeight);
}
