#pragma once
#include <vector>

#include "types.h"
#include "CameraInfo.h"

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
