#include <iostream>

#include "SceneLoader.h"
#include "Graphics.h"

int main()
{
	try {
		SceneLoader loader;
		Graphics graphics;
		loader.load(SPONZA_OBJ);
		graphics.init(WINDOWWIDTH_DEFAULT, WINDOWHEIGHT_DEFAULT);
		graphics.createMap(loader.scene());
		graphics.startLoop();
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
