#include "LightsHandler.h"

GLuint LightsHandler::getShadowTexture()
{
	return texture_;
}

VM::mat4 LightsHandler::getMoonMatrix()
{
	return directionalLight_.getMatrix();
}

void LightsHandler::setLightDirection(const VM::vec3& lightDirection)
{
	lightDirection_ = lightDirection;
}

void LightsHandler::calculateCamera()
{
	GL::Camera camera;

	camera.direction = -lightDirection_;
	camera.position = -lightDirection_ * 1500;
	camera.up = VM::vec3(0, 1, 0);

	directionalLight_.model = camera.getModel();
	directionalLight_.view = camera.getView();
//	directionalLight_.view = CameraInfo::lookAt(-lightDirection_, 
//		VM::vec3(0.0, 0.0, 0.0), VM::vec3(0.0, 1.0, 0.0));
//	directionalLight_.projection = CameraInfo::ortho(
//		-1.0, 1.0, -1.0, 1.0, -1.0, 2.0);
//	directionalLight_.projection = CameraInfo::ortho(
//		-10.0, 10.0, -10.0, 10.0, -10.0, 20.0);
//	directionalLight_.projection = CameraInfo::ortho(
//		-2186.0, 2078.0, -221, 1759, -1395, 1370);
	directionalLight_.projection = CameraInfo::ortho(
		-1500.0, 1500.0, -1500, 1500, 1.0, 3000.0);
}

void LightsHandler::setupTexture()
{
	GLint maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	if (maxSize >= 8192)
		textureWidth_ = textureHeight_ = 8192;
	else
		textureWidth_ = textureHeight_ = maxSize;
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, textureWidth_,
		textureHeight_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); CHECK_GL_ERRORS

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECK_GL_ERRORS
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LightsHandler::setupFBO()
{
	glGenFramebuffers(1, &fbo_); CHECK_GL_ERRORS
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_); CHECK_GL_ERRORS

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, texture_, 0); CHECK_GL_ERRORS

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
		std::cout << "Framebuffer incomplete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0); CHECK_GL_ERRORS
}

void LightsHandler::compileShaders()
{
	textureDrawProgram_ = GL::CompileShaderProgram("LightView");
		CHECK_GL_ERRORS
}

GLuint LightsHandler::drawTexture(std::vector<VAOs>& materials_)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_); CHECK_GL_ERRORS
	glDrawBuffer(GL_NONE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERRORS
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, textureWidth_, textureHeight_);

	GLint cameraLocation =
		glGetUniformLocation(textureDrawProgram_, "camera"); CHECK_GL_ERRORS

	glUseProgram(textureDrawProgram_); CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		directionalLight_.getMatrix().data().data()); CHECK_GL_ERRORS

	for (unsigned int i = 0; i < materials_.size(); i++)
	{
		VAOs meshes = materials_[i];
		for (auto iter = meshes.begin();
			 iter != meshes.end();
			 iter++)
		{
			glBindVertexArray(iter->first); CHECK_GL_ERRORS
			glDrawElements(GL_TRIANGLES, iter->second, GL_UNSIGNED_INT, 0);
				CHECK_GL_ERRORS
			glBindVertexArray(0); CHECK_GL_ERRORS
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	return texture_;
}

GLuint LightsHandler::createLights(std::vector<VAOs>& materials_)
{
	calculateCamera();
	compileShaders();
	setupTexture();
	setupFBO();
	GLuint result = drawTexture(materials_);
	return result;
}
