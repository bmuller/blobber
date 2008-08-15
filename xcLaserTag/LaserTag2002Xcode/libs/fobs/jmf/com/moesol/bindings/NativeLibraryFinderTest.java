package com.moesol.bindings;

import java.util.logging.Level;
import java.util.logging.Logger;

import junit.framework.TestCase;

public class NativeLibraryFinderTest extends TestCase {

	/**
	 * Not much of a test, but does log all the different
	 * combinations that are tried, and checks that we end
	 * up throwing the original exception if all fail.
	 */
	public void testFailedFind() {
		Logger l = Logger.getLogger("");
		Level oldLevel = l.getLevel();
		l.setLevel(Level.FINE);
		try {
			Logger.getLogger("").getHandlers()[0].setLevel(Level.ALL);
			NativeLibraryFinder nf = new NativeLibraryFinder();
			try {
				nf.loadLibrary(getClass(), "foo");
			} catch (UnsatisfiedLinkError e) {
				assertEquals("no foo in java.library.path", e.getMessage());
			}
		} finally {
			l.setLevel(oldLevel);
		}
	}
}
