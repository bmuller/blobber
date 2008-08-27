class Frame { 
 public:
    int     width;          ///< width in pixels
    int     height;         ///< height in pixels
    int     depth;          ///< bit depth (bits per pixel)
    int     format;         ///< VIDEO_* format
    void*   data;           ///< pointer to data buffer
    void allocateBuffer();
    Frame(int w, int h, int f) : width(w), height(h), format(f) { allocateBuffer(); };
    ~Frame() { debug("Freeing data buffer"); free(data); };
    int getSize();
};

