#pragma once

#include <cmath>
#include <iostream>
#include <cstdio>
#include <stdint.h>


enum ImageType 
{
	PNG, JPG, BMP, TGA
};

struct Image 
{
	uint8_t*		p_data{ NULL };
	size_t			_size{ 0 };
	int				_channels{};
	uint64_t		_w{};
	uint64_t		_h{};
	bool			_status{ false };

	Image(const char* filename);
	Image(uint64_t w, uint64_t h, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);

	Image& colorMask(float r, float g, float b);
	Image& colorMask(float r, float g, float b, float a);

	Image& overlay(const Image& source, int x, int y);
	Image& resize(uint32_t width, uint32_t height);
	Image& addShadow(float intensity, uint16_t radius, float opacity);

	Image& crop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
};