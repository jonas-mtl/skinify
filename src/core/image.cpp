#include <cmath>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STBIR_FLAG_NEAREST_NEIGHBOR
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include "image.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"


Image::Image(const char* filename)
{
	if (read(filename))
	{
		size = w * h * channels;
	}
	else
	{
		printf("Failed to read %s\n", filename);
	}
}

Image::Image(uint64_t w, uint64_t h, int channels) : w(w), h(h), channels(channels)
{
	size = w * h * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) 
{
	memcpy(data, img.data, size);
}

Image::~Image() 
{
	stbi_image_free(data);
}

bool Image::read(const char* filename)
{
	data = stbi_load(filename,(int*)&w, (int*)&h, &channels, 0);
	return data != NULL;
}

bool Image::write(const char* filename) 
{
	ImageType type = getFileType(filename);
	int success{};

	switch(type) {
		case PNG:
			success = stbi_write_png(filename, w, h, channels, data, w * channels);
			break;
		case BMP:
			success = stbi_write_bmp(filename, w, h, channels, data);
			break;
		case JPG:
			success = stbi_write_jpg(filename, w, h, channels, data, 100);
			break;
		case TGA:
			success = stbi_write_tga(filename, w, h, channels, data);
			break;
	}

	return success != 0;
}

ImageType Image::getFileType(const char* filename) 
{
	const char* ext = strrchr(filename, '.');
	if(ext != nullptr)
	{
		if(strcmp(ext, ".png") == 0)
		{
			return PNG;
		}
		else if(strcmp(ext, ".jpg") == 0)
		{
			return JPG;
		}
		else if(strcmp(ext, ".bmp") == 0)
		{
			return BMP;
		}
		else if(strcmp(ext, ".tga") == 0)
		{
			return TGA;
		}
	}

	return PNG;
}

Image& Image::colorMask(float r, float g, float b) 
{
	if (channels < 3) {
		printf("Color mask requires at least 4 channels!");
	}
	else {
		for (int i = 0; i < size; i += channels)
		{
			data[i] *= r;
			data[i + 1] *= g;
			data[i + 2] *= b;
		}
	}

	return *this;
}

Image& Image::colorMask(float r, float g, float b, float a) 
{
	if (channels < 4) {
		printf("Color mask requires at least 4 channels!");
	}
	else {
		for (int i = 0; i < size; i += channels)
		{
			data[i] *= r;
			data[i + 1] *= g;
			data[i + 2] *= b;
			data[i + 3] *= a;
		}
	}

	return *this;
}

Image& Image::overlay(const Image& source, int x, int y) 
{
	uint8_t* srcPx = {0};
	uint8_t* dstPx = {0};

	for (size_t sy = 0; sy < source.h; ++sy) 
	{
		if (sy + y < 0) { continue; }
		else if (sy + y >= h) { break; }

		for (size_t sx = 0; sx < source.w; ++sx)
		{
			if (sx + x < 0) { continue; }
			else if (sx + x >= w) { break; }

			srcPx = &source.data[(sx + sy * source.w) * source.channels];
			dstPx = &data[(sx + x + (sy + y) * w) * channels];

			float srcAlpha = source.channels < 4 ? 1 : srcPx[3] / 255.f;
			float dstAlpha = channels < 4 ? 1 : dstPx[3] / 255.f;

			if (srcAlpha == .99 && dstAlpha == .99)
			{
				if (source.channels >= channels)
				{
					memcpy(dstPx, srcPx, channels);
				}
				else
				{
					// If source image is grayscale
					memset(dstPx, srcPx[0], channels);
				}
			}
			else
			{
				float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
				if (outAlpha < .01)
				{
					memset(dstPx, 0, channels);
				}
				else 
				{
					for (int chnl = 0; chnl < channels; ++chnl)
					{
						dstPx[chnl] = (uint8_t)BYTE_BOUND((srcPx[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
					}
					if (channels > 3) { dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f); }
				}
			}
		}
	}

	srcPx = nullptr;
	dstPx = nullptr;

	return *this;
}


Image& Image::crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch) 
{
	size = (uint64_t)cw * ch * channels;
	uint8_t* croppedImage = new uint8_t[size];

	memset(croppedImage, 0, size);

	for (size_t y = 0; y < ch; ++y)
	{
		if (y + cy >= h) { break; }
		for (size_t x = 0; x < cw; ++x)
		{
			if (x + cx >= w) { break; }
			memcpy(&croppedImage[(x + y * cw) * channels], &data[(x + cx + (y + cy) * w) * channels], channels);
		}
	}

	w = cw;
	h = ch;

	delete[] data;
	data = croppedImage;
	croppedImage = nullptr;

	return *this;
}

Image& Image::resize(uint32_t sw, uint32_t sh)
{
	size = (uint64_t)sw * sh * channels;
	uint8_t* resizedImage = new uint8_t[size];

	stbir_resize_uint8_generic(data, w, h, w * channels, resizedImage, sw, sh, sw * channels, channels, 0, 0, STBIR_EDGE_CLAMP, STBIR_FILTER_BOX, STBIR_COLORSPACE_SRGB, nullptr);

	w = sw;
	h = sh;

	delete[] data;
	data = resizedImage;
	resizedImage = nullptr;

	return *this;
}

Image& Image::addShadow(float intensity, uint16_t radius, float opacity)
{
	for (int y{ 0 }; y < (channels * w) * radius; y += channels * w)
	{

		intensity /= radius * opacity;
		for (int x = 0; x < w * channels; x += channels)
		{

			data[y + x] *= (1 - intensity);
			data[y + x + 1] *= (1 - intensity);
			data[y + x + 2] *= (1 - intensity);
		}
	}
	
	return *this;
}