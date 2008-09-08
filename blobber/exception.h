
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
