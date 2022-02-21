#include "extractpal.hpp"
#include <png.h>

static uint16_t packColor(png_color color, bool transparent) {
	uint16_t pcolor = !transparent;
	pcolor = (pcolor << 5) + ((color.red >> 3) & 0x1F);
	pcolor = (pcolor << 5) + ((color.green >> 3) & 0x1F);
	pcolor = (pcolor << 5) + ((color.blue >> 3) & 0x1F);
	return pcolor;
}

static void pngRead(png_structp pngData, png_bytep buffer, png_size_t length) {
	std::istream* input = (std::istream*) png_get_io_ptr(pngData);
	input->read((char*)buffer, length);
}

static void pngError(png_structp pngData, png_const_charp message) {
	std::cout << message << std::endl;
	std::cin.get();
}

static void pngWarning(png_structp pngData, png_const_charp message) {
	std::cout << message << std::endl;
}

bool extractpal(std::istream& input, std::ostream& output, bool use16bits) {
	png_structp pngData = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, pngError, pngWarning);
	png_infop pngInfo = png_create_info_struct(pngData);
	png_set_read_fn(pngData, &input, pngRead);
	png_read_info(pngData, pngInfo);

	if (!(png_get_color_type(pngData, pngInfo) & PNG_COLOR_MASK_PALETTE)) {
		std::cout << "This image doesn't contain a palette." << std::endl;
		return false;
	}

	int numColors = 256;
	png_colorp palette_rgb;
	if (!png_get_PLTE(pngData, pngInfo, &palette_rgb, &numColors)) {
		std::cout << "Failure to retrieve palette." << std::endl;
		return false;
	}

	int numAlpha = 256;
	png_bytep palette_a;
	bool hasAlpha = false;
	png_color_16p unused;
	if (!use16bits && png_get_tRNS(pngData, pngInfo, &palette_a, &numAlpha, &unused)) {
		hasAlpha = true;
	}

	if (use16bits) {
		output << (uint8_t) 0x10;
		for (int i = 0; i < numColors; ++i) {
			uint16_t color = packColor(palette_rgb[i], i == 0);
			output.write((char*)&color, 2);
		}
		for (int i = numColors; i < 256; ++i) {
			const uint16_t color = 0;
			output.write((char*)&color, 2);
		}
	} else {
		output << (uint8_t) 0x20;
		for (int i = 0; i < numColors; ++i) {
			output << palette_rgb[i].blue;
			output << palette_rgb[i].green;
			output << palette_rgb[i].red;
			if (i >= numAlpha) output << (uint8_t)0xff;
			else output << palette_a[i];
		}
		for (int i = numColors; i < 256; ++i) {
			const uint32_t black = 0xff000000;
			output.write((char*)&black, 4);
		}
	}

	return true;
}