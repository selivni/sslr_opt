#include "ModelArrays.h"

vec3f::vec3f(GLfloat x_, GLfloat y_, GLfloat z_): x(x_), y(y_), z(z_)
	{}

vec3f::vec3f(const aiVector3D vector): x(vector.x), y(vector.y), z(vector.z)
	{}

std::vector<vec3f>& ModelArrays::vertices()
	{return vertices_;}
std::vector<vec3f>& ModelArrays::uvCoords()
	{return uvCoords_;}
std::vector<vec3f>& ModelArrays::normals()
	{return normals_;}
std::vector<GLuint>& ModelArrays::indices()
	{return indices_;}
std::vector<GLuint*>& ModelArrays::indicesStarts()
	{return indicesStarts_;}
std::vector<GLsizei>& ModelArrays::indicesCounts()
	{return indicesCounts_;}

ModelArrays::ModelArrays(GLuint verticesCount, GLuint indicesCount)
{
	vertices_.reserve(verticesCount);
	uvCoords_.reserve(verticesCount);
	normals_.reserve(verticesCount);
	indices_.reserve(indicesCount);
	indicesStarts_.clear();
	indicesCounts_.clear();
	indicesSum = 0;
}

void ModelArrays::concat(const aiMesh* mesh, std::vector<unsigned int>& faces)
{
	aiVector3D* vert = mesh->mVertices;
	aiVector3D* uv = mesh->mTextureCoords[0];
	aiVector3D* norm = mesh->mNormals;
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		vertices_.push_back(vert[i]);
		uvCoords_.push_back(uv[i]);
		uvCoords_[uvCoords_.size() - 1].z = mesh->mMaterialIndex;
		normals_.push_back(norm[i]);
	}
	for (auto i = faces.begin(); i != faces.end(); i++)
		indices_.push_back(*i);
	indicesStarts_.push_back(reinterpret_cast<GLuint*> (indicesSum));
	indicesCounts_.push_back(faces.size());
	indicesSum += faces.size();
}
