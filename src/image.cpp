#include <iostream>
#include <cstring>

#include "image.h"

unsigned int Image::getPixelIndex(int x, int y)
{
    y=-y;
	y = (y+height) % height;
	x = (x+width) % width;
	return (y*width + x)*channels;
}

void Image::setPixel(int x, int y, int value)
{
    image_data[getPixelIndex(x,y)] = (uint8_t) value;
}

int Image::getPixel(int x, int y)
{
    return image_data[getPixelIndex(x,y)];
}

unsigned int Image::size()
{
    return sizeof(uint8_t)*width*height*channels;
}

Image::Image(int width, int height, int channels)
: width(width), height(height), channels(channels)
{
    if (channels >= 2)
    {
        std::cout << "WARNING::IMAGE::INIT::CHANNELS" << std::endl;
        std::cout << "Images for channel count greater than 1 is not yet supported. Setting channels=1." << std::endl;
        channels = 1;
    }

    image_data = new uint8_t[width*height*channels];
    if (image_data == nullptr)
    {
        std::cout << "ERROR::IMAGE::INIT::NULL_PTR" << std::endl;
    }

    std::memset(image_data, 0, size());
}

Image::~Image()
{
    delete[] image_data;
}

