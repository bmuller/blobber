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
 * ChangeLog:
 *  2005-02-01: Masato Alemayehu
 *                  Global changes. Now it compiles ;)
 *  2005-01-31: Masato Alemayehu
 *                  Original code submission.
 */

package com.omnividea.media.content.unknown;
import java.awt.*;
import javax.swing.*;
import javax.media.*;
import javax.media.renderer.*;
import javax.media.format.*;
import javax.media.protocol.*;
import java.awt.Component;
import javax.media.control.*;
import java.util.*;
import com.omnividea.FobsConfiguration;

public class Handler implements Processor {
    
    protected int currentState = Controller.Realized;
    protected int previousState;
    protected int targetState;

    protected TimeBase timebase = null;
    protected Time startTime = null;
    protected Component controllerCanvas = null;
    protected DataSource dataSource = null;
    protected ArrayList controllerListeners = null;
    protected GainControl gainControl = null;
    protected float desiredRate = 1.0f;
    protected Demultiplexer parser = null;
    protected Component visualComponent = null;
    protected VideoRenderer renderer = null;
    protected Codec decoder = null;
    protected Track[] tracks = null;
    protected Track videoTrack = null;
    
    public Handler(){
    	controllerListeners = new ArrayList();
    }
    
    public void syncStart(Time time) {
     	startTime = time;
	//videoTrack.setStartTime(time);
	Buffer in = new Buffer();
 	Buffer out = new Buffer();
	while(true){
		try{
		videoTrack.readFrame(in);
		Thread.sleep(30);
		decoder.process(in, out);
		renderer.process(out);
		}catch(InterruptedException ie){}
	}
    }

    public void setStopTime(Time time) {
    }

    public void setMediaTime(Time time) {
    }

    public Time mapToTimeBase(Time time) throws ClockStoppedException {
    	return time;
    }

    public void setSource(javax.media.protocol.DataSource dataSource) throws java.io.IOException, IncompatibleSourceException {
    	this.dataSource = dataSource;
	parser = new com.omnividea.media.parser.video.Parser();
	parser.setSource(dataSource);
    }

    public float setRate(float rate) {
    	return desiredRate = rate;
    }

    public void removeController(Controller controller) {
    }

    public void addController(Controller controller) throws IncompatibleTimeBaseException {
    }

    public void setTimeBase(TimeBase timeBase) throws IncompatibleTimeBaseException {
	    throw new IncompatibleTimeBaseException();
    }

    public Control getControl(String str) {
    	return null;
    }

    public void removeControllerListener(ControllerListener controllerListener) {
    	controllerListeners.remove(controllerListener);
    }

    public void addControllerListener(ControllerListener controllerListener) {
    	controllerListeners.add(controllerListener);
    }

    public javax.media.protocol.ContentDescriptor setContentDescriptor(javax.media.protocol.ContentDescriptor contentDescriptor) throws NotConfiguredError {
	    return null;
    }

    public String toString() {

        String retValue;
        
        retValue = "Media Player";
        return retValue;
    }

    public void stop() {
    }

    public void start() {
    	syncStart(Manager.getSystemTimeBase().getTime());
    }

    public void realize() {
    try{
	decoder = new com.omnividea.media.codec.video.NativeDecoder();
	decoder.setInputFormat(videoTrack.getFormat());
	decoder.open();
	renderer = new com.sun.media.renderer.video.LightWeightRenderer();
	renderer.open();
	Format outputFormat = decoder.setOutputFormat(renderer.getSupportedInputFormats()[0]);
	if(outputFormat==null)
		outputFormat=new RGBFormat(new Dimension(720, 576),
				Format.NOT_SPECIFIED,
				int[].class,
				Format.NOT_SPECIFIED,
				24,
				3, 2, 1,
				3, Format.NOT_SPECIFIED,
				Format.TRUE,
				Format.NOT_SPECIFIED);
	renderer.open();
	renderer.setInputFormat(outputFormat);
	visualComponent = renderer.getComponent();
	} catch(Exception e){}
    }

    public void prefetch() {
    	realize();
    }

    public java.awt.Component getVisualComponent() {
    	return visualComponent;
    }

    public javax.media.control.TrackControl[] getTrackControls() throws NotConfiguredError {
    	return null;
    }

    public javax.media.protocol.DataSource getDataOutput() throws NotRealizedError {
    	return null;
    }

    public Control[] getControls() {
    	return null;
    }

    public java.awt.Component getControlPanelComponent() {
    	return null;
    }

    public javax.media.protocol.ContentDescriptor getContentDescriptor() throws NotConfiguredError {
    	return null;
    }

    public void deallocate() {
    }

    public void configure() {
    try{
	tracks = parser.getTracks();
	for(int i=0;i<tracks.length; i++)
		if(tracks[i].getFormat() instanceof VideoFormat)
			videoTrack = tracks[i];
	} catch(Exception e){}
    }

    public void close() {
    }

    public Time getDuration() {
    	return null;
    }

    public GainControl getGainControl() {
    	return null;
    }

    public long getMediaNanoseconds() {
    	return 0;
    }

    public Time getMediaTime() {
    	return null;
    }

    public float getRate() {
    	return 0f;
    }

    public Time getStartLatency() {
    	return null;
    }

    public int getState() {
	    return currentState;
    }

    public Time getStopTime() {
    	return null;
    }

    public javax.media.protocol.ContentDescriptor[] getSupportedContentDescriptors() throws NotConfiguredError {
	    return new ContentDescriptor[0];
    }

    public Time getSyncTime() {
    	return null;
    }

    public int getTargetState() {
    	return targetState;
    }

    public TimeBase getTimeBase() {
	    return timebase;
    }
	

    public static void main(String[] args) throws Exception {
	String url = args[0];
        MediaLocator ml = new MediaLocator(url);
        com.omnividea.media.protocol.file.DataSource ds = new com.omnividea.media.protocol.file.DataSource(null);
	ds.setLocator(ml);
	System.out.println(ds.getUrlName());
	Demultiplexer parser = new com.omnividea.media.parser.video.Parser();
	parser.setSource(ds);
	Track[] tracks = parser.getTracks();
	Track video = null;
	for(int i=0;i<tracks.length; i++)
		if(tracks[i].getFormat() instanceof VideoFormat)
			video = tracks[i];
	Codec decoder = new com.omnividea.media.codec.video.NativeDecoder();
	decoder.setInputFormat(video.getFormat());
	decoder.open();
	VideoRenderer renderer = new com.sun.media.renderer.video.LightWeightRenderer();
	renderer.open();
	Format outputFormat = decoder.setOutputFormat(renderer.getSupportedInputFormats()[0]);
	if(outputFormat==null)
		outputFormat=new RGBFormat(new Dimension(720, 576),
				Format.NOT_SPECIFIED,
				int[].class,
				Format.NOT_SPECIFIED,
				24,
				3, 2, 1,
				3, Format.NOT_SPECIFIED,
				Format.TRUE,
				Format.NOT_SPECIFIED);
	renderer.open();
	renderer.setInputFormat(outputFormat);
	JFrame f = new JFrame();
	f.add(renderer.getComponent());
	f.pack();
	f.setVisible(true);
	
	Buffer in = new Buffer();
	Buffer out = new Buffer();
	while(true){
		video.readFrame(in);
		Thread.sleep(30);
		decoder.process(in, out);
		//renderer.process(out);
	}
    }
}
