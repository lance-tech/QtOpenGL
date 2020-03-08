#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glRect2D.h"
#include "glMesh.h"

#include <qdebug.h>

#include <Qt>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <QResizeEvent>
#include <qopenglwidget.h>



class GLWindow : public QWidget
{
	Q_OBJECT

public:
	explicit GLWindow(QWidget* parent = Q_NULLPTR);
	virtual ~GLWindow() override;

	virtual QPaintEngine* paintEngine() const override
	{
		return nullptr;
	}

private:
	void InitGL();
	void Initialize();
	void Update(double& deltaTime);

	HDC hdc;
	HGLRC hrc;
	QTimer updateTimer;

	int ScreenWidth = 1024;
	int ScreenHeight = 768;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 VP;

	glRect2D* rect1;
	glMesh* mesh1;


	//
	Shader* shader;
	GLuint VertexArrayID;
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint Texture;
	GLuint TextureID;

	GLuint noiseTexture;
	GLuint noiseTextureID;

	GLuint rampTexture;
	GLuint rampTextureID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	GLuint LightID;



	// For Render To Texture
	Shader* quad_shader;
	GLuint quad_vertexbuffer;
	GLuint quad_programID;
	GLuint texID;
	GLuint timeID;

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	GLuint depthTexture;
	GLuint depthRenderbuffer;

	GLuint renderedTexture;
	GLuint frameBufferName;
	
protected:
	virtual void paintEvent(QPaintEvent* paintEvent) override;
	virtual void resizeEvent(QResizeEvent* resizeEvent) override;
};
