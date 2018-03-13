#include <iostream>

#include "SceneLoader.h"

int main()
{
	try {
		SceneLoader loader;
		loader.load(SPONZA_OBJ);
		return EXIT_SUCCESS;
	}
	catch (std::invelid_argument error)
	{
		std::cout << "Exception caught by the main module" << std::endl;
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
		ctd::cout << "Unknown exception caught, aborting" << std::endl;
	}
	return EXIT_FAILURE;
}
