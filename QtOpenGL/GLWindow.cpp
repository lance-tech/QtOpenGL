#include "GLWindow.h"
//#include <iostream>
#include <qdebug.h>



const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float WINDOW_LEFT = -20.f;
const float WINDOW_RIGHT = 20.f;
const float WINDOW_BOTTOM = -20.f;
const float WINDOW_TOP = 20.f;


GLWindow::GLWindow(QWidget* parent)
	: QWidget(parent), hdc(nullptr),
	CameraPos(glm::vec3(0.0f, 3.0f, 10.0f)),
	CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	CameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
	InitGL();
	
	rect1 = new glRect2D();
	rect1->SetShader("base.vs", "base.frag");
	rect1->GetShader().SetTexture("perlin_noise.bmp");
	rect1->SetRect(-1, 1, 1, -1);

	mesh1 = new glMesh("teapot.obj");
	mesh1->SetProjectionMatrix(Projection);
	mesh1->SetViewMatrix(View);
	mesh1->SetModelMatrix(glm::mat4(1.0f));
	//mesh1->SetShader("Lambert.vert", "Lambert.frag");
	mesh1->SetShader("DirectionalLight.vert", "DirectionalLight.frag");
	mesh1->GetShader().SetTexture("perlin_noise.bmp");
}

GLWindow::~GLWindow()
{
	if (hrc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		hrc = NULL;
		ReleaseDC((HWND)winId(), hdc);
	}

	delete rect1;
	delete mesh1;
}

void GLWindow::Update(double& deltaTime)
{
	//qDebug() << "Render!";

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		float speed = deltaTime * 0.005f;

		View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
		glm::quat _quaternion(glm::vec3(glm::radians(xRotAngle), glm::radians(yRotAngle), glm::radians(0.0f)));
		glm::mat4 rotationMatrix = glm::toMat4(_quaternion);
		VP = Projection * View * rotationMatrix;

		//glm::mat4 modelMatrix1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 2.5f, 0.0f));
		////modelMatrix1 = glm::rotate(modelMatrix1, float(45 * (3.14 / 180.0)), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelMatrix1 = glm::rotate(modelMatrix1, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
		//rect1->SetModelMatrix(modelMatrix1);
		//rect1->Draw(VP, deltaTime);


		glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), float(speed * (3.14 / 180.0)), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMatrix2 = glm::translate(modelMatrix2, glm::vec3((j - 5) * 1.5f, 0.0f, (i - 5) * 1.5f));
				mesh1->SetModelMatrix(rotMatrix * modelMatrix2);
				mesh1->Draw(VP, deltaTime);
			}
		}
	}


	SwapBuffers(hdc);
}

void GLWindow::InitGL()
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hdc = GetDC((HWND)winId());

	int windowPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, windowPixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		qDebug() << "Failed to initialize GLEW";
		//std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}


	glViewport(0, 0, ScreenWidth, ScreenHeight);

	glClearColor(0.0f, 0.0f, 0.8f, 1.0f);

	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup Camera
	//Projection = glm::ortho(
	//	WINDOW_LEFT,
	//	WINDOW_RIGHT,
	//	WINDOW_BOTTOM,
	//	WINDOW_TOP,
	//	-100.0f, 100.0f);
	//View = glm::lookAt(
	//	glm::vec3(0, 0, 1), // World Space
	//	glm::vec3(0, 0, 0), // looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	View = glm::lookAt(CameraPos, CameraFront, CameraUp);

	//View = glm::lookAt(
	//	glm::vec3(0, 10, 10), // World Space
	//	glm::vec3(0, 0, 0), // looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);

	VP = Projection * View;

	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	updateTimer.start(0);

	this->setFocus();
}

void GLWindow::paintEvent(QPaintEvent* paintEvent)
{
	double time = GetTickCount64();
	Update(time);
}

void GLWindow::resizeEvent(QResizeEvent* resizeEvent)
{
	QWidget::resizeEvent(resizeEvent);
	auto size = resizeEvent->size();
	ScreenWidth = size.width();
	ScreenHeight = size.height();

	if ((ScreenWidth < 0) || (ScreenHeight < 0))
		return;

	if (glViewport != NULL) {
		glViewport(0, 0, ScreenWidth, ScreenHeight);
	}
}

void GLWindow::mousePressEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();

	has_rotation_started = true;
	startX = pos.x();
	startY = pos.y();
	
	qDebug() << has_rotation_started;
}

void GLWindow::mouseReleaseEvent(QMouseEvent* event)
{
	has_rotation_started = false;

	qDebug() << has_rotation_started;
}

void GLWindow::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();

	if (has_rotation_started)
	{
		xRotAngle = xRotAngle + (pos.y() - startY);
		yRotAngle = yRotAngle + (pos.x() - startX);

		startX = pos.x();
		startY = pos.y();
	}
	/*if (firstMouse)
	{
		lastX = pos.x();
		lastY = pos.y();
		firstMouse = false;
	}
	

	qDebug() << "Last XY : " << lastX << ", " << lastY;

	float xoffset = pos.x() - lastX;
	float yoffset = lastY - pos.y();

	qDebug() << "Offset XY : " << xoffset << ", " << yoffset;

	lastX = pos.x();
	lastY = pos.y();

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.f;
	}

	glm::vec3 front;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	CameraFront = glm::normalize(front);*/

	//qDebug() << CameraFront.x << ", " << CameraFront.y << ", " << CameraFront.z;
}

void GLWindow::PressedKey(int key)
{
	if (key == Qt::Key_W)
	{
		CameraPos += CameraFront;
		qDebug() << CameraPos.x << ", " << CameraPos.y << ", " << CameraPos.z;
	}

	if (key == Qt::Key_S)
	{
		CameraPos -= CameraFront;
	}

	if (key == Qt::Key_A)
	{
		CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp));
	}

	if (key == Qt::Key_D)
	{
		CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp));
	}
}