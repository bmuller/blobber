
namespace blobber {

  class BlobberException : public std::exception {
  public:
    std::string _w;
    explicit BlobberException(const std::string &s) : _w(s) {};
    virtual ~BlobberException() throw() {};
    virtual const char * what() const throw() { return _w.c_str(); };
  };

  class UnsupportedFrameFormatException : public BlobberException {
  public:
    UnsupportedFrameFormatException(std::string s) : BlobberException(s) {};
  };  

  class NoSuchVideoDeviceException : public BlobberException {
  public:
    NoSuchVideoDeviceException(std::string s) : BlobberException(s) {};
  };  

  class CameraReadException : public BlobberException {
  public:
    CameraReadException(std::string s) : BlobberException(s) {};
  };  

  class ModuleFileException : public BlobberException {
  public:
    ModuleFileException(std::string s) : BlobberException(s) {};
  };

  class ModuleLoadException : public BlobberException {
  public:
    ModuleLoadException(std::string s) : BlobberException(s) {};
  };

  class ModuleListException : public BlobberException {
  public:
    ModuleListException(std::string s) : BlobberException(s) {};
  };

  class ModuleRuntimeException : public BlobberException {
  public:
    ModuleRuntimeException(std::string s) : BlobberException(s) {};
  };

  class ConfigurationException : public BlobberException {
  public:
    ConfigurationException(std::string s) : BlobberException(s) {};
  };

  class GUIException : public BlobberException {
  public:
    GUIException(std::string s) : BlobberException(s) {};
  };

  class NoSuchFeatureException : public BlobberException {
  public:
    NoSuchFeatureException(std::string s) : BlobberException(s) {};
  };
};
