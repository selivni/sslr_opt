#include "SslrInfo.h"

SslrInfo::SslrInfo(): enabled_(false), drawBuffersProgram_(0),
	pictureNumberCounter_(0)
	{}


void SslrInfo::compileShaders()
{
	drawBuffersProgram_ = GL::CompileShaderProgram("BufferSponza");
		CHECK_GL_ERRORS
	
	lrProgram_ = GL::CompileShaderProgram("LRSponza");
}

void SslrInfo::setWindowSize(int width, int height)
{
	windowWidth_ = width;
	windowHeight_ = height;
}

void SslrInfo::setRecommendationsTexDivider(int value)
{
	recTexDiv_ = value;
}

const GLfloat SslrInfo::simpleQuad[]=
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

void SslrInfo::prepareQuad()
{
	glGenVertexArrays(1, &quadVAO_);
	glBindVertexArray(quadVAO_); CHECK_GL_ERRORS
	glGenBuffers(1, &quadVBO_); CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO_); CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(simpleQuad), simpleQuad,
		GL_STATIC_DRAW); CHECK_GL_ERRORS
	GLuint vertLocation = glGetAttribLocation(drawBuffersProgram(),
		"point"); CHECK_GL_ERRORS
	glEnableVertexAttribArray(vertLocation); CHECK_GL_ERRORS
	glVertexAttribPointer(vertLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		CHECK_GL_ERRORS
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SslrInfo::prepareBuffers()
{
	glGenFramebuffers(1, &mrtFbo_); CHECK_GL_ERRORS
	glGenFramebuffers(1, &lrFbo_);
	prepareTextures();
	glBindFramebuffer(GL_FRAMEBUFFER, mrtFbo_); CHECK_GL_ERRORS



	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colour_, 0); CHECK_GL_ERRORS
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, normal_, 0); CHECK_GL_ERRORS
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
		GL_TEXTURE_2D, reflection_, 0); CHECK_GL_ERRORS
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthBuffer_, 0); CHECK_GL_ERRORS

	glBindFramebuffer(GL_FRAMEBUFFER, 0); CHECK_GL_ERRORS

	glBindFramebuffer(GL_FRAMEBUFFER, lrFbo_); CHECK_GL_ERRORS

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, recommendations_, 0); CHECK_GL_ERRORS
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SslrInfo::setupBufferTexture(GLuint texture)
{
//	glBindTexture(GL_TEXTURE_2D, texture); CHECK_GL_ERRORS

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECK_GL_ERRORS

//	glBindTexture(GL_TEXTURE_2D, 0);
}

void SslrInfo::prepareTextures()
{
	glGenTextures(1, &colour_);
	glGenTextures(1, &normal_);
	glGenTextures(1, &reflection_);
	glGenTextures(1, &depthBuffer_);
	glGenTextures(1, &recommendations_);
	
	glBindTexture(GL_TEXTURE_2D, colour_); CHECK_GL_ERRORS
	setupBufferTexture(colour_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, 
		GL_RGB, GL_UNSIGNED_BYTE, 0); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, normal_); CHECK_GL_ERRORS
	setupBufferTexture(normal_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, 
		GL_RGB, GL_FLOAT, 0); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, reflection_); CHECK_GL_ERRORS
	setupBufferTexture(reflection_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowWidth_, windowHeight_, 0,
		GL_RED, GL_FLOAT, 0); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, depthBuffer_); CHECK_GL_ERRORS
	setupBufferTexture(depthBuffer_); CHECK_GL_ERRORS
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowWidth_,
		windowHeight_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); CHECK_GL_ERRORS

	glBindTexture(GL_TEXTURE_2D, recommendations_); CHECK_GL_ERRORS
	setupBufferTexture(recommendations_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, windowWidth_ / recTexDiv_,
		windowHeight_ / recTexDiv_, 0, GL_RG, GL_FLOAT, 0); CHECK_GL_ERRORS

	glBindTexture(GL_TEXTURE_2D, 0);
}

void SslrInfo::refreshBuffers()
{
	glDeleteTextures(1, &colour_);
	glDeleteTextures(1, &normal_);
	glDeleteTextures(1, &reflection_);
	glDeleteTextures(1, &depthBuffer_);
	glDeleteTextures(1, &recommendations_);

	glDeleteFramebuffers(1, &mrtFbo_);
	glDeleteFramebuffers(1, &lrFbo_);

	prepareBuffers();
}

bool SslrInfo::flip()
{
	return enabled_ = !enabled_;
}

GLuint SslrInfo::mrtBuffer()
	{return mrtFbo_;}

GLuint SslrInfo::lrBuffer()
	{return lrFbo_;}

GLuint SslrInfo::lrProgram()
	{return lrProgram_;}

GLuint SslrInfo::drawBuffersProgram()
	{return drawBuffersProgram_;}

GLuint SslrInfo::colourBuffer()
	{return colour_;}

GLuint SslrInfo::normalBuffer()
	{return normal_;}

GLuint SslrInfo::reflectionBuffer()
	{return reflection_;}

GLuint SslrInfo::depthBuffer()
	{return depthBuffer_;}

GLuint SslrInfo::recommendationsBuffer()
	{return recommendations_;}

GLuint SslrInfo::quadVAO()
	{return quadVAO_;}

void SslrInfo::readResultsDebug()
{
	unsigned char pixelBuffer[windowWidth_ * windowHeight_ * 3];
	glBindFramebuffer(GL_FRAMEBUFFER, mrtFbo_); CHECK_GL_ERRORS
	glReadBuffer(GL_COLOR_ATTACHMENT0); CHECK_GL_ERRORS
	glReadPixels(0, 0, windowWidth_, windowHeight_, GL_RGB, GL_UNSIGNED_BYTE,
		pixelBuffer); CHECK_GL_ERRORS
	
	struct jpeg_compress_struct compress;
	struct jpeg_error_mgr manager;

	compress.err = jpeg_std_error(&manager);
	jpeg_create_compress(&compress);
	
	char number[256];
	sprintf(number, "%d", pictureNumberCounter_);
	FILE* descriptor = fopen((std::string("Buffer_snapshot_") +
		number).data(), "w");
	jpeg_stdio_dest(&compress, descriptor);
	compress.image_width = windowWidth_;
	compress.image_height = windowHeight_;
	compress.input_components = 3;
	compress.in_color_space = JCS_RGB;

	jpeg_set_defaults(&compress);
	jpeg_set_quality(&compress, 90, true);
	jpeg_start_compress(&compress, TRUE);

	JSAMPROW row_pointer[1];
	int row_stride = compress.image_width * 3;

	while (compress.next_scanline < compress.image_height)
	{
		row_pointer[0] = (JSAMPLE*) (pixelBuffer + compress.next_scanline *
			row_stride);
		jpeg_write_scanlines(&compress, row_pointer, 1);
	}
	jpeg_finish_compress(&compress);
	jpeg_destroy_compress(&compress);

	pictureNumberCounter_++;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
