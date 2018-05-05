#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <cstring>

#include "scene.h"
#include "vector3.h"
#include "Utility.h"
#include "SOIL.h"

extern "C"
{
#include "jpeglib.h"
}
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GRAPHICS_M_DEBUG_SUPER
#undef GRAPHICS_M_DEBUG_SUPER

//GLuint - VAO, uint - mesh vertices count
typedef std::pair<GLuint, unsigned int> MeshInfo;

typedef std::list<MeshInfo> VAOs;
//GLuint - texture index; VAOs - meshes' VAOs
//typedef std::pair<GLuint[2], VAOs> MaterialInfo;
//1 - ambient texture index, 2 - opacity texture index
struct TexturesInfo
{
	int ambientIndex;
	int opacityIndex;//Can be -1
	bool fake;//Only an ambient texture can be fake
	TexturesInfo(int, int, bool);
};

class SslrInfo
{
public:
	SslrInfo();

	void setWindowSize(int, int);

	bool flip();
	void compileShaders();
	void prepareBuffers();
//	void refreshBuffers();//after the reshape
	void prepareTextures();
	void prepareQuad();

	void readResultsDebug();

	bool enabled();
	GLuint frameBuffer();
	GLuint mrtProgram();
	GLuint drawBuffersProgram();

	GLuint quadVAO();

	GLuint colourBuffer();
	GLuint normalBuffer();
	GLuint reflectionBuffer();
	GLuint depthBuffer();


private:
	bool enabled_;
	GLuint mrtProgram_;
	GLuint drawBuffersProgram_;

	GLuint quadVAO_;
	GLuint quadVBO_;

	GLuint frameBuffer_;

	GLuint colour_;
	GLuint normal_;
	GLuint reflection_;

	GLuint depthBuffer_;
	GLuint depthRenderBuffer_;

	int windowWidth_;
	int windowHeight_;

	void setupBufferTexture(GLuint texture);
	static const GLfloat simpleQuad[];

	int pictureNumberCounter_;
};

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
	
	int windowWidth_;
	int windowHeight_;

	void createCamera();
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
//	GLuint sphereShader_;

	int lastTime_;
	unsigned int frameCounter_;

//	LightsArray lights_;
//	void updateLights();
//	void passLights(GLuint);

	void updateFPS();
	
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
