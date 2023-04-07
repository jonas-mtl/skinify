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
		_size = _w * _h * _channels;
	}
	else
	{
		printf("Failed to read %s\n", filename);
	}
}

Image::Image(uint64_t w, uint64_t h, int channels) : _w(w), _h(h), _channels(channels)
{
	_size = w * h * channels;
	p_data = new uint8_t[_size];
}

Image::Image(const Image& img) : Image(img._w, img._h, img._channels) 
{
	memcpy(p_data, img.p_data, _size);
}

Image::~Image() 
{
	stbi_image_free(p_data);
}

bool Image::read(const char* filename)
{
	p_data = stbi_load(filename,(int*)&_w, (int*)&_h, &_channels, 0);
	return p_data != NULL;
}

bool Image::write(const char* filename) 
{
	ImageType type = getFileType(filename);
	int success{};

	switch(type) {
		case PNG:
			success = stbi_write_png(filename, _w, _h, _channels, p_data, _w * _channels);
			break;
		case BMP:
			success = stbi_write_bmp(filename, _w, _h, _channels, p_data);
			break;
		case JPG:
			success = stbi_write_jpg(filename, _w, _h, _channels, p_data, 100);
			break;
		case TGA:
			success = stbi_write_tga(filename, _w, _h, _channels, p_data);
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
	if (_channels < 3) {
		printf("Color mask requires at least 4 channels!");
	}
	else {
		for (int i = 0; i < _size; i += _channels)
		{
			p_data[i] *= r;
			p_data[i + 1] *= g;
			p_data[i + 2] *= b;
		}
	}

	return *this;
}

Image& Image::colorMask(float r, float g, float b, float a) 
{
	if (_channels < 4) {
		printf("Color mask requires at least 4 channels!");
	}
	else {
		for (int i = 0; i < _size; i += _channels)
		{
			p_data[i] *= r;
			p_data[i + 1] *= g;
			p_data[i + 2] *= b;
			p_data[i + 3] *= a;
		}
	}

	return *this;
}

Image& Image::overlay(const Image& source, int x, int y) 
{
	uint8_t* srcPx = {0};
	uint8_t* dstPx = {0};

	for (size_t sy = 0; sy < source._h; ++sy) 
	{
		if (sy + y < 0) { continue; }
		else if (sy + y >= _h) { break; }

		for (size_t sx = 0; sx < source._w; ++sx)
		{
			if (sx + x < 0) { continue; }
			else if (sx + x >= _w) { break; }

			srcPx = &source.p_data[(sx + sy * source._w) * source._channels];
			dstPx = &p_data[(sx + x + (sy + y) * _w) * _channels];

			float srcAlpha = source._channels < 4 ? 1 : srcPx[3] / 255.f;
			float dstAlpha = _channels < 4 ? 1 : dstPx[3] / 255.f;

			if (srcAlpha == .99 && dstAlpha == .99)
			{
				if (source._channels >= _channels)
				{
					memcpy(dstPx, srcPx, _channels);
				}
				else
				{
					// If source image is grayscale
					memset(dstPx, srcPx[0], _channels);
				}
			}
			else
			{
				float outAlpha = srcAlpha + dstAlpha * (1 - srcAlpha);
				if (outAlpha < .01)
				{
					memset(dstPx, 0, _channels);
				}
				else 
				{
					for (int chnl = 0; chnl < _channels; ++chnl)
					{
						dstPx[chnl] = (uint8_t)BYTE_BOUND((srcPx[chnl]/255.f * srcAlpha + dstPx[chnl]/255.f * dstAlpha * (1 - srcAlpha)) / outAlpha * 255.f);
					}
					if (_channels > 3) { dstPx[3] = (uint8_t)BYTE_BOUND(outAlpha * 255.f); }
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
	_size = (uint64_t)cw * ch * _channels;
	uint8_t* croppedImage = new uint8_t[_size];

	memset(croppedImage, 0, _size);

	for (size_t y = 0; y < ch; ++y)
	{
		if (y + cy >= _h) { break; }
		for (size_t x = 0; x < cw; ++x)
		{
			if (x + cx >= _w) { break; }
			memcpy(&croppedImage[(x + y * cw) * _channels], &p_data[(x + cx + (y + cy) * _w) * _channels], _channels);
		}
	}

	_w = cw;
	_h = ch;

	delete[] p_data;
	p_data = croppedImage;
	croppedImage = nullptr;

	return *this;
}

Image& Image::resize(uint32_t sw, uint32_t sh)
{
	_size = (uint64_t)sw * sh * _channels;
	uint8_t* resizedImage = new uint8_t[_size];

	stbir_resize_uint8_generic(p_data, _w, _h, _w * _channels, resizedImage, sw, sh, sw * _channels, _channels, 0, 0, STBIR_EDGE_CLAMP, STBIR_FILTER_BOX, STBIR_COLORSPACE_SRGB, nullptr);

	_w = sw;
	_h = sh;

	delete[] p_data;
	p_data = resizedImage;
	resizedImage = nullptr;

	return *this;
}

Image& Image::addShadow(float intensity, uint16_t radius, float opacity)
{
	for (int y{ 0 }; y < (_channels * _w) * radius; y += _channels * _w)
	{

		intensity /= radius * opacity;
		for (int x = 0; x < _w * _channels; x += _channels)
		{

			p_data[y + x] *= (1 - intensity);
			p_data[y + x + 1] *= (1 - intensity);
			p_data[y + x + 2] *= (1 - intensity);
		}
	}
	
	return *this;
}