#pragma once

#include <vector>

#include "scene.h"
#include "Utility.h"

struct vec3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	vec3f(const GLfloat, const GLfloat, const GLfloat);
	vec3f(const aiVector3D);
};

class ModelArrays
{
public:
	std::vector<vec3f>& vertices();
	std::vector<vec3f>& uvCoords();
	std::vector<vec3f>& normals();
	std::vector<GLuint>& indices();
	std::vector<GLuint*>& indicesStarts();
	std::vector<GLsizei>& indicesCounts();
	ModelArrays(GLuint, GLuint);
	void concat(const aiMesh*, std::vector<unsigned int>&);
private:
	std::vector<vec3f> vertices_;
	std::vector<vec3f> uvCoords_;
	std::vector<vec3f> normals_;
	std::vector<GLuint> indices_;
	std::vector<GLuint*> indicesStarts_;
	std::vector<GLsizei> indicesCounts_;
	GLuint indicesSum;
};
