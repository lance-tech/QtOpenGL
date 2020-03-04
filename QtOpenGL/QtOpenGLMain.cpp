#include "QtOpenGLMain.h"
#include <QLayout>

QtOpenGLMain::QtOpenGLMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//QVBoxLayout mainLayout;
	mainLayout.setMargin(0);
	mainLayout.addWidget(&glWindow);
	ui.widget->setLayout(&mainLayout);
}
