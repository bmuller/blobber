#ifndef FRAME_H
#define FRAME_H

#include<iostream>
#include<string>
#include<fcntl.h>
#include<linux/videodev2.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<sys/errno.h>
#include<sys/mman.h>

#include"pixconverter.h"

using namespace std;

// From V4L2 spec:
#define CLEAR(x) memset (&(x), 0, sizeof (x))

class Frame
{
  public:
    Frame(std::string devName);
    ~Frame();

    void update();
    void setDevice(std::string devName);
    bool setBrightness(double percent);
    bool setContrast(double percent);
    bool setExposure(double percent, bool isAuto);
    bool setSaturation(double percent);
    int getWidth();
    int getHeight();
    void* getCapture();

  protected:
    std:: string device;
    int devID;
    int numBuffers;
    struct v4l2_capability capabilities;
    struct v4l2_format format;
    struct buffer{void* start;
                  size_t length;}
                  *buffers;
    struct v4l2_requestbuffers requestBuffers;
    void* capture;
    YUYVtoGL_RGB* convert;

    int xioctl(int fd,
               int request,
               void* arg);
    int tioctl(int fd,
               int request,
               void* arg);
    void openDevice();
    void closeDevice();
    void initDevice();
};

#endif
