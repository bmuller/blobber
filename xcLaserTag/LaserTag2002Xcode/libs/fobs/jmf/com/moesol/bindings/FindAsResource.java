package com.moesol.bindings;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

public class FindAsResource implements NativeLibraryFinderStrategy {

	private static final Logger s_logger = Logger.getLogger(FindAsResource.class.getName());

	public void findAndLoad(Class classInJar, String library) {
		String mappedName = System.mapLibraryName(library);
		InputStream is = findLibraryResource(classInJar, mappedName);
		
		try {
			NativeLibraryFinder.getLibraryDirectory().mkdirs();
			File outputFile = computeOutputFile(NativeLibraryFinder.getLibraryDirectory(), mappedName);
			outputFile.deleteOnExit(); // delete on exit 
			copyStream(is, outputFile);
			System.load(outputFile.getAbsolutePath());
		} catch (IOException e) {
			UnsatisfiedLinkError le = new UnsatisfiedLinkError("Failed to extract native library");
			le.initCause(e);
			throw le;
		} finally {
			safeClose(is);
		}
	}

	InputStream findLibraryResource(Class classInJar, String mappedName) {
		for (int i = 0; i < NativeLibraryFinder.OS_PREFIXES.length; i++) {
			String osPrefix = NativeLibraryFinder.OS_PREFIXES[i];
			String resource = osPrefix + mappedName;
			s_logger.log(Level.FINE, "trying {0}", resource);
			InputStream is = classInJar.getResourceAsStream(resource);
			if (is != null) {
				return is;
			}
		}
		throw new UnsatisfiedLinkError("Failed to find: " + mappedName);
	}

	/**
	 * Compute an unique name for us to copy the library to.
	 * The file returned will not be in use by any other process.
	 * 
	 * @param mappedName
	 * @return a unique filename
	 * @throws IOException
	 */
	static File computeOutputFile(File outputDirectory, String mappedName) throws IOException {
		for (int i = 0; i < 1024; i++) {
			File check = new File(outputDirectory, mappedName + "." + i);
			if (check.createNewFile()) {
				return check;
			}
		}
		throw new RuntimeException("More that 1024 processes are running that are using or have failed to clean: " + outputDirectory + "/" + mappedName);
	}
	
	private static void copyStream(InputStream is, File outputFile) throws IOException {
		OutputStream os = null;
		try {
			os = new BufferedOutputStream(new FileOutputStream(outputFile));
			int c;
			while ((c = is.read()) != -1) {
				os.write(c);
			}
		} finally {
			safeClose(os);
		}
	}

	private static void safeClose(InputStream closable) {
		if (closable == null) {
			return;
		}
		try {
			closable.close();
		} catch (IOException e) {
			s_logger.log(Level.INFO, "Failed to close", e);
		}
	}

	private static void safeClose(OutputStream closable) {
		if (closable == null) {
			return;
		}
		try {
			closable.close();
		} catch (IOException e) {
			s_logger.log(Level.INFO, "Failed to close", e);
		}
	}
}
