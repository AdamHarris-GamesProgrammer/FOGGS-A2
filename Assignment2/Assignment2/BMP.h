#pragma once
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Timer.h"

#pragma pack(push, 1)
struct BMPFileHeader {
	uint16_t fileType{ 0x4D42 }; //File type: BMP files are always 0x4D42
	uint32_t fileSize{ 0 }; //Size of the file in bytes
	uint16_t reserved1{ 0 }; //Reserved, always 0
	uint16_t reserved2{ 0 }; //Reserved, always 0
	uint32_t offsetData{ 0 }; //Start Position of the pixel data, stored in bytes from start of file
};

struct BMPInfoHeader {
	uint32_t size{ 0 }; //Size of this header
	int32_t width{ 0 }; //Width of the bitmap in pixels
	int32_t height{ 0 }; //Height of the bitmap in pixels

	uint16_t planes{ 1 }; //No. of planes for the target device, always 1
	uint16_t bitCount{ 0 }; //No. bits per pixel
	uint32_t compression{ 0 }; //0 or 3 - uncompressed, this can only read uncompressed BMPs
};

struct BMPColourHeader {
	uint32_t redMask{ 0x00ff0000 };//Red mask for the alpha channel
	uint32_t greenMask{ 0x0000ff00 };//Green mask for the alpha channel
	uint32_t blueMask{ 0x000000ff };//Blue mask for the alpha channel
	uint32_t alphaMask{ 0xff000000 }; //Bit mask for the alpha channel
	uint32_t colourSpaceType{ 0x73524742 }; //Default sRGB
};
#pragma pack(pop)

struct BMP {
	BMPFileHeader fileHeader;
	BMPInfoHeader bmpInfoHeader;
	BMPColourHeader bmpColourHeader;
	std::vector<uint8_t> data;
	std::vector<uint8_t> rgbData;

	int requiredSize;

	BMP(const char* fname) {
		Read(fname);
	}

	void ConvertToRGB() {
		rgbData.reserve(requiredSize);
		uint32_t channels = bmpInfoHeader.bitCount / 8;
		for (uint32_t y = 0; y < bmpInfoHeader.height; ++y) {
			for (uint32_t x = 0; x < bmpInfoHeader.width; ++x) {
				int index = channels * (y * bmpInfoHeader.width + x);
				rgbData.emplace_back(data[index + 2]); //R
				rgbData.emplace_back(data[index + 1]); //G
				rgbData.emplace_back(data[index + 0]); //B
			}
		}
	}

private:
	uint32_t rowStride{ 0 };

	void Read(const char* fname) {
		std::ifstream inp{ fname, std::ios_base::binary }; //Opens the file in binary Base 2
		if (inp) {
			inp.read((char*)&fileHeader, sizeof(fileHeader));
			if (fileHeader.fileType != 0x4D42) {
				throw std::runtime_error("Error! Unrecognized file format.");
			}
			inp.read((char*)&bmpInfoHeader, sizeof(bmpInfoHeader));

			//The BMPColorHeader is used only for transparent images
			if (bmpInfoHeader.bitCount == 32) {
				if (bmpInfoHeader.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColourHeader))) {
					inp.read((char*)&bmpColourHeader, sizeof(bmpColourHeader));

					//Check if the pixel data is stored as RGBA 
					CheckColorHeader(bmpColourHeader);
				}
				else
				{
					std::cerr << "Warning! The file\"" << fname << "\" does not seem have the correct format" << std::endl;
					throw std::runtime_error("Error! Unrecognized file format.");
				}
			}

			//Jumps to pixel data location
			inp.seekg(fileHeader.offsetData, inp.beg);

			//Adjusts the header files for output as some programs add additional information that is not needed
			if (bmpInfoHeader.bitCount == 32) {
				bmpInfoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColourHeader);
				fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColourHeader);
			}
			else
			{
				bmpInfoHeader.size = sizeof(BMPInfoHeader);
				fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
			}
			fileHeader.fileSize = fileHeader.offsetData;

			if (bmpInfoHeader.height < 0) {
				throw std::runtime_error("The program can only use BMP images with the origin in the bottom left corner!");
			}

			requiredSize = bmpInfoHeader.width * bmpInfoHeader.height * bmpInfoHeader.bitCount / 8;

			data.reserve(requiredSize); //(8 converts from bits to bytes)
			data.resize(requiredSize); //Initializes all values

			if (bmpInfoHeader.width % 4 == 0) {
				inp.read((char*)data.data(), data.size());
				fileHeader.fileSize += data.size();
			}
			else
			{
				rowStride = bmpInfoHeader.width * bmpInfoHeader.bitCount / 8;
				uint32_t newStride = MakeStrideAligned(4);
				std::vector<uint8_t> padding_row(newStride - rowStride);

				for (int y = 0; y < bmpInfoHeader.height; ++y) {
					inp.read((char*)(data.data() + rowStride * y), rowStride);
					inp.read((char*)padding_row.data(), padding_row.size());
				}
				fileHeader.fileSize += data.size() + bmpInfoHeader.height * padding_row.size();
			}
		}
		else
		{
			throw std::runtime_error("Unable to open the input image file.");
		}
	}

	//Add 1 to row_stride until it is divisible with align_stride
	uint32_t MakeStrideAligned(uint32_t alignStride) {
		uint32_t newStride = rowStride;
		while (newStride % alignStride != 0) {
			newStride++;
		}
		return newStride;
	}

	//Check if the pixel data is stored in RGBA and if the color space type is sRGB
	void CheckColorHeader(BMPColourHeader& bmp_color_header) {

		if (   bmp_color_header.redMask		!= 0x00ff0000 //Red
			|| bmp_color_header.greenMask	!= 0x0000ff00 //Green
			|| bmp_color_header.blueMask	!= 0x000000ff //Blue
			|| bmp_color_header.alphaMask	!= 0xff000000 //Alpha
			) {
			throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
		}
		if (   bmp_color_header.colourSpaceType != 0x73524742) { //0x73524742 = default sRGB
			throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
		}
	}


};