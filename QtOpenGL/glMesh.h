#pragma once

#include <string>

// OpenGL
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

// Other
#include "Shader.h"
#include "ModelLoader.h"

class glMesh
{
public:
	glMesh();
	glMesh(std::string path);
	virtual ~glMesh();

	void InitializeMesh(std::string path);
	void Draw(glm::mat4 &VP, double& time);

	Shader* shader;
	void SetShader(const char* vsShader, const char* fsShader);
	void DestroyShader();
	Shader& GetShader();
	void SetTexture(char *path);
	
	void SetProjectionMatrix(glm::mat4 projection);
	void SetViewMatrix(glm::mat4 View);
	void SetModelMatrix(glm::mat4 model);

private:
	std::string Path;

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ProjectionMatrixID;
	GLuint LightID;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint indexbuffer;
	GLuint vertexArrayObject;
};
