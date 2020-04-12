#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtOpenGLMain.h"

#include <QKeyEvent>
#include "GLWindow.h"


class QtOpenGLMain : public QMainWindow
{
	Q_OBJECT

public:
	QtOpenGLMain(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtOpenGLMainClass ui;

	QVBoxLayout mainLayout;
	GLWindow glWindow;

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;

signals:
	void KeyPressed(int key);
};
