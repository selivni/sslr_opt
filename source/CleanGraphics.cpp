#include "Graphics.h"
#include "PointerGraphics.h"

TexturesInfo::TexturesInfo(int one, int two, bool three):
	ambientIndex(one), opacityIndex(two), fake(three)
	{}

SslrInfo::SslrInfo(): enabled_(false), mrtProgram_(0), drawBuffersProgram_(0),
	pictureNumberCounter_(0)
	{}

void SslrInfo::compileShaders()
{

	mrtProgram_ = GL::CompileShaderProgram("MRTSponza");
		
	drawBuffersProgram_ = GL::CompileShaderProgram("BufferSponza");
		
	
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
	glBindVertexArray(quadVAO_); 
	glGenBuffers(1, &quadVBO_); 
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO_); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(simpleQuad), simpleQuad,
		GL_STATIC_DRAW); 
	GLuint vertLocation = glGetAttribLocation(drawBuffersProgram(),
		"point"); 
	glEnableVertexAttribArray(vertLocation); 
	glVertexAttribPointer(vertLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SslrInfo::prepareBuffers()
{
	glGenFramebuffers(1, &mrtFbo_); 
	glGenFramebuffers(1, &lrFbo_);
	prepareTextures();
	glBindFramebuffer(GL_FRAMEBUFFER, mrtFbo_); 



	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, colour_, 0); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, normal_, 0); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
		GL_TEXTURE_2D, reflection_, 0); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthBuffer_, 0); 

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	glBindFramebuffer(GL_FRAMEBUFFER, lrFbo_); 

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, recommendations_, 0); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SslrInfo::setupBufferTexture(GLuint texture)
{
//	glBindTexture(GL_TEXTURE_2D, texture); 

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_NEAREST); 

//	glBindTexture(GL_TEXTURE_2D, 0);
}

void SslrInfo::prepareTextures()
{
	glGenTextures(1, &colour_);
	glGenTextures(1, &normal_);
	glGenTextures(1, &reflection_);
	glGenTextures(1, &depthBuffer_);
	glGenTextures(1, &recommendations_);
	
	glBindTexture(GL_TEXTURE_2D, colour_); 
	setupBufferTexture(colour_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, 
		GL_RGB, GL_UNSIGNED_BYTE, 0); 
	glBindTexture(GL_TEXTURE_2D, normal_); 
	setupBufferTexture(normal_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth_, windowHeight_, 0, 
		GL_RGB, GL_FLOAT, 0); 
	glBindTexture(GL_TEXTURE_2D, reflection_); 
	setupBufferTexture(reflection_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowWidth_, windowHeight_, 0,
		GL_RED, GL_FLOAT, 0); 
	glBindTexture(GL_TEXTURE_2D, depthBuffer_); 
	setupBufferTexture(depthBuffer_); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowWidth_,
		windowHeight_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); 

	glBindTexture(GL_TEXTURE_2D, recommendations_); 
	setupBufferTexture(recommendations_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, windowWidth_ / recTexDiv_,
		windowHeight_ / recTexDiv_, 0, GL_RG, GL_FLOAT, 0); 

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

GLuint SslrInfo::mrtProgram()
	{return mrtProgram_;}

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

Graphics::~Graphics()
	{}

void SslrInfo::readResultsDebug()
{
	unsigned char pixelBuffer[windowWidth_ * windowHeight_ * 3];
	glBindFramebuffer(GL_FRAMEBUFFER, mrtFbo_); 
	glReadBuffer(GL_COLOR_ATTACHMENT0); 
	glReadPixels(0, 0, windowWidth_, windowHeight_, GL_RGB, GL_UNSIGNED_BYTE,
		pixelBuffer); 
	
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

const unsigned int Graphics::RecTexDivValue = 4;
void Graphics::init(int windowWidth, int windowHeight)
{
	windowHeight_ = windowHeight;
	windowWidth_ = windowWidth;
	fpsEnabled_ = false;

	sslr_.setWindowSize(windowWidth, windowHeight);
	sslr_.setRecommendationsTexDivider(RecTexDivValue);

	GraphicalPointer = this;

	int argc = 1;
	char** argv = new char*[2];
	char name[] = PROJECT_NAME;
	argv[1] = 0;
	argv[0] = name;

	textureImageWidth_ = -1;
	textureImageHeight_ = -1;

	lastTime_ = 0;

	std::cout << "Initializing GLUT... " << std::flush;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |
						GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitContextVersion(3, 3);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(windowWidth, windowHeight);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); 
	glutCreateWindow(PROJECT_NAME);

	glutDisplayFunc(openGLFunctions::display);
	glutIdleFunc(openGLFunctions::idle);
	glutKeyboardFunc(openGLFunctions::keyboard);
	glutSpecialFunc(openGLFunctions::special);
	glutPassiveMotionFunc(openGLFunctions::mouseMove);
	glutReshapeFunc(openGLFunctions::reshape);
	mouseCaptured_ = false;
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	glClearColor(0.0, 0.0, 0.3, 0);
	std::cout << "success" << std::endl; 

	std::cout << "Initializing GLEW... " << std::flush;
	putenv((char*)"MESA_GL_VERSION_OVERRIDE=3.3COMPAT"); 
	glewExperimental = true; 
	glewInit();
	glGetError();

	std::cout << "success" << std::endl;
}

void Graphics::createMap(const aiScene* scene)
{
	scene_ = scene;

	compileShaders();
	sslr_.compileShaders();

	sslr_.prepareBuffers();

	sslr_.prepareQuad();
	
	std::cout << "Setting camera... " << std::flush;
	createCamera();
	std::cout << "success" << std::endl;

	std::cout << "Loading model... " << std::flush;
	createModel();
	std::cout << "success" << std::endl;

/*	std::cout << "Creating lights... " << std::flush;
	createLights();
	std::cout << "success" << std::endl;*/
}

void Graphics::createCamera()
{
	camera_.angle = 90.0f / 180.0f * M_PI;
	camera_.direction = VM::vec3(0, 0.3, -1);
	camera_.position = VM::vec3(0, 100, 0);
	camera_.screenRatio = static_cast<float>(windowWidth_) / windowHeight_;
	camera_.up = VM::vec3(0, 1, 0);
	camera_.zfar = 10000.0f;
	camera_.znear = 5.0f;
}

void Graphics::compileShaders()
{
	modelShader_ = GL::CompileShaderProgram("sponza");
		
}

void Graphics::checkInfo()
{
	std::cout << "Checking what's loaded:" << std::endl;
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		std::cout << i + 1 << ": ";
		for (auto iter = materials_[i].begin();
			 iter != materials_[i].end();
			 iter++)
		{
			std::cout << iter->first << ' ';
		}
		std::cout << std::endl;
		std::cout << "Textures: ambient = " << texturesInfo_[i].ambientIndex <<
			", opacity = " << texturesInfo_[i].opacityIndex << " fake = " <<
			texturesInfo_[i].fake << std::endl;
	}
}

std::vector<unsigned int> Graphics::concatFaces(aiMesh* mesh)
{
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		if (mesh->mFaces[i].mNumIndices == 3)
		{
			for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				result.push_back(mesh->mFaces[i].mIndices[j]);
		} else if (mesh->mFaces[i].mNumIndices == 4)
		{
			unsigned int* indices = mesh->mFaces[i].mIndices;
			for (unsigned int j = 0; j < 3; j++)
				result.push_back(indices[j]);
			for (unsigned int j = 0; j < 3; j++)
				result.push_back(indices[j+1]);
		} else
			throw std::invalid_argument(
				std::to_string(mesh->mFaces[i].mNumIndices) +
				" vertices in a single face! Cant deal with it, aborting");
	}
	return result;
}

MeshInfo Graphics::loadMesh(int i, unsigned int& length)
{
	MeshInfo result;
	GLuint shader;
	unsigned int material = scene_->mMeshes[i]->mMaterialIndex;

	shader = modelShader_;
	
	glGenVertexArrays(1, &result.first);
	glBindVertexArray(result.first);

	GLuint vertBuffer;
	aiVector3D* vertices = scene_->mMeshes[i]->mVertices;
	int vertLength = scene_->mMeshes[i]->mNumVertices;

	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * vertLength,
				 vertices, GL_STATIC_DRAW); 
	GLuint vertLocation = glGetAttribLocation(shader, "point"); 
	glEnableVertexAttribArray(vertLocation); 
	glVertexAttribPointer(vertLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 

	GLuint uvBuffer;
	aiVector3D* uv = scene_->mMeshes[i]->mTextureCoords[0];

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * vertLength,
				 uv, GL_STATIC_DRAW); 
	GLuint uvLocation = glGetAttribLocation(shader, "uvCoord"); 
	glEnableVertexAttribArray(uvLocation); 
	glVertexAttribPointer(uvLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 

	GLuint normBuffer;
	aiVector3D* norm = scene_->mMeshes[i]->mNormals;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &normBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * vertLength,
				 norm, GL_STATIC_DRAW); 
	GLuint normLocation = glGetAttribLocation(shader, "normal");
		
	glEnableVertexAttribArray(normLocation); 
	glVertexAttribPointer(normLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		

	GLuint facesBuffer;
	std::vector<unsigned int> faces = concatFaces(scene_->mMeshes[i]);
	length = faces.size();

	glGenBuffers(1, &facesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(),
				 faces.data(), GL_STATIC_DRAW); 

	glBindVertexArray(0);
	
	result.second = material;
	return result;
}

bool Graphics::loadTexture(const char* path)
{
	int width, height;
	int channels;
	unsigned char* image = SOIL_load_image(path,
		&width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == NULL)
	{
		std::cerr << "Failed to load texture " << path << std::endl;
		return false;
	}
	if (textureImageWidth_ == -1 && textureImageHeight_ == -1)
	{
		textureImageWidth_ = width;
		textureImageHeight_ = height;
	} else if (textureImageWidth_ != width || textureImageHeight_ != height)
		throw std::invalid_argument(std::string("Error: texture ") + path + 
			std::string("is of a different size than others! This is "
						"unacceptable!"));
	rawTextures_.push_back(image);

	#ifdef GRAPHICS_M_DEBUG_SUPER
	std::cout << "Loaded texture " << path << std::endl;
	#endif

	return true;
}

void Graphics::concatTextures()
{
	unsigned int imageSize = textureImageWidth_ * textureImageHeight_ * 4;
	if (textureImageWidth_ == -1 || textureImageHeight_ == -1)
		return;
	textureArray_ = new unsigned char[(texturesCount_ + opacityTexCount_) *
		imageSize];
	for (unsigned int i = 0; i < texturesCount_ + opacityTexCount_; i++)
	{
		std::memcpy(textureArray_ + i * imageSize, rawTextures_[i], imageSize *
			sizeof(unsigned char));
		SOIL_free_image_data(rawTextures_[i]);
	}
}

void Graphics::flushTextures()
{
	glGenTextures(1, &textures_); 
	glBindTexture(GL_TEXTURE_2D_ARRAY, textures_); 
	glTexParameteri(GL_TEXTURE_2D_ARRAY,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
//	glTexParameteri(GL_TEXTURE_2D_ARRAY,
//		GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_RGBA8, textureImageWidth_,
		textureImageHeight_, texturesCount_ + opacityTexCount_); 

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, textureImageWidth_,
		textureImageHeight_, texturesCount_ + opacityTexCount_, GL_RGBA,
		GL_UNSIGNED_BYTE, textureArray_); 

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY); 

//	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Graphics::loadTextures()
{
	aiString path;
	aiReturn error;
	texturesCount_ = 0;
	opacityTexCount_ = 0;
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		if ((error = scene_->mMaterials[i]->GetTexture(
			 aiTextureType_AMBIENT, 0, &path)) == aiReturn_SUCCESS)
		{
			char* data = path.data;
			for (unsigned int j = 0; data[j] != '\0'; j++)
				if (data[j] == '\\')
					data[j] = '/';
			if (!loadTexture((std::string(MODEL_PATH) +
				"/" + path.C_Str()).data()))
				throw std::invalid_argument(std::string("Error: failed to"
					" load") + path.C_Str());
			else
				texturesCount_++;
		} else if (!loadTexture((std::string(MODEL_PATH) +
				"/textures/sponza_no_tex.tga").data()))
			throw std::invalid_argument("Error: failed to load no_texture texture");
		else
		{
			texturesInfo_[i].fake = true;
			texturesCount_++;
		}
		texturesInfo_[i].ambientIndex = texturesCount_ - 1;
	}

	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		if ((error = scene_->mMaterials[i]->GetTexture(
			 aiTextureType_OPACITY, 0, &path)) == aiReturn_SUCCESS)
		{
			char* data = path.data;
			for (unsigned int j = 0; data[j] != '\0'; j++)
				if (data[j] == '\\')
					data[j] = '/';
			if (!loadTexture((std::string(MODEL_PATH) +
				"/" + path.C_Str()).data()))
				throw std::invalid_argument(std::string("Error: failed to"
					" load") + path.C_Str());
			else
				opacityTexCount_++;
			texturesInfo_[i].opacityIndex =
				texturesCount_ + opacityTexCount_ - 1;
		}
	}

	concatTextures();
	flushTextures();

	std::cout << "Loaded " << texturesCount_ << '/'
			  << scene_->mNumMaterials << " possible textures" << std::endl;
	if (opacityTexCount_ != 0)
		std::cout << "Also loaded " << opacityTexCount_
				  << " opacity textures" << std::endl;
}

void Graphics::createModel()
{
	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		VAOs meshes;
		TexturesInfo material(-1, -1, false);
		materials_.push_back(meshes);
		texturesInfo_.push_back(material);
	}

	loadTextures();

	for (unsigned int i = 0; i < scene_->mNumMeshes; i++)
	{
		unsigned int length;
		MeshInfo meshInfo = loadMesh(i, length);
		unsigned int index = meshInfo.second;
		meshInfo.second = length;
		materials_[index].push_back(meshInfo);
	}
	#ifdef GRAPHICS_M_DEBUG_SUPER
	checkInfo();
	#endif
}

void Graphics::updateFPS()
{
	if (fpsEnabled_)
	{
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		std::cout << '\r'
			<< "FPS: " << 1000 / (currentTime - lastTime_) << "      ";
		lastTime_ = currentTime;
	}
}

void Graphics::drawSponza()
{
	updateFPS();
	GLint cameraLocation =
		glGetUniformLocation(modelShader_, "camera"); 
	GLint materialIndexLocation =
		glGetUniformLocation(modelShader_, "material"); 
	GLint cameraPosLoc =
		glGetUniformLocation(modelShader_, "cameraPosition");
			
	GLint textureLocation =
		glGetUniformLocation(modelShader_, "textureArray"); 

	glUseProgram(modelShader_); 
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); 
	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);

	glUniform1i(textureLocation, textures_);
//	passLights(modelShader_);

	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		VAOs meshes = materials_[i];
		GLuint materialIndex = i;
		glUniform1ui(materialIndexLocation, materialIndex);
		for (auto iter = meshes.begin();
			 iter != meshes.end();
			 iter++)
		{
			glBindVertexArray(iter->first); 
			glDrawElements(GL_TRIANGLES, iter->second, GL_UNSIGNED_INT, 0);
				
			glBindVertexArray(0); 
		}
	}
	glUseProgram(0);
}

void Graphics::drawPrimaryTextures()
{
	glBindFramebuffer(GL_FRAMEBUFFER, sslr_.mrtBuffer()); 
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, buffers); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glViewport(0, 0, windowWidth_, windowHeight_);

	updateFPS();
	GLint cameraLocation =
		glGetUniformLocation(sslr_.mrtProgram(), "camera"); 
	GLint materialIndexLocation =
		glGetUniformLocation(sslr_.mrtProgram(), "material"); 
	GLint cameraPosLoc =
		glGetUniformLocation(sslr_.mrtProgram(), "cameraPosition");
			
	GLint textureLocation =
		glGetUniformLocation(sslr_.mrtProgram(), "textureArray"); 

	glUseProgram(sslr_.mrtProgram()); 
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); 
	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);

	glUniform1i(textureLocation, textures_);

	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		VAOs meshes = materials_[i];
		GLuint materialIndex = i;
		glUniform1ui(materialIndexLocation, materialIndex);
		for (auto iter = meshes.begin();
			 iter != meshes.end();
			 iter++)
		{
			glBindVertexArray(iter->first); 
			glDrawElements(GL_TRIANGLES, iter->second, GL_UNSIGNED_INT, 0);
				
			glBindVertexArray(0); 
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Graphics::drawRecommendations()
{
	glBindFramebuffer(GL_FRAMEBUFFER, sslr_.lrBuffer());
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, buffers); 
	glViewport(0, 0, windowWidth_ / RecTexDivValue, windowHeight_ / RecTexDivValue); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glUseProgram(sslr_.lrProgram()); 
	
	GLint cameraLocation =
		glGetUniformLocation(sslr_.lrProgram(), "camera"); 

	GLint modelLocation =
		glGetUniformLocation(sslr_.lrProgram(), "model"); 

	GLint viewLocation =
		glGetUniformLocation(sslr_.lrProgram(), "view"); 

	GLint projectionLocation =
		glGetUniformLocation(sslr_.lrProgram(), "projection"); 

	GLint cameraPosLoc =
		glGetUniformLocation(sslr_.lrProgram(), "cameraPosition");
			

	GLint colourLocation = glGetUniformLocation(
		sslr_.lrProgram(), "colour"); 
	GLint normalLocation = glGetUniformLocation(
		sslr_.lrProgram(), "normal"); 
	GLint reflectionLocation = glGetUniformLocation(
		sslr_.lrProgram(), "reflection"); 
	GLint depthLocation = glGetUniformLocation(
		sslr_.lrProgram(), "depth"); 

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); 
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE,
		camera_.getModel().data().data()); 
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE,
		camera_.getView().data().data()); 
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE,
		camera_.getProjection().data().data()); 

	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);

	glUniform1i(colourLocation, 0); 
	glUniform1i(normalLocation, 1); 
	glUniform1i(reflectionLocation, 2); 
	glUniform1i(depthLocation, 3); 

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, sslr_.colourBuffer()); 
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, sslr_.normalBuffer()); 
	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, sslr_.reflectionBuffer()); 
	glActiveTexture(GL_TEXTURE3); 
	glBindTexture(GL_TEXTURE_2D, sslr_.depthBuffer()); 

	glBindVertexArray(sslr_.quadVAO()); 
	glDrawArrays(GL_TRIANGLES, 0, 6); 

	glViewport(0, 0, windowWidth_, windowHeight_); 
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Graphics::drawFinalImage()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glUseProgram(sslr_.drawBuffersProgram()); 
	
	GLint cameraLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "camera"); 

	GLint modelLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "model"); 

	GLint viewLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "view"); 

	GLint projectionLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "projection"); 

	GLint cameraPosLoc =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "cameraPosition");
			

	GLint windowSizeLoc = glGetUniformLocation(sslr_.drawBuffersProgram(),
		"windowSize"); 

	GLint colourLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "colour"); 
	GLint normalLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "normal"); 
	GLint reflectionLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "reflection"); 
	GLint depthLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "depth"); 
	GLint recommendationsLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "recommendations"); 

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); 
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE,
		camera_.getModel().data().data()); 
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE,
		camera_.getView().data().data()); 
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE,
		camera_.getProjection().data().data()); 

	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);
	GLfloat winSize[2];
	winSize[0] = static_cast<float>(windowWidth_);
	winSize[1] = static_cast<float>(windowHeight_);
	glUniform2fv(windowSizeLoc, 1, winSize);

	glUniform1i(colourLocation, 0); 
	glUniform1i(normalLocation, 1); 
	glUniform1i(reflectionLocation, 2); 
	glUniform1i(depthLocation, 3); 
	glUniform1i(recommendationsLocation, 4); 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sslr_.colourBuffer());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sslr_.normalBuffer());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sslr_.reflectionBuffer());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, sslr_.depthBuffer());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, sslr_.recommendationsBuffer());
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

	glBindVertexArray(sslr_.quadVAO()); 
	glDrawArrays(GL_TRIANGLES, 0, 6); 

	glBindVertexArray(0);
	glUseProgram(0);
/*
	GLint one;
	glGetIntegerv(0x9145, &one);
	std::cout << (int) one << std::endl;
*/
}

void openGLFunctions::sslrDisplay()
{
	GraphicalPointer->sslrDisplay();
}

void Graphics::sslrDisplay()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	drawPrimaryTextures();

	drawRecommendations();
#ifdef GRAPHICS_M_DEBUG_SUPER
	sslr_.readResultsDebug();
#endif

	drawFinalImage();

	glutSwapBuffers();
}

void openGLFunctions::display()
{
	GraphicalPointer->display();
}

void Graphics::display()
{
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

//	drawLights();
	drawSponza();
//	updateLights();

	glutSwapBuffers(); 
}

void openGLFunctions::idle()
{
	GraphicalPointer->idle();
}

void Graphics::idle()
{
	glutPostRedisplay();
}

void openGLFunctions::keyboard(unsigned char key, int x, int y)
{
	GraphicalPointer->keyboard(key, x, y);
}

void Graphics::shutdown()
{
	glutDestroyWindow(glutGetWindow());
	std::cout << std::endl;
}

void Graphics::startLoop()
{
	glutMainLoop();
}

void Graphics::toggleMouse()
{
	mouseCaptured_ = !mouseCaptured_;
	if (mouseCaptured_)
	{
		glutWarpPointer(windowWidth_ / 2, windowHeight_ / 2);
		glutSetCursor(GLUT_CURSOR_NONE);
	} else
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
}

void Graphics::keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 27)
		shutdown();
	else if (key == 'w' || key == 'W')
	{
		camera_.goForward(25.0);
//		std::cout << "VM::vec3(" << camera_.position.x << ", "
//				  << camera_.position.y << ", "
//				  << camera_.position.z << ")," << std::endl;
//		std::cout << "\rCurrent camera position: " << camera_.position.x << ' '
//				  << camera_.position.y << ' '
//				  << camera_.position.z << std::endl;
	}
	else if (key == 's' || key == 'S')
	{
		camera_.goBack(25.0);
//		std::cout << "\rCurrent camera position: " << camera_.position.x << ' '
//				  << camera_.position.y << ' '
//				  << camera_.position.z << std::endl;
	}
	else if (key == 'm' || key == 'M')
		toggleMouse();
	else if (key == (char)23)
		camera_.goForward(1.0);
	else if (key == (char)19)
		camera_.goBack(1.0);
	else if (key == 'O')
	{
		if (sslr_.flip())
			glutDisplayFunc(openGLFunctions::sslrDisplay);
		else
			glutDisplayFunc(openGLFunctions::display);
	}
	else if (key == 'f' || key == 'F')
		fpsEnabled_ = !fpsEnabled_;
	else if (key == 'p' || key == 'P')
	{
		std::cout << "\rCurrent camera position: " << camera_.position.x << ' '
				  << camera_.position.y << ' '
				  << camera_.position.z << std::endl;
	}
}

void openGLFunctions::special(int key, int x, int y)
{
	GraphicalPointer->special(key, x, y);
}

void Graphics::special(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		camera_.rotateY(0.02);
	else if (key == GLUT_KEY_LEFT)
		camera_.rotateY(-0.02);
	else if (key == GLUT_KEY_UP)
		camera_.rotateTop(-0.02);
	else if (key == GLUT_KEY_DOWN)
		camera_.rotateTop(0.02);
}

void openGLFunctions::mouseMove(int x, int y)
{
	GraphicalPointer->mouseMove(x, y);
}

void Graphics::mouseMove(int x, int y)
{
	if (mouseCaptured_)
	{
		int centerX = windowWidth_ / 2;
		int centerY = windowHeight_ / 2;
		if (x != centerX || y != centerY)
		{
			camera_.rotateY((x - centerX) / 1000.0f);
			camera_.rotateTop((y - centerY) / 1000.0f);
			glutWarpPointer(centerX, centerY);
		}
	}
}

void openGLFunctions::reshape(GLint newWidth, GLint newHeight)
{
	GraphicalPointer->reshape(newWidth, newHeight);
}

void Graphics::reshape(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	windowWidth_ = newWidth;
	windowHeight_ = newHeight;

	std::cout << "Window reshaped to " << windowWidth_ << 'x' <<
		windowHeight_ << std::endl;

	sslr_.setWindowSize(newWidth, newHeight);
	sslr_.refreshBuffers();

	camera_.screenRatio = static_cast<float>(windowWidth_) / windowHeight_;
}
