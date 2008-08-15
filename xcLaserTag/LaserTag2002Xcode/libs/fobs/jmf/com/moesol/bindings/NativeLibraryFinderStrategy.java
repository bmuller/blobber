package com.moesol.bindings;

public interface NativeLibraryFinderStrategy {
	public void findAndLoad(Class classInJar, String library);
}
