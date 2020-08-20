#include "headers/game.h"

bool PROJECTION_MODE = 0; // ortho = 1, perspective = 0
int OPENGL_MAJOR = 4;
int OPENGL_MINOR = 4;
std::vector<Vertex> generateTriangles();
//Private functions
void Game::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED"
				  << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(
	const char *title,
	bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED"
				  << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);
	//IMPORTANT WHITH PERSPECTIVE MATRIX!!!

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); //IMPORTANT!!
}

void Game::initGLEW()
{
	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED"
				  << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);
	// glDepthMask(GL_FALSE);

	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, this->changeRenderMode);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	if (PROJECTION_MODE)
	{
		this->ProjectionMatrix = glm::ortho(static_cast<float>(-this->framebufferWidth / 200),
											static_cast<float>(this->framebufferWidth / 200),
											static_cast<float>(-this->framebufferHeight / 200),
											static_cast<float>(this->framebufferHeight / 200),
											-1000.0f, 1000.0f);
	}
	else
	{
		this->ProjectionMatrix = glm::perspective(
			glm::radians(this->fov),
			static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
			this->nearPlane,
			this->farPlane);
	}
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
									   (char *)"resources/shaders/vertex_core.glsl", (char *)"resources/shaders/fragment_core.glsl"));
}

void Game::initTextures()
{
	//TEXTURE 0
	this->textures.push_back(new Texture("resources/images/db_trans.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/images/db_trans_specular.png", GL_TEXTURE_2D));

	//TEXTURE 1
	this->textures.push_back(new Texture("resources/images/tex.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("resources/images/tex.jpg", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.4f), glm::vec3(1.f), glm::vec3(1.f),
										   0, 1));
}

void Game::initObjectModels()
{
}

void Game::initModels()
{
	std::vector<Mesh *> meshes;

	std::vector<Mesh *> meshes2;

	std::vector<Vertex> mesh = loadObjFile("resources/models/halfToruss.obj");
	std::vector<Vertex> mesh2 = generateTriangles();
	meshes.push_back(
		new Mesh(
			mesh.data(),
			mesh.size(),
			NULL,
			0,
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(270.f, 0.f, 0.f),
			glm::vec3(0.25f)));
	meshes2.push_back(
		new Mesh(
			mesh2.data(),
			mesh2.size(),
			NULL,
			0,
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(0.05f)));			



	this->models.push_back(new Model(
		glm::vec3(0.f, 0.f, -6.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes));
	this->models.push_back(new Model(
		glm::vec3(0.f, 0.f, -4.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes2));		

	// this->models.push_back(new Model(
	// 	glm::vec3(0.f, 0.f, -3.f),
	// 	glm::vec3(90.f, 0.f, 0.f),
	// 	glm::vec3(0.02f),
	// 	this->materials[0],
	// 	this->textures[TEX_CONTAINER],
	// 	this->textures[TEX_CONTAINER_SPECULAR],
	// 	"resources/models/surface2.obj"));

	// this->models.push_back(new Model(
	// 	glm::vec3(0.f, -4.f, 0.f),
	// 	glm::vec3(0.f, 0.f, 0.f),
	// 	glm::vec3(0.5f),
	// 	this->materials[0],
	// 	this->textures[TEX_CONTAINER],
	// 	this->textures[TEX_CONTAINER_SPECULAR],
	// 	"resources/models/torus.obj"));

	for (auto *&i : meshes)
		delete i;
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
}

void Game::updateUniforms()
{
	//Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	if (PROJECTION_MODE)
	{
		this->ProjectionMatrix = glm::ortho(static_cast<float>(-this->framebufferWidth / 200),
											static_cast<float>(this->framebufferWidth / 200),
											static_cast<float>(-this->framebufferHeight / 200),
											static_cast<float>(this->framebufferHeight / 200),
											-1000.0f, 1000.0f);
	}
	else
	{
		this->ProjectionMatrix = glm::perspective(
			glm::radians(this->fov),
			static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
			this->nearPlane,
			this->farPlane);
	}

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Constructors / Destructors
Game::Game(
	const char *title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable)
	: WINDOW_WIDTH(WINDOW_WIDTH),
	  WINDOW_HEIGHT(WINDOW_HEIGHT),
	  GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	  GL_VERSION_MINOR(GL_VERSION_MINOR),
	  camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	//Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 60.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->iv1 = 2 * (this->nearPlane) * (this->farPlane);
	this->iv2 = this->nearPlane + this->farPlane;
	this->iv3 = this->farPlane - this->nearPlane;

	this->depthPixels = (GLubyte *)malloc(4* WINDOW_WIDTH * WINDOW_HEIGHT);

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initObjectModels();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto *&i : this->models)
		delete i;

	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];
}

//Accessor
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifier
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Game::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		*this->lights[0] = this->camera.getPosition();
	}
	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		this->saveDepthMap();
	}
}

void Game::updateKeyboardInput()
{
	//Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	//Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->models[0]->move(glm::vec3(0.f, 0.05f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->models[0]->move(glm::vec3(0.f, -0.05f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->models[0]->move(glm::vec3(-0.05f, 0.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->models[0]->move(glm::vec3(0.05f, 0.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		this->models[0]->move(glm::vec3(0.f, 0.f, 0.05f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
	{
		this->models[0]->move(glm::vec3(0.f, 0.f, -0.05f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->models[0]->rotate(glm::vec3(1.f, 0.f, 0.f));
	}
	if (glfwGetKey(this->window, GLFW_KEY_V) == GLFW_PRESS)
	{
		this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
	}
}

void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update()
{
	//UPDATE INPUT ---
	this->updateDt();
	this->updateInput();

	// this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
	// this->models[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
	// this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));

	// std::cout << "FPS: " << 1/this->dt << std::endl;
}

void Game::render()
{
	//UPDATE ---
	//updateInput(window);

	//DRAW ---
	//Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update the uniforms
	this->updateUniforms();

	//Render models
	for (auto &i : this->models)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	GLubyte pixels[4 * WINDOW_WIDTH * WINDOW_HEIGHT];
	glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, pixels);

	for (size_t i = 0; i < 4*WINDOW_WIDTH*WINDOW_HEIGHT; i++)
	{
		pixels[i] = pixels[i] - this->depthPixels[i];
		if(pixels[i] == 0){
			std::cout << "Torus touched surface" << std::endl;
			break;
		}
	}
	
	// if (this->smZbuf)
	// {
	// 	glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, this->smZbuf);
	// }

	std::cout << "[" <<  (float)pixels[0] << ", " << (float)pixels[WINDOW_WIDTH-1] <<  ", " << (float)pixels[WINDOW_WIDTH*(WINDOW_HEIGHT) -1 ] << ", " << (float)pixels[WINDOW_WIDTH*(WINDOW_HEIGHT -1)] << "]" << std::endl;

	// asynchronous load depth map to PBO
	// glReadPixels(0,0,WINDOW_WIDTH,WINDOW_HEIGHT,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);

	// // read the depth map
	// glGetBufferSubData(GL_PIXEL_PACK_BUFFER, 0, (GLsizeiptr) WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(float), this->smZbuf);

	// if ( 0 ){
	// 	// for (int i = 0; i < (WINDOW_WIDTH*WINDOW_HEIGHT); i++) {smZbuf[i] = iv1 / (iv2 - (2*smZbuf[i]-1)*iv3); smZbuf[i] += 0.02f*smZbuf[i]*noise[i];}
	// }
	// else {
	// 	for (int i = 0; i < (WINDOW_WIDTH*WINDOW_HEIGHT); i++){ smZbuf[i] = this->iv1 / (this->iv2 - (2*this->smZbuf[i]-1)*this->iv3); }

	// }

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::saveDepthMap()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->updateUniforms();

	//Render models
	for (auto &i : this->models)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	
	glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, this->depthPixels);

	// std::ofstream myfile("depthmap.txt");
	// if (myfile.is_open())
	// {
	// 	myfile << "This is a line.\n";
	// 	myfile << "This is another line.\n";

	// 	for (int count = 0; count < 3; count++)
	// 	{
	// 		myfile << count << "\n";
	// 	}

	// 	for (size_t i = 0; i < WINDOW_HEIGHT; i++)
	// 	{
	// 		for (size_t j = 0; j < WINDOW_WIDTH; j++)
	// 		{
	// 			myfile << (float)pixels[i + j] << " ";
	// 		}
	// 		myfile << "\n";
	// 	}

	// 	myfile.close();
	// }

	this->models.pop_back();

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Static functions
void Game::framebuffer_resize_callback(GLFWwindow *window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};

void Game::changeRenderMode(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

		switch (polygonMode)
		{
		case GL_LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GL_FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		default:
			break;
		}
	}

	if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS)
	{
		PROJECTION_MODE = !PROJECTION_MODE;
	}
	if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS)
	{
	}
}


template <typename T>
std::vector<T> linspace(T a, T b, size_t N)
{
   T h = (b - a) / static_cast<T>(N - 1);
   std::vector<T> xs(N);
   typename std::vector<T>::iterator x;
   T val;
   for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h)
      *x = val;
   return xs;
}

int binomialCoeff(int n, int k)
{
   // Base Cases
   if (k == 0 || k == n)
      return 1;

   // Recur
   return binomialCoeff(n - 1, k - 1) +
          binomialCoeff(n - 1, k);
}

double Bernstein(int i, int n, double u)
{

   double *temp = new double[n + 1];

   for (int j = 0; j <= n; ++j)
   {

      temp[j] = 0.0;
   }

   temp[n - i] = 1.0;
   double u1 = 1.0 - u;

   for (int k = 1; k <= n; ++k)
   {

      for (int j = n; j >= k; --j)
      {

         temp[j] = u1 * temp[j] + u * temp[j - 1];
      }
   }

   // std::cout << "B" << i << n << "(" << u << ")"
   //           << ":" << temp[n];
   return temp[n];
}
std::vector<Vertex> generateTriangles()
{
   int divisions = 51;
   double umin = 0.0, umax = 1.0, vmin = 0.0, vmax = 1.0;

   int N = 4, M = 4;

   int control_points[16][3] = {
        {-75, -75, 5},
        {-75, -25, -10},
        {-75, 25, 15},
        {-75, 75, 10},
        {-25, -75, 5},
        {-25, -25, 10},
        {-25, 25, 15},
        {-25, 75, 20},
        {25, -75, 25},
        {25, -25, 30},
        {25, 25, 20},
        {25, 75, 25},
        {75, -75, 15},
        {75, -25, 25},
        {75, 25, 25},
        {75, 75, 1},
   };

   std::vector<double> u1 = linspace(umin, umax, divisions);
   std::vector<double> v1 = linspace(vmin, vmax, divisions);

   glm::vec3 finalControlPointList[N][M];

   int temp = 0;

   for (size_t i = 0; i < N; i++)
   {
      for (size_t j = 0; j < M; j++)
      {

         finalControlPointList[i][j] = glm::vec3(control_points[temp][0], control_points[temp][1], control_points[temp][2]);
         temp++;
      }
   }
   // int index1 = 1;
   // int index2 = 1;
   // std::cout << finalControlPointList[index1][index2].x << " " << finalControlPointList[index1][index2].y << " " << finalControlPointList[index1][index2].z << std::endl;

   double u = 0, v = 0, b1, b2;
   glm::vec3 triangle_array[divisions][divisions];
   glm::vec3 temp_vector;

   for (size_t i1 = 0; i1 < divisions; i1++)
   {
      u = u1[i1];
      for (size_t j1 = 0; j1 < divisions; j1++)
      {
         v = v1[j1];
         temp_vector = glm::vec3(0.f);
         for (size_t i = 0; i < N; i++)
         {
            for (size_t j = 0; j < M; j++)
            {
               // b1 = Bernstein(i,N, u);
               // b2 = Bernstein(j, M, v);
               // std::cout << std::fixed<< b1*b2 << std::endl;

               b1 = binomialCoeff(N - 1, i) * pow(u, i) * pow((1 - u), (N - 1 - i)) * binomialCoeff(M - 1, j) * pow(v, j) * pow((1 - v), (M - 1 - j));
               temp_vector += (float)b1 * finalControlPointList[i][j];
            }
         }

         triangle_array[i1][j1] = temp_vector;
      }
   }

   int index1 = 0;
   int index2 = 0;
   std::cout << triangle_array[index1][index2].x << " " << triangle_array[index1][index2].y << " " << triangle_array[index1][index2].z << std::endl;

   std::vector<Vertex> vertexArray;
   Vertex tempVertex;
   tempVertex.color = glm::vec3(1.f);
   tempVertex.normal = glm::vec3(1.f);
   tempVertex.texcoord = glm::vec2(0.f, 1.f);

   for (size_t i = 0; i < divisions - 1; i++)
   {
      for (size_t j = 0; j < divisions - 1; j++)
      {
         tempVertex.position = triangle_array[j][i];
         vertexArray.push_back(tempVertex);
         tempVertex.position = triangle_array[j + 1][i];
         vertexArray.push_back(tempVertex);
         tempVertex.position = triangle_array[j + 1][i + 1];
         vertexArray.push_back(tempVertex);

         tempVertex.position = triangle_array[j][i];
         vertexArray.push_back(tempVertex);
         tempVertex.position = triangle_array[j + 1][i + 1];
         vertexArray.push_back(tempVertex);
         tempVertex.position = triangle_array[j][i + 1];
         vertexArray.push_back(tempVertex);
      }
   }

   return vertexArray;
}
