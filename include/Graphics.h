#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <cstring>

#include "scene.h"
#include "vector3.h"
#include "Utility.h"
#include "SOIL.h"

#include "types.h"
#include "SslrInfo.h"
#include "LightsHandler.h"
#include "FPSHandler.h"
#include "TexturesInfo.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GRAPHICS_M_DEBUG_SUPER
#undef GRAPHICS_M_DEBUG_SUPER

class Graphics
{
public:
	void init(int windowWidth, int windowHeight);
	void createMap(const aiScene* scene);
	void shutdown();
	void special(int, int, int);
	void keyboard(unsigned char, int, int);
	void mouseMove(int, int);
	void reshape(GLint, GLint);
	void display();
	void sslrDisplay();
	void idle();
	void startLoop();
	~Graphics();
private:
	const aiScene* scene_;
	static const unsigned int RecTexDivValue;
	
	int windowWidth_;
	int windowHeight_;

	void createCamera();
	void setCameraLocation();
	void printCameraLocation();
	void compileShaders();
	void checkInfo();
	void createModel();
//
	bool mouseCaptured_;
	void toggleMouse();

	GL::Camera camera_;

	GLuint textures_;
	GLuint texturesCount_;
	GLuint opacityTexCount_;
	std::vector<VAOs> materials_;
	std::vector<TexturesInfo> texturesInfo_;

	unsigned int sphereMeshSize_;

	GLuint modelShader_;
	GLuint modelShaderLights_;

	VM::vec3 lightDirection_;
	bool lightsEnabled_;
	LightsHandler lights_;

	FPSHandler fps_;
	
	int lastTime_;
	bool timeCaptureEnabled_;
	int accumulatedTime_;
	int accumulatedTimeDivider_;
	void timeCaptureBegin();
	void timeCaptureEnd();

//	void drawLights();
	void drawSponza();

	std::vector<unsigned char*> rawTextures_;
	unsigned char* textureArray_;
	int textureImageWidth_;
	int textureImageHeight_;
	void loadTextures();
	void concatTextures();
	void flushTextures();
	bool loadTexture(const char*);

	std::vector<unsigned int> concatFaces(aiMesh*);
	MeshInfo loadMesh(int, unsigned int&);
//	void createLights();

	SslrInfo sslr_;
	void drawPrimaryTextures();
	void drawRecommendations();
	void drawFinalImage();

};


//openGL forbids passing pointers to functions that are class members
namespace openGLFunctions
{
	void sslrDisplay();
	void display();
	void idle();
	void keyboard(unsigned char, int, int);
	void special(int, int, int);
	void mouseMove(int, int);
	void reshape(GLint, GLint);
}
