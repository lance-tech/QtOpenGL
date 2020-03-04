#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glRect2D.h"
#include "glMesh.h"

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
	void Update(double& deltaTime);

	HDC hdc;
	HGLRC hrc;
	QTimer updateTimer;

	int ScreenWidth, ScreenHeight;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 VP;

	glRect2D* rect1;
	glMesh* mesh1;
	
protected:
	virtual void paintEvent(QPaintEvent* paintEvent) override;
	virtual void resizeEvent(QResizeEvent* resizeEvent) override;
};
