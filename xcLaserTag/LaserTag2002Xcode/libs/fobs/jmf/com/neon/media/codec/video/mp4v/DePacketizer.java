/* $Id: DePacketizer.java,v 1.1 2005/12/28 15:58:18 jsanpedro Exp $ */
/**
 * Neon Media MPEG-4 Video
 * RTP De-Packetizer
 * @author Scott Hays
 * @created 2005/08/26
 * Copyright (c) 2005 Neon Advanced Technologies Co., Ltd.  All rights reserved.
 *
 * The Neon MPEG-4 Video RTP DePacketizer is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; either version 2.1 of the License, or 
 * (at your option) any later version.
 *
 * The Neon MPEG-4 Video RTP DePacketizer is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 */
package com.neon.media.codec.video.mp4v;

import com.neon.util.RTP;

import com.ibm.media.codec.video.VideoCodec;

import java.awt.Dimension;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.io.IOException;
import java.util.Vector;

import javax.media.Buffer;
import javax.media.Format;
import javax.media.PlugIn;
import javax.media.format.VideoFormat;

public class DePacketizer extends VideoCodec {
	private static String IN_ENCODING = "MP4V/RTP";
	private static String OUT_ENCODING = "MP4V";
	private static final int DEF_WIDTH = 352;
	private static final int DEF_HEIGHT = 240;
	private boolean firstFrame = true;
	private MP4VFrame currentFrame = null;
	private short seqNum = 0;
		
	private final int FRAME_BUFFER_INITIAL_SIZE = 8*1024;	// 8KB

/*	//DEBUG
	private File dumpFile;
	private File packDumpFile;
	private File completeDFile;
	private File logFile;
	private FileOutputStream dumpOut;
	private FileOutputStream packDumpOut;
	private FileOutputStream completeDOut;
	private FileOutputStream logOut;
	private PrintStream log;
*/	//DEBUG
	
	private static boolean isFrameStart(Buffer buffer) {
		Object data = buffer.getData();
		byte[] payload = (byte[])data;
		int offset = buffer.getOffset();
		return isFrameStart(payload, offset);
	}
	
	// starts with VS, VO, VOL, or VOP start code
	private static boolean isFrameStart(byte[] payload, int offset) {
		int firstBytes = (payload[offset] & 0xff) << 16 | (payload[offset+1] & 0xff) << 8 | (payload[offset+2] & 0xff);
		// first 3 bytes of a system code
		return firstBytes == 0x000001;
	}
	
	// should at least have VOL header (but should really have VS and VO as well)
	private static boolean canStartStream(Buffer buffer) {
		byte[] payload = (byte[])buffer.getData();
		int offset = buffer.getOffset();
		//System.err.println("mp4v.DePacketizer: offset = " + offset);
		int length = 32;	// VOL SHOULD be in the first 32 bytes so don't bother scanning entire buffer
		if (buffer.getLength() <= length) return false;
		boolean keep_looking = true;
		int currentCode = (payload[offset] & 0xff) << 24 | (payload[offset+1] & 0xff) << 16 | (payload[offset+2] & 0xff) << 8 | (payload[offset+3] & 0xff);

		for (int i=4; i < length; i++) {
			if ((currentCode & 0xfffffff0) == 0x00000120) return true;
			if (currentCode == 0x000001b0) System.err.println("MP4VFrame.canStartStream: frame has VS header");
			if (currentCode == 0x000001b5) System.err.println("MP4VFrame.canStartStream: frame has VO header");
			//System.err.println("MP4VFrame.canStartStream:0x" + Integer.toHexString(currentCode));
			currentCode = (currentCode << 8) | (payload[offset+i] & 0xff);
		}
		return false; // VOL not found
	}
	
	public DePacketizer() {
		System.err.println(this.getClass().getName() + " constructor called.");
		PLUGIN_NAME = "Neon MP4V RTP DePacketizer";
		inputFormats = supportedInputFormats = new VideoFormat[] { new VideoFormat(IN_ENCODING), new VideoFormat("MP4V-ES"), new VideoFormat("MP4V-ES/90000") };
		defaultOutputFormats = new VideoFormat[] { new VideoFormat(OUT_ENCODING) };
		
/*		//DEBUG
		try {
			dumpFile = new File("dump.m4v");
			packDumpFile = new File("packdump.m4v");
			completeDFile = new File("completedump.m4v");
			logFile = new File("nmp.log");
			dumpOut = new FileOutputStream(dumpFile);
			packDumpOut = new FileOutputStream(packDumpFile);
			completeDOut = new FileOutputStream(completeDFile);
			logOut = new FileOutputStream(logFile);
			log = new PrintStream(logOut);
		} catch (FileNotFoundException e) {
			System.err.println("mp4v.DePacketizer: failed to open dump files: " + e.getMessage());
			e.printStackTrace();
		}
*/		//DEBUG
	}
	
	public Format[] getSupportedOutputFormats(Format f) {
		System.err.println("mp4v.DePacketizer: getting for f = " + f);
		if (f == null) return defaultOutputFormats;
		else if (f.getEncoding().equalsIgnoreCase(IN_ENCODING)) {
			if (f instanceof VideoFormat) {
				VideoFormat vf = (VideoFormat)f;
				VideoFormat supOutFmt = new VideoFormat(OUT_ENCODING, vf.getSize() == null ? new Dimension(DEF_WIDTH, DEF_HEIGHT):vf.getSize(), vf.getMaxDataLength(), Format.byteArray, vf.getFrameRate());
				System.err.println("mp4v.DePacketizer: supported Output Format = " + supOutFmt);
				return new Format[] { supOutFmt };
			} else return new Format[] {new VideoFormat(OUT_ENCODING, new Dimension(DEF_WIDTH, DEF_HEIGHT), Format.NOT_SPECIFIED, Format.byteArray, Format.NOT_SPECIFIED) };
		} 
		System.err.println("mp4v.DePacketizer: getSupportedOutputFormats f = " + f);
		return new Format[0];
	}
	
	public String getName() {
		return PLUGIN_NAME;
	}

	public Format setOutputFormat(Format f) {
		VideoFormat vf = (VideoFormat)f;
		if (!f.getEncoding().equalsIgnoreCase(OUT_ENCODING)) {
			System.err.println("mp4v.DePacketizer: setOutputFormat received unsupported encoding: " + f.getEncoding());
			return null;
		}
		System.err.println("mp4v.DePacketizer: setOutputFormat received format with size: " + vf.getSize());
		outputFormat = new VideoFormat(OUT_ENCODING, vf.getSize() == null ? new Dimension(DEF_WIDTH, DEF_HEIGHT):vf.getSize(), vf.getMaxDataLength(), Format.byteArray, vf.getFrameRate()); 
		return outputFormat;
	}

	public synchronized int process(Buffer inBuffer, Buffer outBuffer) {
		if (firstFrame && !(inBuffer.getData() instanceof byte[])) { // assume that if the first inBuffers are byte arrays, all are
			System.err.println("MP4VFrame.isFrameStart: payload is not byte[]");
			return OUTPUT_BUFFER_NOT_FILLED;
		}
		//DEBUG
		//HACK:Sun RTP RECEIVE BUG
		//	Sun RTP seems to require regular I/O, otherwise it seldom delivers received packets
		System.err.println("mp4v.DePacketizer: process() inBuffer.getLength() = " + inBuffer.getLength());
		//DEBUG
		if (isEOM(inBuffer)) {
			propagateEOM(outBuffer);
			return BUFFER_PROCESSED_OK;
		}

		if (inBuffer.isDiscard()) {
			updateOutput(outBuffer, outputFormat, 0, 0);
			outBuffer.setDiscard(true);
			return OUTPUT_BUFFER_NOT_FILLED;
		}
		
/*		//DEBUG
		try {
			completeDOut.write((byte[])inBuffer.getData(), inBuffer.getOffset(), inBuffer.getLength());
		} catch (IOException e) {
			System.err.println("mp4v.DePacketizer: couldn't write to complete dump: " + e.getMessage());
			e.printStackTrace();
		}
*/		//DEBUG
		
		//System.err.println("mp4v.DePacketizer: process packet");
		// lost RTP fragment packet(s) so drop frame
		if (currentFrame != null && inBuffer.getTimeStamp() != currentFrame.getTimeStamp()) {
			System.err.println("mp4v.DePacketizer: DROP FRAME:timestamp mismatch: got(" + inBuffer.getTimeStamp() + ") on packet " + inBuffer.getSequenceNumber() + ", expected(" + currentFrame.getTimeStamp() + ")");
			currentFrame = null;
//			frameBuffer.clear();
		}
		
		// check to see if packet is first of a frame
		if (currentFrame == null && isFrameStart(inBuffer)) {
			// ensure first frame given to encoder has needed configuration information
			if (firstFrame && !canStartStream(inBuffer)) return PlugIn.OUTPUT_BUFFER_NOT_FILLED;
			if (firstFrame) System.err.println(">>>> mp4v.DePacketizer: START STREAM");
			currentFrame = new MP4VFrame(inBuffer);
			firstFrame = false;
		
		} else {
			if (currentFrame == null) {
				System.err.println("^^^^mp4v.DePacketizer: DROP PACKET " + inBuffer.getSequenceNumber() + ": no current frame and packet cannot start frame.");
				return PlugIn.OUTPUT_BUFFER_NOT_FILLED;
			}
//			System.err.println("mp4v.DePacketizer: adding packet to frame");
			currentFrame.add(inBuffer);	
		}
		
		// check to see if entire frame should now have been received
		if ((inBuffer.getFlags() & Buffer.FLAG_RTP_MARKER) != 0) {
			if (!currentFrame.isMissingFragments()) {
				completeTransfer(inBuffer, outBuffer);
				currentFrame = null;
//				System.err.println("____mp4v.DePacketizer: delivering frame._____");
				return PlugIn.BUFFER_PROCESSED_OK;
			} else {
				// frame incomplete and last packet received, so drop
				System.err.println("&&&&mp4v.DePacketizer: DROP FRAME:missing fragments");
				currentFrame = null;
				return PlugIn.OUTPUT_BUFFER_NOT_FILLED;
			}
		}

//		System.err.println("mp4v.DePacketizer: output buffer not filled");

		return PlugIn.OUTPUT_BUFFER_NOT_FILLED;
	}

	private void completeTransfer(Buffer inBuffer, Buffer outBuffer) {
//		System.err.println("mp4v.DePacketizer: completeTransfer called");
		if (outputFormat == null) System.err.println("mp4v.DePacketizer: outputFormat NULL!!!!!!!!!!");
		//SANITY CHECK
		if (!isFrameStart(currentFrame.getFrameBuffer(), 0)) {
			System.err.println("((((((mp4v.DePacketizer: FRAME CORRUPTED!!))))))");
			System.exit(1);
		}

		outBuffer.setFormat(outputFormat);
		
		outBuffer.setData(currentFrame.getFrameBuffer());
		outBuffer.setOffset(0);
		outBuffer.setDuration((long)(((VideoFormat)outputFormat).getFrameRate() + 0.5));
		outBuffer.setSequenceNumber(seqNum++);	//FIXME roll over??
		outBuffer.setTimeStamp(currentFrame.getTimeStamp());
		outBuffer.setLength(currentFrame.getDataLength());
		
/*		//DEBUG
		try {
			dumpOut.write(currentFrame.getFrameBuffer(), 0, currentFrame.getDataLength());
		} catch (Exception e) {
			System.err.println("mp4v.DePacketizer: failed to write frame: " + e.getMessage());
			e.printStackTrace();
		}
*/		//DEBUG
		currentFrame = null;
	}
	
	private class MP4VFrame {
		private long firstSequenceNumber = 0, lastSequenceNumber = 0;
		private int numPackets = 0;
		private Buffer frameBuffer = null;
		private Vector outOfOrderFragments;
		
		public MP4VFrame(Buffer inBuffer) {
			firstSequenceNumber = inBuffer.getSequenceNumber();
			lastSequenceNumber = RTP.previousSequenceNumber(firstSequenceNumber);
			numPackets++;
			frameBuffer = new Buffer();
			frameBuffer.setData(new byte[FRAME_BUFFER_INITIAL_SIZE]);
			frameBuffer.setOffset(0);
			frameBuffer.setLength(0);
			frameBuffer.setTimeStamp(inBuffer.getTimeStamp());
			outOfOrderFragments = new Vector();
			add(inBuffer);
		}

		public long getTimeStamp() {
			return frameBuffer.getTimeStamp();
		}

		public boolean isMissingFragments() {
			return outOfOrderFragments.size() != 0;
		}
		
		public void add(Buffer buffer) {
			if (!RTP.areConsecutiveSequenceNumbers(lastSequenceNumber, buffer.getSequenceNumber())) {
				if (RTP.compareSequenceNumbers(firstSequenceNumber, buffer.getSequenceNumber()) < 0) { // ensure sequence number isn't bizarre
					System.err.println("####mp4v.DePacketizer$MP4VFrame: saving out of order fragment: " + lastSequenceNumber + " not conseq with " + buffer.getSequenceNumber());
					saveOutOfOrderFragment(buffer);
				} else System.err.println("$$$$$mp4v.DePacketizer$MP4VFrame: strange fragment dropped: " + buffer.getSequenceNumber());
				return;
			}

//			System.err.println("mp4v.DePacketizer$MP4VFrame: adding buffer " + buffer.getSequenceNumber());

			int old_len = frameBuffer.getLength();
			frameBuffer.setData(validateByteArraySize(frameBuffer, frameBuffer.getLength()+buffer.getLength()));
			if (old_len < frameBuffer.getLength()) System.err.println("++++mp4v.DePacketizer: had to expand frameBuffer by " + (frameBuffer.getLength() - old_len));
			System.arraycopy((byte[])buffer.getData(), buffer.getOffset(), (byte[])frameBuffer.getData(), frameBuffer.getLength(), buffer.getLength());
			frameBuffer.setLength(frameBuffer.getLength()+buffer.getLength());
			numPackets++;
			lastSequenceNumber = buffer.getSequenceNumber();
			addFromQueuedFragments();

			//DEBUG
			try {
//				packDumpOut.write((byte[])buffer.getData(), buffer.getOffset(), buffer.getLength());
			} catch (Exception e) {
				System.err.println("mp4v.DePacketizer: failed to write packet dump: " + e.getMessage());
				e.printStackTrace();
			}
			//DEBUG
		}

		public byte[] getFrameBuffer() {
			return (byte[])frameBuffer.getData();
		}

		public int getDataLength() {
			return frameBuffer.getLength();
		}
		
		private void addFromQueuedFragments() {
			if (outOfOrderFragments.size() <= 0) return;
			Buffer storedBuf = (Buffer)outOfOrderFragments.get(0);
			if (RTP.areConsecutiveSequenceNumbers(lastSequenceNumber, storedBuf.getSequenceNumber())) {
				outOfOrderFragments.remove(0);
				add(storedBuf);
			}
		}

		private void saveOutOfOrderFragment(Buffer buffer) {
//			System.err.println("mp4v.DePacketizer$MP4VFrame: saving out of order packet");
			for (int i=0; i < outOfOrderFragments.size(); i++) {
				Buffer storedBuf = (Buffer)outOfOrderFragments.get(i);

				int cmpval = RTP.compareSequenceNumbers(buffer.getSequenceNumber(), storedBuf.getSequenceNumber());

				if (cmpval < 0) outOfOrderFragments.add(i, buffer);	// new buffer goes before stored
				else if (cmpval > 0) continue;	// new buffer goes after stored
				else return;	// cmpval == 0 -> duplicate, so drop

			}

			outOfOrderFragments.add(buffer);
		}
	}
}
