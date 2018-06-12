#pragma once
#include "Utility.h"

extern "C"
{
#include <jpeglib.h>
}

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
