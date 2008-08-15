import os
import sys
import subprocess

import fobs
import string

import os
from string import Template

class pkgconfig(dict):
	def __init__(self, package, paths):
		self._user_paths = paths
		self._load(package, paths)

	def _load(self, package, paths):
		_paths = ['/usr/lib/pkgconfig', '/usr/local/lib/pkgconfig'] + paths
		for path in _paths:
			fn = os.path.join(path, '%s.pc' % package)
		if os.path.exists(fn):
			self._parse(fn)
  
	def _parse(self, filename):
		lines = open(filename).readlines()
		lokals = {}
	
		for line in lines:
			line = line.strip()
	  
			if not line:
				continue
			elif ':' in line: # exported variable
				name, val = line.split(':')
				val = val.strip()
				if '$' in val:
					try:
						val = Template(val).substitute(lokals)
					except ValueError:
						raise ValueError("Error in variable substitution!")
				self[name] = val
			elif '=' in line: # local variable
				name, val = line.split('=')
				if '$' in val:
					try:
						val = Template(val).substitute(lokals)
					except ValueError:
						raise ValueError("Error in variable substitution!")
				lokals[name] = val
		
		for item in self.get('Requires','').split():
			if item == '=':
				break
			pkgClass = pkgconfig(item, self._user_paths)
			self.merge(pkgClass)
	
	def merge(self, pkgc):
		self['Libs']=self.get('Libs','')+' '+pkgc.get('Libs','')
		self['Cflags']=self.get('Cflags','')+' '+pkgc.get('Cflags','')
	
	def get_output(self):
		return self['Libs']+" "+self['Cflags']


def CustomParseConfig(env, command):
	newenv = os.environ
	for k,v in env['ENV'].iteritems():
		newenv[k]=v
	proc = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE,stderr=subprocess.PIPE,env=newenv)
	out = proc.stdout.read()
	proc.stdout.close()
	err = proc.stderr.read()
	proc.stderr.close()
	status = proc.wait()
	if err:
		sys.stderr.write(err)
	if status:
		raise OSError("'%s' exited %d" % (command,status))
	return env.MergeFlags(out)


def walkDirs(path):
	"""helper function to get a list of all subdirectories"""
	def addDirs(pathlist, dirname, names):
		"""internal function to pass to os.path.walk"""
		for n in names:
			f = os.path.join(dirname, n)
			if os.path.isdir(f):
				pathlist.append(f)
	pathlist = [path]
	os.path.walk(path, addDirs, pathlist)
	return pathlist

def ConfigureJNI(env):
	"""Configure the given environment for compiling Java Native Interface
	   c or c++ language files."""
	try:
		# if not env.get('JAVAC'):
		#	  print "The Java compiler must be installed and in the current path."
		#	  return 0
		#
		# first look for a shell variable called JAVA_HOME
		java_base = os.environ.get('JAVA_HOME')
		if not java_base:
			if sys.platform == 'darwin':
				# Apple's OS X has its own special java base directory
				java_base = '/System/Library/Frameworks/JavaVM.framework'
			else:
				# Search for the java compiler
				print "JAVA_HOME environment variable is not set. Searching for java... ",
				jcdir = os.path.dirname(os.path.realpath(env.WhereIs('javac')))
				if not jcdir:
					print "not found."
					return False
				# assuming the compiler found is in some directory like
				# /usr/jdkX.X/bin/javac, java's home directory is /usr/jdkX.X
				java_base = os.path.abspath(os.path.join(jcdir, ".."))
				print "found."
	
		if sys.platform=='win32':
			java_base=os.path.abspath(java_base)

		print "\tJava found in: ",java_base

		if sys.platform == 'cygwin':
			# Cygwin and Sun Java have different ideas of how path names
			# are defined. Use cygpath to convert the windows path to
			# a cygwin path. i.e. C:\jdkX.X to /cygdrive/c/jdkX.X
			java_base = string.replace( \
				os.popen("cygpath -up '"+java_base+"'").read(), '\n', '')

		if sys.platform == 'darwin':
			# Apple does not use Sun's naming convention
			java_headers = [os.path.join(java_base, 'Headers')]
			java_libs = [os.path.join(java_base, 'Libraries')]
		else:
			# windows and linux
			java_headers = [os.path.join(java_base, 'include')]
			java_libs = [os.path.join(java_base, 'lib')]
			# Sun's windows and linux JDKs keep system-specific header
			# files in a sub-directory of include
			
			#test if jni.h exists in the target dir
			jni_h_name = os.path.join(java_base, 'include', 'jni.h')
			if not os.path.exists(jni_h_name):
				print "Java detected in %s, but jni.h could not be found. Is this a JDK or a JRE folder?"%java_base
				return false


			if java_base == '/usr' or java_base == '/usr/local':
				# too many possible subdirectories. Just use defaults
				java_headers.append(os.path.join(java_headers[0], 'win32'))
				java_headers.append(os.path.join(java_headers[0], 'linux'))
				java_headers.append(os.path.join(java_headers[0], 'solaris'))
			else:
				# add all subdirs of 'include'. The system specific headers
				# should be in there somewhere
				java_headers = walkDirs(java_headers[0])

		# add Java's include and lib directory to the environment
		env.Append(CPPPATH = java_headers)
		env.Append(LIBPATH = java_libs)

		# add any special platform-specific compilation or linking flags
		if sys.platform == 'darwin':
			env.Append(SHLINKFLAGS = '-dynamiclib -framework JavaVM')
			env['SHLIBSUFFIX'] = '.jnilib'
		elif sys.platform == 'cygwin':
			env.Append(CCFLAGS = '-mno-cygwin')
			env.Append(SHLINKFLAGS = '-mno-cygwin -Wl,--kill-at')
		# Add extra potentially useful environment variables
		env['JAVA_HOME'] = java_base
		env['JNI_CPPPATH'] = java_headers
		env['JNI_LIBPATH'] = java_libs
		return True
	except:
		return False
