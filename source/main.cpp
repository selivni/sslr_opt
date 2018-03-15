#include <iostream>

#include "SceneLoader.h"

int main()
{
	try {
		SceneLoader loader;
		loader.load(SPONZA_OBJ);
		return EXIT_SUCCESS;
	}
	catch (std::invalid_argument error)
	{
		std::cout << "Exception caught by the main module" << std::endl;
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception caught, aborting" << std::endl;
	}
	return EXIT_FAILURE;
}
