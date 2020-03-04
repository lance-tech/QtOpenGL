#include "QtOpenGLMain.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtOpenGLMain w;
	w.show();
	return a.exec();
}
