typedef struct
{
  int     width;          ///< width in pixels                                                                            
  int     height;         ///< height in pixels                                                                           
  int     depth;          ///< bit depth (bits per pixel)                                                                 
  int     format;         ///< VIDEO_* format                                                                             
  void*   data;           ///< pointer to data buffer                                                                     
} FRAME;
