#Needs glut/openGL and Decoder.h or quicktime, and process.h
#Decoder.h is a part of fobs: http://fobs.sourceforge.net
#process.h is a windows? file, platform specific ifdefs in ofThread.h need work, TARGET_OSX && TARGET_LINUX both use pthreads
#need to compile fobs and make it a library
#Outside resources include: pthreads, glut, freeImage lib, and freetype
#Install glut, download a copy of openframeworks.... it may hve the files that you are missing
#don't forget the libraries, all of these are headers so far

LIBS=../../../libs

OPENFRAMEWORKS=${LIBS}/openFrameworks
OFAPP=${OPENFRAMEWORKS}/app
OFCOM=${OPENFRAMEWORKS}/communication
OFGRAPHICS=${OPENFRAMEWORKS}/graphics
OFSOUND=${OPENFRAMEWORKS}/sound
OFUTILS=${OPENFRAMEWORKS}/utils
OFVIDEO=${OPENFRAMEWORKS}/video

OFALL=-I${OPENFRAMEWORKS} -I${OFAPP} -I${OFCOM} -I${OFGRAPHICS} -I${OFSOUND} -I${OFUTILS} -I${OFVIDEO} -I${OFADDONS}
OFALLCPP=${OFAPP}/*.cpp ${OFCOM}/*.cpp ${OFGRAPHICS}/*.cpp ${OFSOUND}/*.cpp ${OFUTILS}/*.cpp ${OFVIDEO}/*.cpp #${OFADDONS}/*.cpp

OFADDONS=../../../libs/ofAddons
OFAOCOM=${OFADDONS}/communication
OFAOCV=${OFADDONS}/computerVision
OFAOGRAPHICS=${OFADDONS}/graphics
OFAOUTILS=${OFADDONS}/utils
OFAOVECTOR=${OFAOUTILS}/ofVectorMath

OFAOALL=-I${OFADDONS} -I${OFAOCOM} -I${OFAOCV} -I${OFAOGRAPHICS} -I${OFAOUTILS} -I${OFAOVECTOR}
OFAOALLCPP=${OFAOCOM}/*.cpp ${OFAOCV}/*.cpp ${OFAOGRAPHICS}/*.cpp ${OFAOUTILS}/*.cpp

GLEE=${LIBS}/GLee

FMODEX=${LIBS}/fmodex/inc
FMODEXLIB=${LIBS}/fmodex/lib

XML=${LIBS}/xml

OPENCV=${LIBS}/openCV/include

TCPUDP=${LIBS}/tcpUdpManager

FOBS=${LIBS}/fobs/core
FOBSLIB=${LIBS}/fobs/dist/lib

FREEIMAGE=${LIBS}/freeimage/include
FREEIMAGEA=${LIBS}/freeimage/lib

FREETYPEBASE=${LIBS}/freetype/include/include
FREETYPE2=${FREETYPEBASE}/freetype2
FREETYPECONFIG=${FREETYPE2}/freetype/config
FREETYPEINTERNAL=${FREETYPE2}/freetype/internal
FREETYPELIB=${LIBS}/freetype/lib

RTAUDIO=${LIBS}/rtAudio

OPENCVLIB=${LIBS}/openCV/lib
#ofTexture.cpp,ofImage.h,ofCore.h,ofTCPClient.cpp
APP=./app
DUR=./utils
DATAIN=./dataIn
DATAOUT=./dataOut
BRUSHES=${DATAOUT}/brushes
GBRUSHES=${BRUSHES}/gestureBrush
GMACHINES=${GBRUSHES}/gmachines_uncurler

laserTag: main.cpp testApp.cpp
	g++ -g -o laserTag main.cpp testApp.cpp ${APP}/*.cpp ${DUR}/*.cpp ${DATAIN}/*.cpp ${DATAOUT}/*.cpp ${BRUSHES}/*.cpp ${GBRUSHES}/*.cpp ${GMACHINES}/*.cpp ${OFALLCPP} ${OFAOALLCPP} ${XML}/*.cpp ${TCPUDP}/*.cpp ${GLEE}/*.c ${RTAUDIO}/*.cpp ${OFALL} ${OFAOALL} -I${GLEE} -I${FMODEX} -I${APP} -I${DUR} -I${DATAIN} -I${DATAOUT} -I${XML} -I${OPENCV} -I${TCPUDP} -I${BRUSHES} -I${GBRUSHES} -I${GMACHINES} -I${FOBS} -I${FREEIMAGE} -I${FREETYPEBASE} -I${FREETYPE2} -I${FREETYPECONFIG} -I${FREETYPEINTERNAL} -I${RTAUDIO} -L${FREEIMAGEA} -L${OPENCVLIB} -L${FMODEXLIB} -L${FOBSLIB} -lfreeimage -lpthread -lglut -lcv -lfreetype -lfmodex -lfobscore -lavcodec -lavformat
