#include "Graphics.h"
#include "PointerGraphics.h"



const unsigned int Graphics::RecTexDivValue = 4;
void Graphics::init(int windowWidth, int windowHeight)
{
	windowHeight_ = windowHeight;
	windowWidth_ = windowWidth;
	timeCaptureEnabled_ = false;
	lightsEnabled_ = false;
	lightDirection_ = VM::vec3(-0.3, -1.0, -0.1);

	sslr_.setWindowSize(windowWidth, windowHeight);
	sslr_.setRecommendationsTexDivider(RecTexDivValue);
	lights_.setLightDirection(normalize(lightDirection_));

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
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); CHECK_GL_ERRORS
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
	std::cout << "success" << std::endl; CHECK_GL_ERRORS

	std::cout << "Initializing GLEW... " << std::flush;
	putenv((char*)"MESA_GL_VERSION_OVERRIDE=3.3COMPAT"); CHECK_GL_ERRORS
	glewExperimental = true; CHECK_GL_ERRORS
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

	std::cout << "Calculating lights... " << std::flush;
	lights_.createLights(materials_);
	std::cout << "success" << std::endl;

/*	std::cout << "Creating lights... " << std::flush;
	createLights();
	std::cout << "success" << std::endl;*/
}

void Graphics::printCameraLocation()
{
	std::cout << "Camera direction: " << camera_.direction.x << ' ' <<
		camera_.direction.y << ' ' << camera_.direction.z << std::endl <<
		"Camera position: " << camera_.position.x << ' ' <<
		camera_.position.y << ' ' << camera_.position.z << std::endl;
}

void Graphics::setCameraLocation()
{
	camera_.direction = VM::vec3(-0.626697, 0.148901, 0.764905);
	camera_.position = VM::vec3(-962.632, 163.074, 137.32);
//	camera_.direction = VM::vec3(1, 0, 0);
//	camera_.position = VM::vec3(-16, 100, -32);
	camera_.up = VM::vec3(0, 1, 0);
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
		CHECK_GL_ERRORS
	modelShaderLights_ = GL::CompileShaderProgram("sponzaWithLights");
		CHECK_GL_ERRORS
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
				 vertices, GL_STATIC_DRAW); CHECK_GL_ERRORS
	GLuint vertLocation = glGetAttribLocation(shader, "point"); CHECK_GL_ERRORS
	glEnableVertexAttribArray(vertLocation); CHECK_GL_ERRORS
	glVertexAttribPointer(vertLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERRORS

	GLuint uvBuffer;
	aiVector3D* uv = scene_->mMeshes[i]->mTextureCoords[0];

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * vertLength,
				 uv, GL_STATIC_DRAW); CHECK_GL_ERRORS
	GLuint uvLocation = glGetAttribLocation(shader, "uvCoord"); CHECK_GL_ERRORS
	glEnableVertexAttribArray(uvLocation); CHECK_GL_ERRORS
	glVertexAttribPointer(uvLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERRORS

	GLuint normBuffer;
	aiVector3D* norm = scene_->mMeshes[i]->mNormals;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &normBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * vertLength,
				 norm, GL_STATIC_DRAW); CHECK_GL_ERRORS
	GLuint normLocation = glGetAttribLocation(shader, "normal");
		CHECK_GL_ERRORS
	glEnableVertexAttribArray(normLocation); CHECK_GL_ERRORS
	glVertexAttribPointer(normLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		CHECK_GL_ERRORS

	GLuint facesBuffer;
	std::vector<unsigned int> faces = concatFaces(scene_->mMeshes[i]);
	length = faces.size();

	glGenBuffers(1, &facesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(),
				 faces.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

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
	glGenTextures(1, &textures_); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D_ARRAY, textures_); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D_ARRAY,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS
//	glTexParameteri(GL_TEXTURE_2D_ARRAY,
//		GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_RGBA8, textureImageWidth_,
		textureImageHeight_, texturesCount_ + opacityTexCount_); CHECK_GL_ERRORS

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, textureImageWidth_,
		textureImageHeight_, texturesCount_ + opacityTexCount_, GL_RGBA,
		GL_UNSIGNED_BYTE, textureArray_); CHECK_GL_ERRORS

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY); CHECK_GL_ERRORS

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

void Graphics::timeCaptureBegin()
{
	if (timeCaptureEnabled_)
		lastTime_ = glutGet(GLUT_ELAPSED_TIME);
}

void Graphics::timeCaptureEnd()
{
	if (timeCaptureEnabled_)
	{
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		accumulatedTime_ += (currentTime - lastTime_);
		accumulatedTimeDivider_++;
		if (accumulatedTime_ > 200)
		{
			std::cout << "Average SSLR time: " <<
				static_cast<float>(accumulatedTime_) / accumulatedTimeDivider_
				<< std::endl;
			accumulatedTime_ = accumulatedTimeDivider_ = 0;
		}
	}
}

void Graphics::drawSponza()
{
	fps_.updateFPS();
	GLuint shaderProgram;
	if (lightsEnabled_)
		shaderProgram = modelShaderLights_;
	else
		shaderProgram = modelShader_;

	GLint cameraLocation =
		glGetUniformLocation(shaderProgram, "camera"); CHECK_GL_ERRORS
	GLint materialIndexLocation =
		glGetUniformLocation(shaderProgram, "material"); CHECK_GL_ERRORS
	GLint cameraPosLoc =
		glGetUniformLocation(shaderProgram, "cameraPosition");
			CHECK_GL_ERRORS
	GLint textureLocation =
		glGetUniformLocation(shaderProgram, "textureArray"); CHECK_GL_ERRORS

	glUseProgram(shaderProgram); CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); CHECK_GL_ERRORS
	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);

//	passLights(modelShader_);

	if (lightsEnabled_)
	{
		GLint moonDepthLocation =
			glGetUniformLocation(shaderProgram, "moonLightDepth");
		GLint moonCamLocation =
			glGetUniformLocation(shaderProgram, "moonCam");
		GLint lightDirLocation =
			glGetUniformLocation(shaderProgram, "moonDir");

		glUniformMatrix4fv(moonCamLocation, 1, GL_TRUE,
			lights_.getMoonMatrix().data().data()); CHECK_GL_ERRORS
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
			camera_.getMatrix().data().data()); CHECK_GL_ERRORS

		glUniform1i(textureLocation, 0); CHECK_GL_ERRORS
		glUniform1i(moonDepthLocation, 1); CHECK_GL_ERRORS
		
		GLfloat values[3] =
			{lightDirection_.x, lightDirection_.y, lightDirection_.z};
		glUniform3fv(lightDirLocation, 1, values);

		glActiveTexture(GL_TEXTURE0); CHECK_GL_ERRORS
		glBindTexture(GL_TEXTURE_2D_ARRAY, textures_); CHECK_GL_ERRORS
		glActiveTexture(GL_TEXTURE1); CHECK_GL_ERRORS
		glBindTexture(GL_TEXTURE_2D, lights_.getShadowTexture()); CHECK_GL_ERRORS
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	} else
	{
		glUniform1i(textureLocation, textures_);
	}

	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		VAOs meshes = materials_[i];
		GLuint materialIndex = i;
		glUniform1ui(materialIndexLocation, materialIndex);
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
	glUseProgram(0);
}

void Graphics::drawPrimaryTextures()
{
	glBindFramebuffer(GL_FRAMEBUFFER, sslr_.mrtBuffer()); CHECK_GL_ERRORS
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, buffers); CHECK_GL_ERRORS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERRORS

	GLuint shaderProgram;
	if (lightsEnabled_)
		shaderProgram = sslr_.mrtLightProgram();
	else
		shaderProgram = sslr_.mrtProgram();

	glViewport(0, 0, windowWidth_, windowHeight_);

	fps_.updateFPS();
	GLint cameraLocation =
		glGetUniformLocation(shaderProgram, "camera"); CHECK_GL_ERRORS
	GLint materialIndexLocation =
		glGetUniformLocation(shaderProgram, "material"); CHECK_GL_ERRORS
	GLint cameraPosLoc =
		glGetUniformLocation(shaderProgram, "cameraPosition");
			CHECK_GL_ERRORS
	GLint textureLocation =
		glGetUniformLocation(shaderProgram, "textureArray"); CHECK_GL_ERRORS

	glUseProgram(shaderProgram); CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); CHECK_GL_ERRORS
	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);

	if (lightsEnabled_)
	{
		GLint moonDepthLocation =
			glGetUniformLocation(shaderProgram, "moonLightDepth");
		GLint moonCamLocation =
			glGetUniformLocation(shaderProgram, "moonCam");
		GLint lightDirLocation =
			glGetUniformLocation(shaderProgram, "moonDir");

		glUniformMatrix4fv(moonCamLocation, 1, GL_TRUE,
			lights_.getMoonMatrix().data().data()); CHECK_GL_ERRORS
		glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
			camera_.getMatrix().data().data()); CHECK_GL_ERRORS

		glUniform1i(textureLocation, 0); CHECK_GL_ERRORS
		glUniform1i(moonDepthLocation, 1); CHECK_GL_ERRORS
		
		GLfloat values[3] =
			{lightDirection_.x, lightDirection_.y, lightDirection_.z};
		glUniform3fv(lightDirLocation, 1, values);

		glActiveTexture(GL_TEXTURE0); CHECK_GL_ERRORS
		glBindTexture(GL_TEXTURE_2D_ARRAY, textures_); CHECK_GL_ERRORS
		glActiveTexture(GL_TEXTURE1); CHECK_GL_ERRORS
		glBindTexture(GL_TEXTURE_2D, lights_.getShadowTexture()); CHECK_GL_ERRORS
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	} else
	{
		glUniform1i(textureLocation, textures_);
	}

	for (unsigned int i = 0; i < scene_->mNumMaterials; i++)
	{
		VAOs meshes = materials_[i];
		GLuint materialIndex = i;
		glUniform1ui(materialIndexLocation, materialIndex);
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
}

void Graphics::drawRecommendations()
{
	timeCaptureBegin();

	glBindFramebuffer(GL_FRAMEBUFFER, sslr_.lrBuffer());
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, buffers); CHECK_GL_ERRORS
	glViewport(0, 0, windowWidth_ / RecTexDivValue, windowHeight_ / RecTexDivValue); CHECK_GL_ERRORS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERRORS
	glUseProgram(sslr_.lrProgram()); CHECK_GL_ERRORS
	
	GLint cameraLocation =
		glGetUniformLocation(sslr_.lrProgram(), "camera"); CHECK_GL_ERRORS

	GLint modelLocation =
		glGetUniformLocation(sslr_.lrProgram(), "model"); CHECK_GL_ERRORS

	GLint viewLocation =
		glGetUniformLocation(sslr_.lrProgram(), "view"); CHECK_GL_ERRORS

	GLint projectionLocation =
		glGetUniformLocation(sslr_.lrProgram(), "projection"); CHECK_GL_ERRORS

	GLint cameraPosLoc =
		glGetUniformLocation(sslr_.lrProgram(), "cameraPosition");
			CHECK_GL_ERRORS

	GLint colourLocation = glGetUniformLocation(
		sslr_.lrProgram(), "colour"); CHECK_GL_ERRORS
	GLint normalLocation = glGetUniformLocation(
		sslr_.lrProgram(), "normal"); CHECK_GL_ERRORS
	GLint reflectionLocation = glGetUniformLocation(
		sslr_.lrProgram(), "reflection"); CHECK_GL_ERRORS
	GLint depthLocation = glGetUniformLocation(
		sslr_.lrProgram(), "depth"); CHECK_GL_ERRORS

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); CHECK_GL_ERRORS
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE,
		camera_.getModel().data().data()); CHECK_GL_ERRORS
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE,
		camera_.getView().data().data()); CHECK_GL_ERRORS
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE,
		camera_.getProjection().data().data()); CHECK_GL_ERRORS

	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);

	glUniform1i(colourLocation, 0); CHECK_GL_ERRORS
	glUniform1i(normalLocation, 1); CHECK_GL_ERRORS
	glUniform1i(reflectionLocation, 2); CHECK_GL_ERRORS
	glUniform1i(depthLocation, 3); CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, sslr_.colourBuffer()); CHECK_GL_ERRORS
	glActiveTexture(GL_TEXTURE1); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, sslr_.normalBuffer()); CHECK_GL_ERRORS
	glActiveTexture(GL_TEXTURE2); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, sslr_.reflectionBuffer()); CHECK_GL_ERRORS
	glActiveTexture(GL_TEXTURE3); CHECK_GL_ERRORS
	glBindTexture(GL_TEXTURE_2D, sslr_.depthBuffer()); CHECK_GL_ERRORS

	glBindVertexArray(sslr_.quadVAO()); CHECK_GL_ERRORS
	glDrawArrays(GL_TRIANGLES, 0, 6); CHECK_GL_ERRORS

	glViewport(0, 0, windowWidth_, windowHeight_); CHECK_GL_ERRORS
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Graphics::drawFinalImage()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERRORS
	glUseProgram(sslr_.drawBuffersProgram()); CHECK_GL_ERRORS
	
	GLint cameraLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "camera"); CHECK_GL_ERRORS

	GLint modelLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "model"); CHECK_GL_ERRORS

	GLint viewLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "view"); CHECK_GL_ERRORS

	GLint projectionLocation =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "projection"); CHECK_GL_ERRORS

	GLint cameraPosLoc =
		glGetUniformLocation(sslr_.drawBuffersProgram(), "cameraPosition");
			CHECK_GL_ERRORS

	GLint windowSizeLoc = glGetUniformLocation(sslr_.drawBuffersProgram(),
		"windowSize"); CHECK_GL_ERRORS

	GLint colourLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "colour"); CHECK_GL_ERRORS
	GLint normalLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "normal"); CHECK_GL_ERRORS
	GLint reflectionLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "reflection"); CHECK_GL_ERRORS
	GLint depthLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "depth"); CHECK_GL_ERRORS
	GLint recommendationsLocation = glGetUniformLocation(
		sslr_.drawBuffersProgram(), "recommendations"); CHECK_GL_ERRORS

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
		camera_.getMatrix().data().data()); CHECK_GL_ERRORS
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE,
		camera_.getModel().data().data()); CHECK_GL_ERRORS
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE,
		camera_.getView().data().data()); CHECK_GL_ERRORS
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE,
		camera_.getProjection().data().data()); CHECK_GL_ERRORS

	GLfloat camPos[3];
	camPos[0] = camera_.position.x;
	camPos[1] = camera_.position.y;
	camPos[2] = camera_.position.z;
	glUniform3fv(cameraPosLoc, 1, camPos);
	GLfloat winSize[2];
	winSize[0] = static_cast<float>(windowWidth_);
	winSize[1] = static_cast<float>(windowHeight_);
	glUniform2fv(windowSizeLoc, 1, winSize);

	glUniform1i(colourLocation, 0); CHECK_GL_ERRORS
	glUniform1i(normalLocation, 1); CHECK_GL_ERRORS
	glUniform1i(reflectionLocation, 2); CHECK_GL_ERRORS
	glUniform1i(depthLocation, 3); CHECK_GL_ERRORS
	glUniform1i(recommendationsLocation, 4); CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sslr_.colourBuffer());
	glGenerateMipmap(GL_TEXTURE_2D); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sslr_.normalBuffer());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sslr_.reflectionBuffer());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, sslr_.depthBuffer());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, sslr_.recommendationsBuffer());
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS

	glBindVertexArray(sslr_.quadVAO()); CHECK_GL_ERRORS
	glDrawArrays(GL_TRIANGLES, 0, 6); CHECK_GL_ERRORS

	glBindVertexArray(0);
	glUseProgram(0);

	timeCaptureEnd();
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
	glEnable(GL_DEPTH_TEST); CHECK_GL_ERRORS
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERRORS

//	drawLights();
	drawSponza();
//	updateLights();

	glutSwapBuffers(); CHECK_GL_ERRORS
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
	{
		if (!timeCaptureEnabled_)
		{
			if (fps_.flip())
				fps_.clear();
		}
	}
	else if (key == 'p' || key == 'P')
	{
		printCameraLocation();
	}
	else if (key == 't' || key == 'T')
	{
		if ((timeCaptureEnabled_ = !timeCaptureEnabled_))
		{
			if (fps_)
				fps_.flip();
			accumulatedTime_ = 0;
			accumulatedTimeDivider_ = 0;
		}
	}
	else if (key == 'l' || key == 'L')
	{
		lightsEnabled_ = !lightsEnabled_;
	}
	else if (key == 'c' || key == 'C')
	{
		setCameraLocation();
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

Graphics::~Graphics()
	{}
