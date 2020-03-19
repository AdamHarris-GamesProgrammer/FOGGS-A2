#pragma once
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Timer.h"

#pragma pack(push, 1)
struct BMPFileHeader {
	uint16_t file_type{ 0x4D42 }; //File type: BMP files are always 0x4D42
	uint32_t file_size{ 0 }; //Size of the file in bytes
	uint16_t reserved1{ 0 }; //Reserved, always 0
	uint16_t reserved2{ 0 }; //Reserved, always 0
	uint32_t offset_data{ 0 }; //Start Position of the pixel data, stored in bytes from start of file
};

struct BMPInfoHeader {
	uint32_t size{ 0 }; //Size of this header
	int32_t width{ 0 }; //Width of the bitmap in pixels
	int32_t height{ 0 }; //Height of the bitmap in pixels

	uint16_t planes{ 1 }; //No. of planes for the target device, always 1
	uint16_t bit_count{ 0 }; //No. bits per pixel
	uint32_t compression{ 0 }; //0 or 3 - uncompressed, this can only read uncompressed BMPs
	uint32_t size_image{ 0 }; //0 for uncompressed
	int32_t x_pixels_per_meter{ 0 }; 
	int32_t y_pixels_per_meter{ 0 }; 
	uint32_t colors_used{ 0 }; //No. of colors indexes in the color table. 
	uint32_t colors_important{ 0 }; //No. of colors used for displaying the BMP
};

struct BMPColorHeader {
	uint32_t red_mask{ 0x00ff0000 };//Red mask for the alpha channel
	uint32_t green_mask{ 0x0000ff00 };//Green mask for the alpha channel
	uint32_t blue_mask{ 0x000000ff };//Blue mask for the alpha channel
	uint32_t alpha_mask{ 0xff000000 }; //Bit mask for the alpha channel
	uint32_t color_space_type{ 0x73524742 }; //Default sRGB
	uint32_t unused[16]{ 0 }; //Unused data for sRGB color space
};
#pragma pack(pop)

struct BMP {
	BMPFileHeader file_header;
	BMPInfoHeader bmp_info_header;
	BMPColorHeader bmp_color_header;
	std::vector<uint8_t> data;
	std::vector<uint8_t> rgbData;

	BMP(const char* fname) {
		Read(fname);
	}

	void Read(const char* fname) {
		std::ifstream inp{ fname, std::ios_base::binary }; //Opens the file in binary Base 2
		if (inp) {
			inp.read((char*)&file_header, sizeof(file_header));
			if (file_header.file_type != 0x4D42) {
				throw std::runtime_error("Error! Unrecognized file format.");
			}
			inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

			//The BMPColorHeader is used only for transparent images
			if (bmp_info_header.bit_count == 32) {
				if (bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
					inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));

					//Check if the pixel data is stored as RGBA 
					CheckColorHeader(bmp_color_header);
				}
				else
				{
					std::cerr << "Warning! The file\"" << fname << "\" does not seem have the correct format" << std::endl;
					throw std::runtime_error("Error! Unrecognized file format.");
				}
			}

			//Jumps to pixel data location
			inp.seekg(file_header.offset_data, inp.beg);

			//Adjusts the header files for output as some programs add additional information that is not needed
			if (bmp_info_header.bit_count == 32) {
				bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
				file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			}
			else
			{
				bmp_info_header.size = sizeof(BMPInfoHeader);
				file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
			}
			file_header.file_size = file_header.offset_data;

			if (bmp_info_header.height < 0) {
				throw std::runtime_error("The program can only use BMP images with the origin in the bottom left corner!");
			}

			data.reserve(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8); //(8 converts from bits to bytes)
			data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8); //(8 converts from bits to bytes)

			if (bmp_info_header.width % 4 == 0) {
				inp.read((char*)data.data(), data.size());
				file_header.file_size += data.size();
			}
			else
			{
				row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
				uint32_t new_stride = MakeStrideAligned(4);
				std::vector<uint8_t> padding_row(new_stride - row_stride);

				for (int y = 0; y < bmp_info_header.height; ++y) {
					inp.read((char*)(data.data() + row_stride * y), row_stride);
					inp.read((char*)padding_row.data(), padding_row.size());
				}
				file_header.file_size += data.size() + bmp_info_header.height * padding_row.size();
			}
		}
		else
		{
			throw std::runtime_error("Unable to open the input image file.");
		}
	}

	void Write(const char* fname) {
		std::ofstream of{ fname, std::ios_base::binary };
		if (of) {
			if (bmp_info_header.bit_count == 32) {
				WriteHeaderAndData(of);
			}
			else if (bmp_info_header.bit_count == 24) {
				if (bmp_info_header.width % 4 == 0) {
					WriteHeaderAndData(of);
				}
				else {
					uint32_t new_stride = MakeStrideAligned(4);
					std::vector<uint8_t> padding_row(new_stride - row_stride);

					WriteHeaders(of);
					for (int y = 0; y < bmp_info_header.height; ++y) {
						of.write((const char*)(data.data() + row_stride * y), row_stride);
						of.write((const char*)padding_row.data(), padding_row.size());
					}
				}
			}
			else
			{
				throw std::runtime_error("The program can only treat 24 or 32 bits per pixel images");
			}
		}
		else
		{
			throw std::runtime_error("Unable to open output image file");
		}
	}

	BMP(int32_t width, int32_t height, bool has_alpha = true) {
		if (width <= 0 || height <= 0) {
			throw std::runtime_error("The image width and height must be positive numbers.");
		}

		bmp_info_header.width = width;
		bmp_info_header.height = height;
		if (has_alpha) {
			bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

			bmp_info_header.bit_count = 32;
			bmp_info_header.compression = 3;
			row_stride = width * 4; //allows each pixel to have a B G R A value
			data.reserve(row_stride * height);
			data.resize(row_stride * height);
			file_header.file_size = file_header.offset_data + data.size();
		}
		else {
			bmp_info_header.size = sizeof(BMPInfoHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

			bmp_info_header.bit_count = 24;
			bmp_info_header.compression = 0;
			row_stride = width * 3;
			data.reserve(row_stride * height);
			data.resize(row_stride * height);

			uint32_t new_stride = MakeStrideAligned(4);
			file_header.file_size = file_header.offset_data + static_cast<uint32_t>(data.size()) + bmp_info_header.height * (new_stride - row_stride);
		}
	}

	void FillRegion(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t B, uint8_t G, uint8_t R, uint8_t A) {
		if (x0 + w > (uint32_t)bmp_info_header.width || y0 + h > (uint32_t)bmp_info_header.height) {
			throw std::runtime_error("The region does not fit in the image!");
		}

		uint32_t channels = bmp_info_header.bit_count / 8;
		for (uint32_t y = y0; y < y0 + h; ++y) {
			for (uint32_t x = x0; x < x0 + w; ++x) {
				data[channels * (y * bmp_info_header.width + x) + 0] = B;
				data[channels * (y * bmp_info_header.width + x) + 1] = G;
				data[channels * (y * bmp_info_header.width + x) + 2] = R;
				if (channels == 4) {
					data[channels * (y * bmp_info_header.width + x) + 3] = A;
				}
			}
		}
	}

	void ConvertToRGB() {
		rgbData.reserve(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);
		uint32_t channels = bmp_info_header.bit_count / 8;
		for (uint32_t y = 0; y < bmp_info_header.height; ++y) {
			for (uint32_t x = 0; x < bmp_info_header.width; ++x) {
				rgbData.emplace_back(data[channels * (y * bmp_info_header.width + x) + 2]);
				rgbData.emplace_back(data[channels * (y * bmp_info_header.width + x) + 1]);
				rgbData.emplace_back(data[channels * (y * bmp_info_header.width + x) + 0]);
			}
		}
	}

private:
	uint32_t row_stride{ 0 };

	void WriteHeaders(std::ofstream& of) {
		of.write((const char*)&file_header, sizeof(file_header));
		of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
		if (bmp_info_header.bit_count == 32) {
			of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
		}
	}

	void WriteHeaderAndData(std::ofstream& of) {
		WriteHeaders(of);
		of.write((const char*)data.data(), data.size());
	}

	//Add 1 to row_stride until it is divisible with align_stride
	uint32_t MakeStrideAligned(uint32_t align_stride) {
		uint32_t new_stride = row_stride;
		while (new_stride % align_stride != 0) {
			new_stride++;
		}
		return new_stride;
	}

	//Check if the pixel data is stored in RGBA and if the color space type is sRGB
	void CheckColorHeader(BMPColorHeader& bmp_color_header) {
		BMPColorHeader expected_color_header;
		if (expected_color_header.red_mask != bmp_color_header.red_mask
			|| expected_color_header.green_mask != bmp_color_header.green_mask
			|| expected_color_header.blue_mask != bmp_color_header.blue_mask
			|| expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
			throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
		}
		if (expected_color_header.color_space_type != bmp_color_header.color_space_type) {
			throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
		}
	}


};