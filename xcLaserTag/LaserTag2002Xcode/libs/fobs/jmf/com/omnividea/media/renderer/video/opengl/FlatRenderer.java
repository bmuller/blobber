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
 *  Jose San Pedro Wandelmer 2005/02/21 - Major changes to code structure
 *      - Package changed to fit Fobs package naming
 *      - Automatic Generated comments removed
 *      - Header included with license information
 *      - Included code to do CSC in a Pixel Shader
 *      - Thanks to Robert Binna for his contribution to the project!!
 */

package com.omnividea.media.renderer.video.opengl;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;

import net.java.games.jogl.GL;
import net.java.games.jogl.GLDrawable;
import net.java.games.jogl.GLU;
import net.java.games.cg.CgGL;
import net.java.games.cg.CGcontext;
import net.java.games.cg.CGprogram;
import net.java.games.cg.CGparameter;


import com.omnividea.FobsConfiguration;

public class FlatRenderer implements RenderEventListener {
    
    int width = 1024;
    int height = 1024;
    int[] textures = new int[1];
    private int[] firstpixels;
    private int formatWidth;
    private int formatHeight;
    private int[] data;
    private float extendX;
    private float extendY;
    private int inputColorOrder;
    private int inputFormat;
    private CGcontext cgContext = null;
    private CGprogram cgProgram = null;
    private CGparameter cgParameter = null;
    private String program = 
        
        "float4 main(half2 coords : TEX0,                                   \n"+
        "            uniform sampler2D Y) : COLOR                          \n"+
        "{                                                                  \n"+
        "    float4 c  = tex2D(Y, coords);                             \n"+
        "    float3 res1 = (c.rrr-(half3(0.062745,0.062745,0.062745)))*1.164;\n"+
        "    res1 += (c.bbb-half3(0.5,0.5,0.5))*half3(1.596,-0.813,0);\n"+
        "    res1 += (c.ggg-half3(0.5,0.5,0.5))*half3(0,-0.391,2.018);\n"+
        "    return clamp(float4(res1,1.0), 0.062745, 0.92); \n"+
        "}                                                                  \n";
 

    public FlatRenderer() {
        inputColorOrder = com.omnividea.media.parser.video.Parser.isBigEndian()?GL.GL_RGBA:GL.GL_BGRA;
        inputFormat = com.omnividea.media.parser.video.Parser.isBigEndian()?GL.GL_UNSIGNED_INT_8_8_8_8:GL.GL_UNSIGNED_INT_8_8_8_8_REV;
    }
    
    public boolean hasFragmentShaderSupport()
    {
        if(CgGL.cgGLIsProfileSupported(CgGL.CG_PROFILE_FP20)||CgGL.cgGLIsProfileSupported(CgGL.CG_PROFILE_ARBFP1))
        {
            return true;
        }
        else return false;
    }
    
    public int getBestProfile()
    {
        if(CgGL.cgGLIsProfileSupported(CgGL.CG_PROFILE_ARBFP1))
        {
            return CgGL.CG_PROFILE_ARBFP1;
        }    
        else
        {
            return CgGL.CG_PROFILE_FP20;
        }
    }
    
    public void init(GLDrawable drawable) {
        System.out.println("init 3d");
        
        
        
        
        GL gl = drawable.getGL();
        GLU glu = drawable.getGLU();
        gl.glGenTextures(1, textures);
        gl.glBindTexture(GL.GL_TEXTURE_2D, textures[0]);
        gl.glDisable(GL.GL_DEPTH_TEST);
        gl.glDisable(GL.GL_LIGHTING);
        gl.glEnable(GL.GL_TEXTURE_2D);
        //gl.glTexEnvf(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_REPLACE);

        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, GL.GL_REPEAT);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, GL.GL_REPEAT);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);

        gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, width, height, 0,  GL.GL_RGBA, GL.GL_UNSIGNED_INT_8_8_8_8, (char[])null);

        //System.out.println("time :: " + (System.currentTimeMillis() - time));  
        gl.glClear(GL.GL_COLOR_BUFFER_BIT);
        gl.glMatrixMode(GL.GL_PROJECTION);    
        gl.glLoadIdentity();               
        glu.gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
        
        cgContext = CgGL.cgCreateContext();
        int errorCg = CgGL.cgGetError();
        System.out.println("cgCreateContext: "+CgGL.cgGetErrorString(errorCg));

        if(hasFragmentShaderSupport())
        {
            
            cgProgram = CgGL.cgCreateProgram(cgContext,  CgGL.CG_SOURCE, program,  getBestProfile(),  "main",  null);
            //errorCg = CgGL.cgGetError();
            //System.out.println("cgCreateProgram: "+CgGL.cgGetErrorString(errorCg));
            
            CgGL.cgGLLoadProgram(cgProgram);
            //errorCg = CgGL.cgGetError();
            //System.out.println("cgLoadProgram: "+CgGL.cgGetErrorString(errorCg));
            CgGL.cgGLBindProgram(cgProgram);
            //errorCg = CgGL.cgGetError();
            //System.out.println("cgBindPRogram: "+CgGL.cgGetErrorString(errorCg));
            
            cgParameter = CgGL.cgGetNamedParameter(cgProgram,  "Y");
            CgGL.cgGLSetTextureParameter(cgParameter,  textures[0]);
            //errorCg = CgGL.cgGetError();
            //System.out.println("cgSetParam: "+CgGL.cgGetErrorString(errorCg));
            
            CgGL.cgGLEnableTextureParameter(cgParameter);

            gl.glBindTexture(GL.GL_TEXTURE_2D, textures[0]);
            CgGL.cgGLEnableProfile(getBestProfile());
            //errorCg = CgGL.cgGetError();
            //System.out.println("cgEnableProfile: "+CgGL.cgGetErrorString(errorCg));
        }
        else
        {
            FobsConfiguration.videoFrameFormat=FobsConfiguration.RGBA;
            inputColorOrder = GL.GL_BGRA;
            inputFormat = GL.GL_UNSIGNED_INT_8_8_8_8_REV;
            System.out.println("Your graphics hardware is not capable of doing Color Space Conversion\n");
        }
    }

    public void display(GLDrawable drawable) {
        GL gl = drawable.getGL();
        GLU glu = drawable.getGLU();
        
        if(data != null) {
            gl.glTexSubImage2D(GL.GL_TEXTURE_2D, 0, 0, 0, formatWidth, formatHeight,  inputColorOrder, inputFormat, data);
        }
                
        gl.glBegin(GL.GL_QUADS);
	        gl.glTexCoord2f(0,0);
	        gl.glVertex3d(-1, 1,0);
	        gl.glTexCoord2f(extendX,0);
	        gl.glVertex3d(1, 1,0);
	        gl.glTexCoord2f(extendX,extendY);
	        gl.glVertex3d(1, -1,0);
	        gl.glTexCoord2f(0,extendY);
	        gl.glVertex3d(-1, -1,0);
        gl.glEnd();
        
        drawable.swapBuffers();
          
        
 
        
    }

    public void reshape(GLDrawable drawable, int x, int y, int width, int height) {
    }

    public void displayChanged(GLDrawable arg0, boolean arg1, boolean arg2) {
    }
    
    public void mouseClicked(MouseEvent e) {}
    
    public void mouseDragged(MouseEvent e) {

    }
    
    public void mouseMoved(MouseEvent e) {}

    public void setRenderingData(int[] data) {
        this.data = data;
        
    }



    public void setFormat(int videoWidth, int videoHeight) {
        formatWidth = videoWidth;
        formatHeight = videoHeight;      
        extendX = ((float) formatWidth)/width;
        extendY = ((float) formatHeight)/height;

    }
  
    
}
