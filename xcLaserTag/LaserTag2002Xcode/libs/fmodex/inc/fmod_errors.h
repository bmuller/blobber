/*$ preserve start $*/

/* ============================================================================================== */
/* FMOD Ex - Error string header file. Copyright (c), Firelight Technologies Pty, Ltd. 2004-2006. */
/*                                                                                                */
/* Use this header if you want to store or display a string version / english explanation of      */
/* the FMOD error codes.                                                                          */
/*                                                                                                */
/* ============================================================================================== */

#ifndef _FMOD_ERRORS_H
#define _FMOD_ERRORS_H

#include "fmod.h"

#ifdef __GNUC__ 
static char *FMOD_ErrorString(FMOD_RESULT errcode) __attribute__((unused));
#endif

static char *FMOD_ErrorString(FMOD_RESULT errcode)
{
    switch (errcode)
    {
/*$ preserve end $*/
        case FMOD_ERR_ALREADYLOCKED:          static char fAlreadyLocked[] = "Tried to call lock a second time before unlock was called. "; return &fAlreadyLocked[0];
        case FMOD_ERR_BADCOMMAND:             static char fBadCommand[] = "Tried to call a function on a data type that does not allow this type of functionality (ie calling Sound::lock on a streaming sound). "; return &fBadCommand[0];
        case FMOD_ERR_CDDA_DRIVERS:           static char fCDDAdrivers[] = "Neither NTSCSI nor ASPI could be initialised. "; return &fCDDAdrivers[0];
        case FMOD_ERR_CDDA_INIT:              static char fCDDAinit[] = "An error occurred while initialising the CDDA subsystem. "; return &fCDDAinit[0];
        case FMOD_ERR_CDDA_INVALID_DEVICE:    static char fCDDAinvalidDevice[] = "Couldn't find the specified device. "; return &fCDDAinvalidDevice[0];
        case FMOD_ERR_CDDA_NOAUDIO:           static char fCDDAnoAudio[] = "No audio tracks on the specified disc. "; return &fCDDAnoAudio[0];
        case FMOD_ERR_CDDA_NODEVICES:         static char fCDDAnoDevices[] = "No CD/DVD devices were found. "; return &fCDDAnoDevices[0];
        case FMOD_ERR_CDDA_NODISC:            static char fCDDAnoDisc[] = "No disc present in the specified drive. "; return &fCDDAnoDisc[0];
        case FMOD_ERR_CDDA_READ:              static char fCDDAread[] = "A CDDA read error occurred. "; return &fCDDAread[0];
        case FMOD_ERR_CHANNEL_ALLOC:          static char fChannelAlloc[] = "Error trying to allocate a channel. "; return &fChannelAlloc[0];
        case FMOD_ERR_CHANNEL_STOLEN:         static char fChannelStolen[] = "The specified channel has been reused to play another sound. "; return &fChannelStolen[0];
        case FMOD_ERR_COM:                    static char fCOM[] = "A Win32 COM related error occured. COM failed to initialize or a QueryInterface failed meaning a Windows codec or driver was not installed properly. "; return &fCOM[0];
        case FMOD_ERR_DMA:                    static char fDMA[] = "DMA Failure.  See debug output for more information. "; return &fDMA[0];
        case FMOD_ERR_DSP_CONNECTION:         static char fDSPconnection[] = "DSP connection error.  Connection possibly caused a cyclic dependancy. "; return &fDSPconnection[0];
        case FMOD_ERR_DSP_FORMAT:             static char fDSPformat[] = "DSP Format error.  A DSP unit may have attempted to connect to this network with the wrong format. "; return &fDSPformat[0];
        case FMOD_ERR_DSP_NOTFOUND:           static char fDSPnotFound[] = "DSP connection error.  Couldn't find the DSP unit specified. "; return &fDSPnotFound[0];
        case FMOD_ERR_DSP_RUNNING:            static char fDSPrunning[] = "DSP error.  Cannot perform this operation while the network is in the middle of running.  This will most likely happen if a connection or disconnection is attempted in a DSP callback. "; return &fDSPrunning[0];
        case FMOD_ERR_DSP_TOOMANYCONNECTIONS: static char fDSPtooManyConnections[] = "DSP connection error.  The unit being connected to or disconnected should only have 1 input or output. "; return &fDSPtooManyConnections[0];
        case FMOD_ERR_EVENT_FAILED:           static char fEventFailed[] = "An Event failed to be retrieved, most likely due to 'just fail' being specified as the max playbacks behavior. "; return &fEventFailed[0];
        case FMOD_ERR_EVENT_INFOONLY:         static char fEventInfoOnly[] = "Can't execute this command on an EVENT_INFOONLY event. "; return &fEventInfoOnly[0];
        case FMOD_ERR_EVENT_INTERNAL:         static char fEventInternal[] = "An error occured that wasn't supposed to.  See debug log for reason. "; return &fEventInternal[0];
        case FMOD_ERR_EVENT_MAXSTREAMS:       static char fEventMaxStreams[] = "Event failed because 'Max streams' was hit when FMOD_INIT_FAIL_ON_MAXSTREAMS was specified. "; return &fEventMaxStreams[0];
        case FMOD_ERR_EVENT_MISMATCH:         static char fEventMismatch[] = "FSB mis-matches the FEV it was compiled with. "; return &fEventMismatch[0];
        case FMOD_ERR_EVENT_NAMECONFLICT:     static char fEventNameConflict[] = "A category with the same name already exists. "; return &fEventNameConflict[0]; 
        case FMOD_ERR_EVENT_NOTFOUND:         static char fEventNotFound[] = "The requested event, event group, event category or event property could not be found. "; return &fEventNotFound[0];
        case FMOD_ERR_FILE_BAD:               static char fFileBad[] = "Error loading file. "; return &fFileBad[0];
        case FMOD_ERR_FILE_COULDNOTSEEK:      static char fFileCouldNotSeek[] = "Couldn't perform seek operation.  This is a limitation of the medium (ie netstreams) or the file format. "; return &fFileCouldNotSeek[0];
        case FMOD_ERR_FILE_DISKEJECTED:       static char fFileDiskEjected[] = "Media was ejected while reading. "; return &fFileDiskEjected[0];
        case FMOD_ERR_FILE_EOF:               static char fFileEOF[] = "End of file unexpectedly reached while trying to read essential data (truncated data?). "; return &fFileEOF[0];
        case FMOD_ERR_FILE_NOTFOUND:          static char fFileNotFound[] = "File not found. "; return &fFileNotFound[0];
        case FMOD_ERR_FILE_UNWANTED:          static char fFileUnwanted[] = "Unwanted file access occured. "; return &fFileUnwanted[0];
        case FMOD_ERR_FORMAT:                 static char fFormat[] = "Unsupported file or audio format. "; return &fFormat[0];
        case FMOD_ERR_HTTP:                   static char fHttp[] = "A HTTP error occurred. This is a catch-all for HTTP errors not listed elsewhere. ";return &fHttp[0];
        case FMOD_ERR_HTTP_ACCESS:            static char fHTTPaccess[] = "The specified resource requires authentication or is forbidden. "; return &fHTTPaccess[0];
        case FMOD_ERR_HTTP_PROXY_AUTH:        static char fHTTPproxyAuth[] = "Proxy authentication is required to access the specified resource. "; return &fHTTPproxyAuth[0];
        case FMOD_ERR_HTTP_SERVER_ERROR:      static char fHTTPserverError[] = "A HTTP server error occurred. "; return &fHTTPserverError[0];
        case FMOD_ERR_HTTP_TIMEOUT:           static char fHTTPtimeout[] = "The HTTP request timed out. "; return &fHTTPtimeout[0];
        case FMOD_ERR_INITIALIZATION:         static char fInitialization[] = "FMOD was not initialized correctly to support this function. "; return &fInitialization[0];
        case FMOD_ERR_INITIALIZED:            static char fInitialized[] = "Cannot call this command after System::init. "; return &fInitialized[0];
        case FMOD_ERR_INTERNAL:               static char fInternal[] = "An error occured that wasn't supposed to.  Contact support. "; return &fInternal[0];
        case FMOD_ERR_INVALID_ADDRESS:        static char fInvalidAddress[] = "On Xbox 360, this memory address passed to FMOD must be physical, (ie allocated with XPhysicalAlloc.) "; return &fInvalidAddress[0];
        case FMOD_ERR_INVALID_FLOAT:          static char fInvalidFloat[] = "Value passed in was a NaN, Inf or denormalized float. "; return &fInvalidFloat[0];
        case FMOD_ERR_INVALID_HANDLE:         static char fInvalidHandle[] = "An invalid object handle was used. ";return &fInvalidHandle[0]; 
        case FMOD_ERR_INVALID_PARAM:          static char fInvalidParam[] = "An invalid parameter was passed to this function. "; return &fInvalidParam[0];
        case FMOD_ERR_INVALID_SPEAKER:        static char fInvalidSpeaker[] = "An invalid speaker was passed to this function based on the current speaker mode. "; return &fInvalidSpeaker[0];
        case FMOD_ERR_INVALID_VECTOR:         static char fInvalidVector[] = "The vectors passed in are not unit length, or perpendicular. "; return &fInvalidVector[0];
        case FMOD_ERR_IRX:                    static char fIRX[] = "PS2 only.  fmodex.irx failed to initialize.  This is most likely because you forgot to load it. "; return &fIRX[0];
        case FMOD_ERR_MAXAUDIBLE:             static char fMaxAudible[] = "Reached maximum audible playback count for this sound's soundgroup. "; return &fMaxAudible[0];
        case FMOD_ERR_MEMORY:                 static char fMemory[] = "Not enough memory or resources. "; return &fMemory[0];
        case FMOD_ERR_MEMORY_CANTPOINT:       static char fMemoryCantPoint[] = "Can't use FMOD_OPENMEMORY_POINT on non PCM source data, or non mp3/xma/adpcm data if FMOD_CREATECOMPRESSEDSAMPLE was used. "; return &fMemoryCantPoint[0];
        case FMOD_ERR_MEMORY_IOP:             static char fMemoryIOP[] = "PS2 only.  Not enough memory or resources on PlayStation 2 IOP ram. "; return &fMemoryIOP[0];
        case FMOD_ERR_MEMORY_SRAM:            static char fMemorySRAM[] = "Not enough memory or resources on console sound ram. "; return &fMemorySRAM[0];
        case FMOD_ERR_NEEDS2D:                static char fNeeds2D[] = "Tried to call a command on a 3d sound when the command was meant for 2d sound. "; return &fNeeds2D[0];
        case FMOD_ERR_NEEDS3D:                static char fNeeds3D[] = "Tried to call a command on a 2d sound when the command was meant for 3d sound. "; return &fNeeds3D[0];
        case FMOD_ERR_NEEDSHARDWARE:          static char fNeedsHardware[] = "Tried to use a feature that requires hardware support.  (ie trying to play a VAG compressed sound in software on PS2). "; return &fNeedsHardware[0];
        case FMOD_ERR_NEEDSSOFTWARE:          static char fNeedsSoftware[] = "Tried to use a feature that requires the software engine.  Software engine has either been turned off, or command was executed on a hardware channel which does not support this feature. "; return &fNeedsSoftware[0];
        case FMOD_ERR_NET_CONNECT:            static char fNetConnect[] = "Couldn't connect to the specified host. "; return &fNetConnect[0];
        case FMOD_ERR_NET_SOCKET_ERROR:       static char fNetSocketError[] = "A socket error occurred.  This is a catch-all for socket-related errors not listed elsewhere. "; return &fNetSocketError[0];
        case FMOD_ERR_NET_URL:                static char fNetURL[] = "The specified URL couldn't be resolved. "; return &fNetURL[0];
        case FMOD_ERR_NET_WOULD_BLOCK:        static char fNetWouldBlock[] = "Operation on a non-blocking socket could not complete immediately. "; return &fNetWouldBlock[0];
        case FMOD_ERR_NOTREADY:               static char fNotReady[] = "Operation could not be performed because specified sound is not ready. "; return &fNotReady[0];
        case FMOD_ERR_OUTPUT_ALLOCATED:       static char fOutputAllocated[] = "Error initializing output device, but more specifically, the output device is already in use and cannot be reused. "; return &fOutputAllocated[0];
        case FMOD_ERR_OUTPUT_CREATEBUFFER:    static char fOutputCreateBuffer[] = "Error creating hardware sound buffer. "; return &fOutputCreateBuffer[0];
        case FMOD_ERR_OUTPUT_DRIVERCALL:      static char fOutputDriverCall[] = "A call to a standard soundcard driver failed, which could possibly mean a bug in the driver or resources were missing or exhausted. "; return &fOutputDriverCall[0];
        case FMOD_ERR_OUTPUT_FORMAT:          static char fOutputFormat[] = "Soundcard does not support the minimum features needed for this soundsystem (16bit stereo output). "; return &fOutputFormat[0];
        case FMOD_ERR_OUTPUT_INIT:            static char fOutputInit[] = "Error initializing output device. "; return &fOutputInit[0];
        case FMOD_ERR_OUTPUT_NOHARDWARE:      static char fOutputNoHardware[] = "FMOD_HARDWARE was specified but the sound card does not have the resources nescessary to play it. "; return &fOutputNoHardware[0];
        case FMOD_ERR_OUTPUT_NOSOFTWARE:      static char fOutputNoSoftware[] = "Attempted to create a software sound but no software channels were specified in System::init. "; return &fOutputNoSoftware[0];
        case FMOD_ERR_PAN:                    static char fPan[] = "Panning only works with mono or stereo sound sources. "; return &fPan[0];
        case FMOD_ERR_PLUGIN:                 static char fPlugin[] = "An unspecified error has been returned from a 3rd party plugin. "; return &fPlugin[0];
        case FMOD_ERR_PLUGIN_INSTANCES:       static char fPluginInstances[] = "The number of allowed instances of a plugin has been exceeded "; return &fPluginInstances[0];
        case FMOD_ERR_PLUGIN_MISSING:         static char fPluginMissing[] = "A requested output, dsp unit type or codec was not available. "; return &fPluginMissing[0];
        case FMOD_ERR_PLUGIN_RESOURCE:        static char fPluginResource[] = "A resource that the plugin requires cannot be found. (ie the DLS file for MIDI playback) "; return &fPluginResource[0];
        case FMOD_ERR_RECORD:                 static char fRecord[] = "An error occured trying to initialize the recording device. "; return &fRecord[0];
        case FMOD_ERR_REVERB_INSTANCE:        static char fReverbInstance[] = "Specified Instance in FMOD_REVERB_PROPERTIES couldn't be set. Most likely because another application has locked the EAX4 FX slot. "; return &fReverbInstance[0];
        case FMOD_ERR_SUBSOUNDS:              static char fSubSounds[] = "The error occured because the sound referenced contains subsounds.  (ie you cannot play the parent sound as a static sample, only its subsounds.) "; return &fSubSounds[0];
        case FMOD_ERR_SUBSOUND_ALLOCATED:     static char fSubSoundAllocated[] = "This subsound is already being used by another sound, you cannot have more than one parent to a sound.  Null out the other parent's entry first. "; return &fSubSoundAllocated[0];
        case FMOD_ERR_TAGNOTFOUND:            static char fTagNotFound[] = "The specified tag could not be found or there are no tags. "; return &fTagNotFound[0];
        case FMOD_ERR_TOOMANYCHANNELS:        static char fTooManyChannels[] = "The sound created exceeds the allowable input channel count.  This can be increased using the maxinputchannels parameter in System::setSoftwareFormat. "; return &fTooManyChannels[0];
        case FMOD_ERR_UNIMPLEMENTED:          static char funImplemented[] = "Something in FMOD hasn't been implemented when it should be! contact support! "; return &funImplemented[0];
        case FMOD_ERR_UNINITIALIZED:          static char funInitialized[] = "This command failed because System::init or System::setDriver was not called. "; return &funInitialized[0];
        case FMOD_ERR_UNSUPPORTED:            static char funSupported[] = "A command issued was not supported by this object.  Possibly a plugin without certain callbacks specified. "; return &funSupported[0];
        case FMOD_ERR_UPDATE:                 static char fUpdate[] = "An error caused by System::update occured. "; return &fUpdate[0];
        case FMOD_ERR_VERSION:                static char fVersion[] = "The version number of this file format is not supported. "; return &fVersion[0];
        case FMOD_OK:                         static char fOk[] = "No errors."; return &fOk[0];
        default :                             static char fDefault[] = "Unknown error."; return &fDefault[0];
/*$ preserve start $*/
    };
}

#endif
/*$ preserve end $*/
