/* $Id: RTP.java,v 1.1 2005/12/28 15:56:58 jsanpedro Exp $ */
/**
 * Neon Standard Utilities
 * RTP Utilities
 * @author Scott Hays
 * @created 2005/08/26
 * Copyright (c) 2005 Neon Advanced Technologies Co., Ltd.  All rights reserved.
 *
 * The Neon RTP Utility is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 *
 * The Neon RTP Utility is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 */
package com.neon.util;

public class RTP {
	private static int MAX_SEQ_NUM = 65535;
	
	public static boolean areConsecutiveSequenceNumbers(long numOne, long numTwo) {
		if (numOne == MAX_SEQ_NUM && numTwo == 0) return true;
		if (numOne+1 == numTwo) return true;
		
		return false;
	}
	
	public static long previousSequenceNumber(long seq) {
		if (seq == 0) return MAX_SEQ_NUM;
		return seq-1;
	}
	
	public static long nextSequenceNumber(long seq) {
		if (seq == MAX_SEQ_NUM) return 0;
		return seq+1;
	}
	
	// 1 -> p > c
	// -1 -> p < c
	// 0 -> p == c
	public static int compareSequenceNumbers(long p, long c) {
		// Allow for the case where sequence number has wrapped.
		if (p > MAX_SEQ_NUM - 100 && c < 100) return 1;
		if (c > MAX_SEQ_NUM - 100 && p < 100) return -1;
		
		if (p < c) return -1;
		if (p == c) return 0;
		
		return 1;
	}
}
