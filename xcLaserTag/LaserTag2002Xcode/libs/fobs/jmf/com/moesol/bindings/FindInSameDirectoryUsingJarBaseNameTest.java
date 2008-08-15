package com.moesol.bindings;

import junit.framework.TestCase;

public class FindInSameDirectoryUsingJarBaseNameTest extends TestCase {

	public void testComputeBaseName() {
		FindInSameDirectoryUsingJarBaseName t = new FindInSameDirectoryUsingJarBaseName();
		String r = t.computeBaseName("fobs4jmf-0.4.jar");
		assertEquals("fobs4jmf-0.4", r);
	}

}
