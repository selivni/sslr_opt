
//GLuint - VAO, uint - mesh vertices count
typedef std::pair<GLuint, uint> MeshInfo;

typedef std::list<MeshInfo> VAOs;
//GLuint - texture index; VAOs - meshes VAO's
typedef std::pair<GLuint[2], VAOs> MaterialInfo;

class Graphics
{
public:
	void init(int windowWidth, windowHeight);
	void createMap(const aiScene* scene);
	void shutdown();
	void specialFunc(int, int, int);
	void keyboardFunc(unsigned char, int, int);
	void mouseMove(int, int);
	void reshapeFunc(GLint, GLint);
	void display();
	void idle();
	void startLoop();
	~GraphicalController();
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

	std::vector<GLuint> textures_;
	GLuint texturesCount_;
	GLuint opacityTexCount_;
	GLuint opTexPointer_;
	std::vector<MaterialInfo> materials_;

	unsigned int sphereMeshSize_;

	GLuint sponzaShaderOne_;
	GLuint sponzaShaderTwo_;
	GLuint sphereShader_;

	int lastTime_;
	unsigned int frameCounter_;

	LightsArray lights_;
	void updateLights();
	void passLights(GLuint);

	void createCamera();
	void updateFPS();
	void prepareTextures(GLuint);
	void unprepareTextures();
	void prepareProgram(GLuint);

	void drawLights();
	void drawSponza();

	void loadTextures();
	bool loadTexture(const char*, GLuint&);
	void compileShaders();
	std::vector<unsigned int> concatFaces(aiMesh*);
	MeshInfo loadMesh(int, uint&);
	void checkInfo();
	void createLights();
	void createModel();
}
