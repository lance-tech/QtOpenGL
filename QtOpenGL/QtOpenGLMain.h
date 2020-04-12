#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtOpenGLMain.h"

#include "GLWindow.h"
#include <string>

class QtOpenGLMain : public QMainWindow
{
	Q_OBJECT

public:
	QtOpenGLMain(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtOpenGLMainClass ui;

	QVBoxLayout mainLayout;
	GLWindow glWindow;

private slots:
	void UpdateFPS(double);
};
