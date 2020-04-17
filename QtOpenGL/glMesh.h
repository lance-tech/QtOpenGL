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
	void Draw(double& time);

	Shader* shader;
	void SetShader(const char* vsShader, const char* fsShader);
	void DestroyShader();
	Shader& GetShader();
	void SetTexture(char *path);
	
	void SetProjectionMatrix(glm::mat4 projection);
	void SetViewMatrix(glm::mat4 View);
	void SetModelMatrix(glm::mat4 model);
	void SetViewRotMatrix(glm::mat4 mat);

	inline void SetViewPosition(glm::vec3 pos) { ViewPosition = pos; }
	inline void SetLightPosition(glm::vec3 pos) { LightPosition = pos; }

private:
	std::string Path;

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ProjectionMatrixID;
	GLuint ModelView3x3MatrixID;
	GLuint LightID;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 ViewRotate;

	glm::vec3 ViewPosition;
	glm::vec3 LightPosition;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint indexbuffer;
	GLuint vertexArrayObject;

	GLuint tangentBuffer;
	GLuint bitangentBuffer;
};
