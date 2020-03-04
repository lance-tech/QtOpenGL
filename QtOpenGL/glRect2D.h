#pragma once

// OpenGL
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

// Other
#include "Shader.h"
#include <cmath>



class glRect2D
{
public:
	glRect2D();
	virtual ~glRect2D();

	void Draw(glm::mat4 &VP, double& time);
	void SetShader(const char* vsShader, const char* fsShader);
	Shader& GetShader();

	void SetProjectionMatrix(glm::mat4 projection);
	void SetViewMatrix(glm::mat4 View);
	void SetModelMatrix(glm::mat4 model);

protected:
	Shader* shader;

private:
	GLfloat color_data[12];
	GLfloat vertex_data[12];
	GLfloat uv_data[8];
	GLuint index_data[6];

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint indexbuffer;
	GLuint colorbuffer;
	GLuint vertexArrayObject;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;

public:
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;

	int width;
	int height;

	GLfloat left;
	GLfloat top;
	GLfloat right;
	GLfloat bottom;

	virtual void SetRect(GLfloat left, GLfloat top, GLfloat right, GLfloat bottom);
	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	glm::vec4 GetColor();
};

