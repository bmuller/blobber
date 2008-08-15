package com.moesol.bindings;

import java.io.File;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

public class FindInSameDirectory implements NativeLibraryFinderStrategy {

	private static final Logger s_logger = Logger.getLogger(FindInSameDirectory.class.getName());
	private String m_classAsResourcePath;

	public void findAndLoad(Class classInJar, String libraryName) {
		String directory = computeDirectoryStringForClass(classInJar);
		if (directory != null) {
			tryOsPrefixes(directory, templateGetMappedName(classInJar, libraryName));
		}
	}
	
	/**
	 * Override to change the mapped name this class uses to search for
	 * the native library.
	 * 
	 * @param classInJar
	 * @param libraryName
	 * @return mapped name of library
	 */
	protected String templateGetMappedName(Class classInJar, String libraryName) {
		return System.mapLibraryName(libraryName);
	}

	private String computeDirectoryStringForClass(Class classInJar) {
		m_classAsResourcePath = computeResourceToFind(classInJar);
		URL url = classInJar.getResource(m_classAsResourcePath);
		String directory = null;
		if ("file".equals(url.getProtocol())) {
			directory = computeDirectoryForFileUrl(url);
		}
		if ("jar".equals(url.getProtocol())) {
			directory = computeDirectoryForJarUrl(url);
		}
		return directory;
	}

	/**
	 * Computes the directory where NativeLibraryFinder will search for
	 * a native library. If <code>classInJar</code> is a class in a jar
	 * file then it will return the dirctory that contains the jar file.
	 * This is a common installation setup where foo.jar and foo.dll are
	 * installed into the same directory. If the jar file was downloaded 
	 * via http then this method will return <code>null</code>. 
	 * 
	 * <p>If <code>classInJar</code> is a class that was not contained in a
	 * jar file, then this will return the directory that is the root of
	 * the class tree. This is the same location as a resource with the
	 * path of "/" would return. This is useful if the native library
	 * will eventually be packaged as a resource in a jar file, because
	 * finding the library directory here will allow the code to skip extracting
	 * the native library.</p>
	 * 
	 * @param classInJar
	 * @return the directory that will be searched for native libraries for
	 * <code>classInJar</code> or null if <code>classInJar</code> is downloaded.
	 */
	public File computeDirectoryForClass(Class classInJar) {
		String strDir = computeDirectoryStringForClass(classInJar);
		if (strDir == null) {
			return null;
		}
		return new File(strDir);
	}

	private String computeResourceToFind(Class classInJar) {
		String pathToClass = classInJar.getName().replace('.', '/');
		String resourceToFind = "/" + pathToClass + ".class";
		return resourceToFind;
	}
	
	String computeDirectoryForJarUrl(URL url) {
		File jarFile = computeJarFile(url);
		return jarFile.getParent();
	}

	protected File computeJarFile(URL url) {
		String strUrl = url.toExternalForm();
		String jarPath = strUrl.substring("jar:file:".length(), strUrl.length() - m_classAsResourcePath.length() - "!".length());
		
		File jarFile = new File(jarPath);
		return jarFile;
	}

	String computeDirectoryForFileUrl(URL url) {
		String strUrl = url.toExternalForm(); 
		String classesDirectory = strUrl.substring("file:".length(), strUrl.length() - m_classAsResourcePath.length());
		File dir = new File(classesDirectory);
		return dir.getAbsolutePath();
	}
	
	void tryOsPrefixes(String directory, String mappedName) {
		for (int i = 0; i < NativeLibraryFinder.OS_PREFIXES.length; i++) {
			String osPrefix = NativeLibraryFinder.OS_PREFIXES[i];
			File fullPath = computePath(directory, osPrefix, mappedName);
			s_logger.log(Level.FINE, "trying {0}", fullPath);
			if (fullPath.exists()) {
				System.load(fullPath.getAbsolutePath());
				return;
			}
		}
		throw new UnsatisfiedLinkError("No library found");
	}

	File computePath(String directory, String osPrefix, String mappedName) {
		return new File(directory, osPrefix + mappedName);
	}

	public void setClassAsResourcePath(String resourceToFind) {
		m_classAsResourcePath = resourceToFind;
	}
	
	public String getClassAsResourcePath() {
		return m_classAsResourcePath;
	}
}
