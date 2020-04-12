#include "GLWindow.h"
//#include <iostream>


const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float WINDOW_LEFT = -20.f;
const float WINDOW_RIGHT = 20.f;
const float WINDOW_BOTTOM = -20.f;
const float WINDOW_TOP = 20.f;

static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
};

GLWindow::GLWindow(QWidget* parent)
	: QWidget(parent), hdc(nullptr), times(0.0), frame(0), lastTime(GetTickCount64())
{
	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	updateTimer.start(0);

	InitGL();
	Initialize();
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

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);


	delete quad_shader;
	delete shader;

	glDeleteFramebuffers(1, &frameBufferName);
	glDeleteTextures(1, &renderedTexture);
	glDeleteRenderbuffers(1, &depthRenderbuffer);
	glDeleteBuffers(1, &quad_vertexbuffer);

}

void GLWindow::Initialize()
{
	rect1 = new glRect2D();
	rect1->SetShader("base.vs", "RampUVMappingSprite.frag");
	rect1->GetShader().SetTexture("smoke1.bmp");
	//rect1->GetShader().SetTexture("smoke-sprite.bmp");
	//rect1->GetShader().SetTexture("perlin_noise.bmp");
	rect1->SetRect(-1, 1, 1, -1);

	rampTexture = ImageLoader::BMP("Ramp_Fire.bmp");
	rampTextureID = glGetUniformLocation(rect1->GetShader().Program, "rampSampler");


	mesh1 = new glMesh("teapot.obj");
	mesh1->SetProjectionMatrix(Projection);
	mesh1->SetViewMatrix(View);
	mesh1->SetModelMatrix(glm::mat4(1.0f));
	//mesh1->SetShader("Lambert.vert", "Lambert.frag");
	mesh1->SetShader("DirectionalLight.vert", "DirectionalLight.frag");
	mesh1->GetShader().SetTexture("perlin_noise.bmp");


	shader = new Shader("StandardShadingRTT.vert", "StandardShadingRTT.frag");

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	programID = shader->Program;

	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

	Texture = ImageLoader::DDS("uvmap.DDS");

	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	bool res = ModelLoader::Obj("suzanne.obj", vertices, uvs, normals);

	ModelLoader::indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");





	// Render to texture
	frameBufferName = 0;
	glGenFramebuffers(1, &frameBufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);

	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL (the last "0" means "empty")
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ScreenWidth, ScreenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);


	// Set "renderedTexture" as our color attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);


	glDrawBuffers(1, DrawBuffers);	// 1 is the size of drawbuffers

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		qDebug() << "Error FrameBuffer";
	}

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	quad_shader = new Shader("Passthrough.vert", "WobblyTexture.frag");
	quad_programID = quad_shader->Program;
	texID = glGetUniformLocation(quad_programID, "renderedTexture");
	timeID = glGetUniformLocation(quad_programID, "time");

	//noiseTexture = ImageLoader::BMP("perlin_noise.bmp");
	noiseTexture = ImageLoader::BMP("render.bmp");
	//noiseTexture = ImageLoader::DDS("uv_deform.dds");
	
	noiseTextureID = glGetUniformLocation(quad_programID, "noiseTexture");

}

void GLWindow::Update(double& deltaTime)
{
	float speed = deltaTime * 0.05f;

	{
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
		glViewport(0, 0, ScreenWidth, ScreenHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs();
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = Projection * View * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}


	{
		//// Render to our framebuffer
		//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
		//// Render on the whole framebuffer, complete from the lower left corner to the upper right
		//glViewport(0, 0, ScreenWidth, ScreenHeight);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 modelMatrix1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 2.5f, 0.0f));
		//modelMatrix1 = glm::rotate(modelMatrix1, float(45 * (3.14 / 180.0)), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(2.0f, 2.0f, 2.0f));
		modelMatrix1 = glm::rotate(modelMatrix1, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
		rect1->SetModelMatrix(modelMatrix1);

		rect1->GetShader().Use();
		rect1->GetShader().ActiveTexture();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rampTexture);
		glUniform1i(rampTextureID, 1);

		rect1->Draw(VP, deltaTime);

		glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, -3.0f, 2.5f));

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3((i * 3) - 13, -5.0f, (j * -3) + 4));
				modelMatrix2 = glm::rotate(modelMatrix2, float(((i * 10) + (j * 20)) + speed * (3.14 / 180.0)), glm::vec3(0.0f, 1.0f, 0.0f));
				mesh1->SetModelMatrix(modelMatrix2);
				mesh1->Draw(VP, deltaTime);
			}
		}
	}

	
	{
		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, ScreenWidth, ScreenHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(quad_programID);

		// Bind texture in texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glUniform1i(texID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glUniform1i(noiseTextureID, 1);

		GLuint renderTextureMatrixID = glGetUniformLocation(quad_programID, "MVP");
		glUniformMatrix4fv(renderTextureMatrixID, 1, GL_FALSE, &VP[0][0]);

		glUniform1f(timeID, speed);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
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


	//glViewport(0, 0, ScreenWidth, ScreenHeight);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

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

	View = glm::lookAt(
		glm::vec3(0, 10, 10), // World Space
		glm::vec3(0, 0, 0), // looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	VP = Projection * View;

	
}

void GLWindow::paintEvent(QPaintEvent* paintEvent)
{
	double time = GetTickCount64();

	elapsedTime = time - lastTime;
	frame++;

	if (elapsedTime > 1000.0 / 30.0)
	{
		qDebug() << frame;

		frame = 0;
		lastTime = time;
	}

	//if (elapsedTime >= 1.0)
	//{
	//	emit UpdateFPS(1000.0 / double(frame));

	//	frame = 0;
	//	lastTime = time;
	//}
	
	

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