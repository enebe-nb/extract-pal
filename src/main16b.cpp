#include "extractpal.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Please, drag'n'drop a file on this executable instead of trying to open it!" << std::endl;
		std::cin.get();
		return 0;
	}

	std::filesystem::path infile(argv[1]);
	std::filesystem::path outfile(infile);
	outfile.replace_extension(".pal");

	if (infile == outfile) {
		std::cout << "Can't receive a palette file." << std::endl;
		std::cin.get();
		return 0;
	}

	std::ifstream input(infile, std::ios::binary);
	std::ofstream output(outfile, std::ios::binary);
	if (!extractpal(input, output, true)) {
		std::cin.get();
	}
	return 0;
}