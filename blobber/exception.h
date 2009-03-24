#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <exception>
#include <string>

namespace blobber {
  using namespace std;

  class BlobberException : public exception {
  public:
    string _w;
    explicit BlobberException(const string &s) : _w(s) {};
    virtual ~BlobberException() throw() {};
    virtual const char * what() const throw() { return _w.c_str(); };
  };

  class UnsupportedFrameFormatException : public BlobberException {
  public:
    UnsupportedFrameFormatException(string s) : BlobberException(s) {};
  };  

  class NoSuchVideoDeviceException : public BlobberException {
  public:
    NoSuchVideoDeviceException(string s) : BlobberException(s) {};
  };  

  class CameraReadException : public BlobberException {
  public:
    CameraReadException(string s) : BlobberException(s) {};
  };  

  class ModuleFileException : public BlobberException {
  public:
    ModuleFileException(string s) : BlobberException(s) {};
  };

  class ModuleLoadException : public BlobberException {
  public:
    ModuleLoadException(string s) : BlobberException(s) {};
  };

  class ModuleListException : public BlobberException {
  public:
    ModuleListException(string s) : BlobberException(s) {};
  };

  class ModuleRuntimeException : public BlobberException {
  public:
    ModuleRuntimeException(string s) : BlobberException(s) {};
  };

  class ConfigurationException : public BlobberException {
  public:
    ConfigurationException(string s) : BlobberException(s) {};
  };

  class GUIException : public BlobberException {
  public:
    GUIException(string s) : BlobberException(s) {};
  };

  class NoSuchFeatureException : public BlobberException {
  public:
    NoSuchFeatureException(string s) : BlobberException(s) {};
  };
};

#endif /* __EXCEPTION_H */

