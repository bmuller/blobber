package com.moesol.bindings;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;

import junit.framework.TestCase;

public class FindInSameDirectoryTest extends TestCase {
	
	public void testComputeForFileUrl() throws MalformedURLException {
		FindInSameDirectory nf = new FindInSameDirectory();
		String resourceToFind = "/com/moesol/bindings/NativeLibaryFinderTest.class";
		nf.setClassAsResourcePath(resourceToFind);
		URL url = new URL("file:/c:/Program Files/yack/classes" + resourceToFind);
		String r = nf.computeDirectoryForFileUrl(url);
		String exp = new File("c:/Program Files/yack/classes").getAbsolutePath();
		assertEquals(exp, r);
	}
	
	public void testComputeForJarUrl() throws MalformedURLException {
		FindInSameDirectory nf = new FindInSameDirectory();
		String resourceToFind = "/com/moesol/bindings/NativeLibaryFinderTest.class";
		nf.setClassAsResourcePath(resourceToFind);
		URL url = new URL("jar:file:/c:/Program Files/yack/bar.jar!" + resourceToFind);
		String r = nf.computeDirectoryForJarUrl(url);
		String exp = new File("c:/Program Files/yack").getAbsolutePath();
		assertEquals(exp, r);
	}
	
	public void testComputePath() {
		FindInSameDirectory nf = new FindInSameDirectory();
		File r = nf.computePath("c:/Program Files/yack", "/WindowsXP/", "foo.dll");
		File e = new File("c:/Program Files/yack/WindowsXP", "foo.dll");
		assertEquals(e, r);
	}

}
