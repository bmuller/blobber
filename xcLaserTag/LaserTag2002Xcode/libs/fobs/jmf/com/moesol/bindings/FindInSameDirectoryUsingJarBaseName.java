package com.moesol.bindings;

import java.io.File;
import java.net.URL;

public class FindInSameDirectoryUsingJarBaseName extends FindInSameDirectory {

	protected String templateGetMappedName(Class classInJar, String libraryName) {
		URL url = classInJar.getResource(getClassAsResourcePath());
		if ("jar".equals(url.getProtocol())) {
			File jarFile = computeJarFile(url);
			String jar = jarFile.getName();
			String baseName = computeBaseName(jar);
			return System.mapLibraryName(baseName);
		}
		throw new UnsatisfiedLinkError("not jar url");
	}

	String computeBaseName(String jar) {
		return jar.replaceAll("\\.[^.]+$", "");
	}
}
