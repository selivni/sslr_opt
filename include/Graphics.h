#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <cstring>
#include <queue>

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

//GLuint - VAO, uint - mesh vertices count(not anymore I guess?)
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

struct CameraInfo
{
public:
	VM::mat4 model;
	VM::mat4 view;
	VM::mat4 projection;
	static VM::mat4 ortho(float, float, float, float, float, float);
	static VM::mat4 lookAt(const VM::vec3&, const VM::vec3&, const VM::vec3&);
	VM::mat4 getMatrix();
private:
	static float dot(const VM::vec3&, const VM::vec3&);
};


class LightsHandler
{
public:
	GLuint getShadowTexture();
	VM::mat4 getMoonMatrix();
	void addLightsInfo();
	void setLightDirection(const VM::vec3&);
	void calculateCamera();
	GLuint createLights(std::vector<VAOs>&);
private:
	CameraInfo directionalLight_;
	VM::vec3 lightDirection_;
	GLuint fbo_;
	GLuint texture_;
	GLuint textureDrawProgram_;

	unsigned int textureWidth_;
	unsigned int textureHeight_;

	void setupTexture();
	void setupFBO();
	void compileShaders();
	GLuint drawTexture(std::vector<VAOs>& materials);
};

class FPSHandler
{
public:
	void updateFPS();
	FPSHandler();
	void clear();
	bool flip();
	operator bool();
private:
	static const int timeLength_;

	std::queue<int> measurements_;
	int sum_;
	bool enabled_;
	int lastTime_;
};

class SslrInfo
{
public:
	SslrInfo();

	void setWindowSize(int, int);
	void setRecommendationsTexDivider(int);

	bool flip();
	void compileShaders();
	void prepareBuffers();
	void refreshBuffers();
	void prepareTextures();
	void prepareQuad();

	void readResultsDebug();

	bool enabled();
	GLuint mrtBuffer();
	GLuint lrBuffer();
	GLuint mrtProgram();
	GLuint mrtLightProgram();
	GLuint lrProgram();
	GLuint drawBuffersProgram();

	GLuint quadVAO();

	GLuint colourBuffer();
	GLuint normalBuffer();
	GLuint reflectionBuffer();
	GLuint depthBuffer();

	GLuint recommendationsBuffer();


private:
	bool enabled_;
	GLuint mrtProgram_;
	GLuint mrtLightProgram_;
	GLuint lrProgram_;
	GLuint drawBuffersProgram_;

	GLuint quadVAO_;
	GLuint quadVBO_;

	GLuint mrtFbo_;
	GLuint lrFbo_;

	GLuint colour_;
	GLuint normal_;
	GLuint reflection_;
	GLuint depthBuffer_;

	GLuint recommendations_;

	int windowWidth_;
	int windowHeight_;

	int recTexDiv_;

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
