#ifndef IMAGE_H
#define IMAGE_H

#ifndef GL
#define GL
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <cstring>

class Image {
    public:
        int width;
        int height;
        int channels;

        // Returns index to first channel of pixel (x, y)
        void setPixel(int x, int y, int value);
        int getPixel(int x, int y); // This falls apart for 2+ channel textures

        uint8_t* data()
        {
            return image_data;
        }

        // Returns size of image in bytes 
        unsigned int size(); 

        // Constructor and destructor
        Image(int width, int height, int channels);
        ~Image();
        
    private:
        // Pointer to section of memory allocated with malloc
        uint8_t* image_data; 
        unsigned int getPixelIndex(int x, int y);
};

#endif