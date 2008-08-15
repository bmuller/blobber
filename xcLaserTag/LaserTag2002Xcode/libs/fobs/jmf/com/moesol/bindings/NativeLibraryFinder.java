package com.moesol.bindings;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

public class NativeLibraryFinder {
	private static final Logger s_logger = Logger.getLogger(NativeLibraryFinder.class.getName());
	private static final ArrayList s_stategies = new ArrayList();
	static {
		s_stategies.add(new FindInSameDirectory());
		s_stategies.add(new FindInSameDirectoryUsingJarBaseName());
		s_stategies.add(new FindAsResource());
	}
	public static String[] OS_PREFIXES = {
		"/",
		"/" + System.getProperty("os.name", "") + "/" + System.getProperty("os.arch", "") + "/",
		"/" + System.getProperty("os.name", "") + "/" + System.getProperty("os.arch", "") + "/" + System.getProperty("os.version", "") + "/",
	};
	private static File s_libraryDirectory = new File(System.getProperty("user.home", "."), ".native");
	
	/**
	 * Finds and loads a native library using several different
	 * strategies. The default set of strategies tries to
	 * find the native library using the following search:
	 * <ol>
	 * <li>tries System.loadLibrary</li>
	 * <li>tries to find the library in the same directory as the jar.
	 * If the URL to the jar is not the <code>file:</code>
	 * protocol this step is skipped.</li>
	 * <li>tries to find a library with the same name as the
	 * jar file with the .jar extension removed.
	 * This supports maven since maven will rename artifacts so
	 * that the name of the jar file and native library will
	 * include the version number. For example, on Windows a jar 
	 * file of <code>fobs4jmf-0.4.jar</code> will cause a search 
	 * for <code>fobs4jmf-0.4.dll</code></li> 
	 * <li>tries to find the library as a resource.
	 * If found as a resource the native library is extracted to the 
	 * library directory property and loaded from there.</li>
	 * </ol>
	 * <p>
	 * For both the file and resource searches it also searchs
	 * for derivations based on the os, cpu architecture, and os version.
	 * So on Windows a <code>loadLibrary("foo")</code> looks for
	 * <pre>
	 * {jar or classes directory}\foo.dll
	 * {jar or classes directory}\Windows XP\x86\foo.dll
	 * {jar or classes directory}\Windows XP\x86\5.1\foo.dll
	 * {resource}/foo.dll
	 * {resource}/Windows XP/x86/foo.dll
	 * {resource}/Windows XP/x86/5.1/foo.dll
	 * </pre>
	 *   
	 * @param classInJar a class that is in the jar to be located
	 * @param libraryName name of library to load
	 */
	public static void loadLibrary(Class classInJar, String libraryName) {
		try {
			System.loadLibrary(libraryName);
		} catch (UnsatisfiedLinkError e) {
			for (Iterator it = s_stategies.iterator(); it.hasNext(); ) {
				NativeLibraryFinderStrategy s = (NativeLibraryFinderStrategy)it.next();
				try {
					s.findAndLoad(classInJar, libraryName);
					return; // worked
				} catch (UnsatisfiedLinkError e1) {
					s_logger.log(Level.FINE, "strategy failed: " + s, e1);
				}
			}
			throw e; // Throw the original System error for consistency
		}
	}

	public static File getLibraryDirectory() {
		return s_libraryDirectory;
	}

	public static void setLibraryDirectory(File libraryDirectory) {
		s_libraryDirectory = libraryDirectory;
	}
	
	public static List getStrategies() {
		return s_stategies;
	}
}
