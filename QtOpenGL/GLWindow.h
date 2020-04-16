#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "glRect2D.h"
#include "glMesh.h"

#include <Qt>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>

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
	void Update(double& deltaTime);

	HDC hdc;
	HGLRC hrc;
	QTimer updateTimer;

	float Zoom;
	glm::vec3 CameraCenter;
	glm::vec3 CameraPos;
	glm::vec3 CameraFront;
	glm::vec3 CameraUp;

	bool firstMouse = true;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX;
	float lastY;
	float fov = 45.0f;

	bool has_rotation_started = false;
	int startX = 0;
	int startY = 0;
	float xRotAngle = 0.0f;
	float yRotAngle = 0.0f;
	float zoom;


	int ScreenWidth, ScreenHeight;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 VP;

	glRect2D* rect1;
	glMesh* mesh1;

	glMesh* TempAxis;
	
protected:
	virtual void paintEvent(QPaintEvent* paintEvent) override;
	virtual void resizeEvent(QResizeEvent* resizeEvent) override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;

private slots:
	void PressedKey(int key);
};
