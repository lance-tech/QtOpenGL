#include "glRect2D.h"


glRect2D::glRect2D()
{
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	Model = glm::mat4(1.0f);

	width = 1.0f;
	height = 1.0f;

	// Initialize Shader Program
	//SetShader("BlockElement.vert", "BlockElement.frag");

	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);
	glGenBuffers(1, &colorbuffer);
	glGenBuffers(1, &uvbuffer);
	glBindVertexArray(vertexArrayObject);

	color_data[0] = 1.0f;
	color_data[1] = 1.0f;
	color_data[2] = 1.0f;
	color_data[3] = 1.0f;
	color_data[4] = 1.0f;
	color_data[5] = 1.0f;
	color_data[6] = 1.0f;
	color_data[7] = 1.0f;
	color_data[8] = 1.0f;
	color_data[9] = 1.0f;
	color_data[10] = 1.0f;
	color_data[11] = 1.0f;
	index_data[0] = 0;
	index_data[1] = 1;
	index_data[2] = 3;
	index_data[3] = 1;
	index_data[4] = 2;
	index_data[5] = 3;

	// Initialize UV
	uv_data[0] = 1.0f;
	uv_data[1] = 1.0f;

	uv_data[2] = 1.0f;
	uv_data[3] = 0.0f;

	uv_data[4] = 0.0f;
	uv_data[5] = 0.0f;

	uv_data[6] = 0.0f;
	uv_data[7] = 1.0f;

	SetRect(0, 1, 1, 0);
}


glRect2D::~glRect2D()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &indexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &uvbuffer);

	delete shader;
}

void glRect2D::SetRect(GLfloat left, GLfloat top, GLfloat right, GLfloat bottom)
{
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;

	vertex_data[0] = right;
	vertex_data[1] = top;
	vertex_data[2] = 0.0f;
	vertex_data[3] = right;
	vertex_data[4] = bottom;
	vertex_data[5] = 0.0f;
	vertex_data[6] = left;
	vertex_data[7] = bottom;
	vertex_data[8] = 0.0f;
	vertex_data[9] = left;
	vertex_data[10] = top;
	vertex_data[11] = 0.0f;



	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);



	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data), uv_data, GL_STATIC_DRAW);
	// UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}


void glRect2D::SetShader(const char* vsShader, const char* fsShader)
{
	shader = new Shader(vsShader, fsShader);
}

Shader& glRect2D::GetShader()
{
	return *shader;
}

void glRect2D::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

glm::vec4 glRect2D::GetColor()
{
	return glm::vec4(r, g, b, a);
}

void glRect2D::SetProjectionMatrix(glm::mat4 projection)
{
	Projection = projection;
}

void glRect2D::SetViewMatrix(glm::mat4 view)
{
	View = view;
}

void glRect2D::SetModelMatrix(glm::mat4 model)
{
	Model = model;
}

void glRect2D::Draw(glm::mat4& VP, double& time)
{
	// Use our shader
	shader->Use();
	shader->ActiveTexture();

	GLint colorLocation = glGetUniformLocation(shader->Program, "inputColor");
	glUniform4f(colorLocation, r, g, b, a);

	glm::mat4 MVP = VP * Model;
	GLint transformLocation = glGetUniformLocation(shader->Program, "MVP");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint timeLocation = glGetUniformLocation(shader->Program, "Time");
	glUniform1f(timeLocation, (GLfloat)time);

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->DisableTexture();
}