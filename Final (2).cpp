﻿/*---------------------------------------------------------*/
/* ----------------  Proyecto Final               -----------*/
/*-----------------    2024-1   ---------------------------*/
/*-- Barrionuevo Pérez Daniel Alejandro	No. de cuenta: 316303116	Grupo:03 --*/
/*-- Bustamante Colín Israel			No.de cuenta : 316193384	Grupo:04 --*/
/*-- Martínez Bautista Victor Eduardo	No.de cuenta : 316255637	Grupo:03 --*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 30.0f, 490.0f));
float MovementSpeed = 100.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

bool reversa = false;
bool state = false;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, 0.0f, -1.0f);

float myVar = 0.0f;
float  var1 = 0.0f;
float  var2 = 0.0f;
bool var3 = false;
float var4 = 0.0f;

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		movAuto_y = 0.0f,
		orienta = 0.0f,
		orientaA = 0.0f,
		orientaB = 0.0f,
		t = 0.0f,
		movGAx = 0.0f,
		movGAy = 0.0f,
		movGAz = 0.0f,
		movGMx = 0.0f,
		movGMy = 0.0f,
		movGMz = 0.0f,
		movGOx = 0.0f,
		movGOy = 0.0f,
		movGOz = 0.0f;

bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

int circuito_auto = 0,
circuito_GA = 0,
circuito_GM = 0,
circuito_GO = 0;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posX2 = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		giroBrazoDer = 0.0f,
		giroBrazoIzq = 0.0f,
		rotRodDer = 0.0f,

		giroCabeza = 0.0f,
		girodisco = 0.0f,
		girocalendario = 0.0f,
		giroojo = 0.0f,
		giropiedra = 0.0f,
		movcalendariox = 0.0f,
		movcalendarioy = 0.0f;

// Para calculo de invremento que se va a realizar
float	incX = 0.0f,
		incX2 = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		girobrazoDerInc = 0.0f,
		giroBrazoIzqInc = 0.0f,
		rotRodDerInc = 0.0f,

		giroCabezaInc = 0.0f,
		girodiscoInc = 0.0f,
		girocalendarioInc = 0.0f,
		giroojoInc = 0.0f,
		movcalendarioxInc = 0.0f,
		movcalendarioyInc = 0.0f,
		giropiedraInc = 0.0f;

#define MAX_FRAMES 20  //Nos va a indicar cuantos cuadros claves va a poder guardar nuestro sistema
int i_max_steps = 60;  //Cuantos cuadros intermedios nos va a generar . Entre menos cuadros pida, más rápida va a ser la transicion pero será más brusca
int i_curr_steps = 0;

// Esta estructura es la que se va a encargar de guardar los datos para mi sistema
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posX2;
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float giroBrazoDer;
	float giroBrazoIzq;
	float rotRodDer;

	float giroCabeza;  //De estructura, encargada de guardar los datos  
	float girodisco;
	float girocalendario;
	float giroojo;
	float giropiedra;
	float movcalendariox;
	float movcalendarioy;

}FRAME;

//
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posX2 = posX2;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].giroBrazoDer = giroBrazoDer;

	KeyFrame[FrameIndex].giroBrazoIzq = giroBrazoIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;

	KeyFrame[FrameIndex].giroCabeza = giroCabeza; //Con esto el sistema va a poder guardar la informacion
	KeyFrame[FrameIndex].girodisco = girodisco;
	KeyFrame[FrameIndex].girocalendario = girocalendario;
	KeyFrame[FrameIndex].giroojo = giroojo;
	KeyFrame[FrameIndex].giropiedra = giropiedra;
	KeyFrame[FrameIndex].movcalendariox = movcalendarioy;
	KeyFrame[FrameIndex].movcalendarioy = movcalendarioy;


	FrameIndex++;
}

// Este es para que inicie desde el cuadro 0 para la animacion 
void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posX2 = KeyFrame[0].posX2;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	giroBrazoDer = KeyFrame[0].giroBrazoDer;

	giroBrazoIzq = KeyFrame[0].giroBrazoIzq;
	rotRodDer = KeyFrame[0].rotRodDer;

	giroCabeza = KeyFrame[0].giroCabeza; //Sustituye lo del escenario y comienza desde el cuadro clave cero
	girodisco = KeyFrame[0].girodisco;
	girocalendario = KeyFrame[0].girocalendario;
	giroojo = KeyFrame[0].giroojo;
	giropiedra = KeyFrame[0].giropiedra;
	movcalendariox = KeyFrame[0].movcalendariox;
	movcalendarioy = KeyFrame[0].movcalendarioy;

}

//La funcion de interpolacion es la encargada de realizar la operacion matematica 
void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incX2 = (KeyFrame[playIndex + 1].posX2 - KeyFrame[playIndex].posX2) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	movcalendarioxInc = (KeyFrame[playIndex + 1].movcalendariox - KeyFrame[playIndex].movcalendariox) / i_max_steps;
	movcalendarioyInc = (KeyFrame[playIndex + 1].movcalendarioy - KeyFrame[playIndex].movcalendarioy) / i_max_steps;


	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	girobrazoDerInc = (KeyFrame[playIndex + 1].giroBrazoDer - KeyFrame[playIndex].giroBrazoDer) / i_max_steps;

	giroBrazoIzqInc = (KeyFrame[playIndex + 1].giroBrazoIzq - KeyFrame[playIndex].giroBrazoIzq) / i_max_steps;
	rotRodDerInc = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;


	giroCabezaInc = (KeyFrame[playIndex + 1].giroCabeza - KeyFrame[playIndex].giroCabeza) / i_max_steps;
	girodiscoInc = (KeyFrame[playIndex + 1].girodisco - KeyFrame[playIndex].girodisco) / i_max_steps;
	girocalendarioInc = (KeyFrame[playIndex + 1].girocalendario - KeyFrame[playIndex].girocalendario) / i_max_steps;
	giroojoInc = (KeyFrame[playIndex + 1].giroojo - KeyFrame[playIndex].giroojo) / i_max_steps;
	giropiedraInc = (KeyFrame[playIndex + 1].giropiedra - KeyFrame[playIndex].giropiedra) / i_max_steps;

}

void animate(void)
{
	lightPosition.x = 300.0f * sin(myVar);
	lightPosition.z = 300.0f * cos(myVar);
	myVar += 0.1f;

	if (var3) {
		var4 = 0.8f;
		var1 = lightPosition.x;
		var2 = lightPosition.z;
	}
	else {
		var1 = 0.0f;
		var2 = 0.0f;
		var4 = 0.0f;
	}


	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posX2 += incX2;
			posY += incY;
			posZ += incZ;

			movcalendariox += movcalendarioxInc;
			movcalendarioy += movcalendarioyInc;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			giroBrazoDer += girobrazoDerInc;

			giroBrazoIzq += giroBrazoIzqInc;
			rotRodDer += rotRodDerInc;

			giroCabeza += giroCabezaInc; //Se le va aestar sumando a la variable de dibujo para que haga la transicion
			girodisco += girodiscoInc;
			girocalendario += girocalendarioInc;
			giroojo += giroojoInc;
			giropiedra += giropiedraInc;

			i_curr_steps++;
		}
	}

	//Personajes
	if (animacion)
	{
		if (circuito_GO == 0)
		{
			movGOx += 1.0f;
			if (movGOx == 20.0f)
				circuito_GO = 1;
		}
		if (circuito_GO == 1)
		{
			orientaB = -90.0f;
			movGOz += 1.0f;
			if (movGOz == 100.0f)
				circuito_GO = 2;
		}
		if (circuito_GO == 2)
		{
			orientaB = -240.0f;
			movGOz -= 1.0f;
			if (movGOz == -100.0f)
				circuito_GO = 3;
		}
		if (circuito_GO == 3)
		{
			orientaB = -90.0f;
			movGOz += 1.0f;
			if (movGOz == 0.0f)
				circuito_GO = 4;
		}
		if (circuito_GO == 4)
		{
			orientaB = -180.0f;
			movGOx -= 1.0f;
			if (movGOx == 0.0f)
				circuito_GO = 5;
		}
		if (circuito_GO == 5)
		{
			orientaB = 360.0f;
			circuito_GO = 0;
		}


		if (circuito_GM == 0)
		{
			t += 0.1f;
			movGMy = (20 * sin(1.44) * t) - (0.5 * 9.8 * t * t);
			movGMz = (20 * sin(0.8080) * t);
			if (movGMz >= 83.0f)
				circuito_GM = 1;
		}
		if (circuito_GM == 1)
		{
			movGMy = 0.0f;
			movGMz = 0.0f;
			t = 0.0f;
			if (movGMy == 0.0f)
				circuito_GM = 0;
		}


		if (circuito_GA == 0)
		{
			movGAz += 1.0f;
			if (movGAz == 30.0f)
				circuito_GA = 1;
		}
		if (circuito_GA == 1)
		{
			orienta = -90.0f;
			movGAx -= 1.0f;
			if (movGAx == -70.0f)
				circuito_GA = 2;
		}
		if (circuito_GA == 2)
		{
			orienta = -125.0f;
			movGAz -= 1.0f;
			movGAx -= 1.0f;
			if (movGAz == -55.0f)
				circuito_GA = 3;
		}
		if (circuito_GA == 3)
		{
			orienta = 80.0f;
			movGAx += 1.0f;
			if (movGAx == 180.0f)
				circuito_GA = 4;
		}
		if (circuito_GA == 4)
		{
			orienta = 315.0f;
			movGAz += 1.0f;
			movGAx -= 1.0f;
			if (movGAz == 40.0f)
				circuito_GA = 5;
		}
		if (circuito_GA == 5)
		{
			orienta = -75.0f;
			movGAx -= 1.0f;
			if (movGAx == 0.0f)
				circuito_GA = 6;
		}
		if (circuito_GA == 6)
		{
			orienta = -185.0f;
			movGAz -= 1.0f;
			if (movGAz == 0.0f)
				circuito_GA = 7;
		}
		if (circuito_GA == 7)
		{
			orienta = 5.0f;
			if (movGAz == 0.0f && movGAx == 0.0f)
				circuito_GA = 0;
		}

		/*if (!state)
		{
			movAuto_z -= 3.0f;
			if (movAuto_z <= -170.0f)
				state = true;
		}
		else
		{
			movAuto_y += 3.0f;
			if (movAuto_y >= 200.5f)
			{
				state = 2;
			}
		}
		if (state = 2)
		{
			movAuto_z += 3.0f;
			if (movAuto_z >= 410.5f)
				state = true;
		}
		else
		{
			movAuto_y -= 3.0f;
			if (movAuto_y >= -200.5f)
			{
				state = 3;
			}
		}

		*/
		/*if (!reversa)
		{
			movAuto_z += 3.0f;
			if (movAuto_z >= 200.0f)
				reversa = true;
		}
		else
		{
			movAuto_z -= 3.0f;
			if (movAuto_z <= 0.0f)
			{
				animacion = false;
				reversa = false;
			}
		}
		*/

		/*if (movAuto_z > 200.0f)
			animacion = false;
		else
			movAuto_z += 3.0f;*/
	}


}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC 20241", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models               AQUI SE CARGAN LOS OBJETOS
	// -----------
	Model Escenario("resources/objects/Escenario/salas.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model ojo("resources/objects/Ojo/Ollin.obj");
	Model pilar("resources/objects/Pilar/PilarDestruido.obj");
	Model disco("resources/objects/Disco/DiscoMuerte.obj");
	Model piedra("resources/objects/Piedra/PiedraDestruida.obj");
	Model craneo("resources/objects/Craneo/craneo.obj");
	Model tren("resources/objects/Tren/trenecito.obj");
	Model piramide("resources/objects/piramide/piramide.obj");
	Model calendario("resources/objects/Calendario/Calenadrio.obj");
	Model cabezaolmeca("resources/objects/Cabeza/cabezaolmeca.obj");
	Model cofre("resources/objects/Cofre/cofre.obj");
	Model totem("resources/objects/Totem/totem.obj");
	Model puertaizq("resources/objects/Puerta izq/puerta izq.obj");
	Model puertader("resources/objects/Puerta der/puerta der.obj");
	Model banca("resources/objects/Banca/banca.obj");

	ModelAnim guerrero_a("resources/objects/Guerrero_A/guerrero_a.dae");
	guerrero_a.initShaders(animShader.ID);

	ModelAnim guerrero_o("resources/objects/Guerrero_O/guerrero_o.dae");
	guerrero_o.initShaders(animShader.ID);

	ModelAnim guerrero_m("resources/objects/Guerrero_M/guerrero_m.dae");
	guerrero_m.initShaders(animShader.ID);

	ModelAnim guerrero_odos("resources/objects/Guerrero_O2/guerrero_o2.dae");
	guerrero_odos.initShaders(animShader.ID);

	ModelAnim guerrero_otres("resources/objects/Guerrero_O3/guerrero_o3.dae");
	guerrero_otres.initShaders(animShader.ID);


	//Inicialización de KeyFrames
	//for (int i = 0; i < MAX_FRAMES; i++)
	{
		/*KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;*/

		KeyFrame[0].posX = 0.0f;
		KeyFrame[0].posX2 = 0.0f;
		KeyFrame[0].posY = 0.0f;
		KeyFrame[0].posZ = 0.0f;
		KeyFrame[0].rotRodIzq = 0.0f;
		KeyFrame[0].giroMonito = 0.0f;
		KeyFrame[0].giroBrazoDer = 0.0f;
		KeyFrame[0].giroMonito = 90;
		KeyFrame[0].giroCabeza = 0.0f;
		KeyFrame[0].girodisco = 0.0f;
		//KeyFrame[0].girocalendario = 0.0f;
		//KeyFrame[0].movcalendariox = 0.0f;
		KeyFrame[0].movcalendarioy = 0.0f;
		KeyFrame[0].giroojo = 0.0f;
		KeyFrame[0].giropiedra = 0.0f;

		KeyFrame[1].posX = -100.0f;
		KeyFrame[1].posX2 = 100.0f;
		KeyFrame[1].posY = 50.0f;
		KeyFrame[1].posZ = 0.0f;
		KeyFrame[1].rotRodIzq = 30.0f;
		KeyFrame[1].giroMonito = 0.0f;
		KeyFrame[1].giroBrazoDer = -90.0f;
		KeyFrame[1].giroCabeza = 360.0f;
		KeyFrame[1].girodisco = 360.0f;
		//KeyFrame[1].girocalendario = 1360.0f;
		KeyFrame[1].movcalendariox = -100.0f;
		KeyFrame[1].movcalendarioy = 0.0f;
		KeyFrame[1].giroojo = 360.0f;
		KeyFrame[1].giropiedra = 360.0f;

		KeyFrame[2].girocalendario = 180.0f;
		//KeyFrame[2].movcalendarioy = 0.0f;

		KeyFrame[3].girocalendario = 0.0f;
		KeyFrame[3].movcalendarioy = -40.0f;
		KeyFrame[3].movcalendariox = -60.0f;



		/*KeyFrame[2].posX = 40.0f;
		KeyFrame[2].posY = 0.0f;
		KeyFrame[2].posZ = 0.0f;
		KeyFrame[2].rotRodIzq = -30.0f;
		KeyFrame[2].giroMonito = 90.0f;
		KeyFrame[2].giroBrazoDer = 0.0f;
		KeyFrame[2].girodisco = 180.0f;*/

	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection); //Direccion de los 
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.9f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.000032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.01f);
		staticShader.setFloat("pointLight[1].linear", 0.0009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(var1, var2, var4));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(var1, var2, var4));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.4f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f))); //Apartura que va a tener 
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f))); //difuminacion  
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personajes Animados Complejos
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		//Guerrero_Azteca
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-170.0f + movGAx, 0.0f, -200.0f + movGAz));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		guerrero_a.Draw(animShader);

		//Guerrero_Olmeca
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-270.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		guerrero_o.Draw(animShader);

		//Guerrero_Maya
		model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f + movGMx, 46.0f + movGMy, -350.0f + movGMz));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(orientaA), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		guerrero_m.Draw(animShader);

		//Guerrero_Olmeca2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-270.0f, 0.0f, -200.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		guerrero_odos.Draw(animShader);

		//Guerrero_Olmeca3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-215.0f + movGOx, 0.0f + movGOy, -150.0f + movGOz));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaB), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		guerrero_otres.Draw(animShader);



		// -------------------------------------------------------------------------------------------------------------------------
		// Animaciones estáticas
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		Escenario.Draw(staticShader);

		//Ojo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		tmp = model = glm::rotate(model, glm::radians(giroojo), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ojo.Draw(staticShader);

		//Piedra
		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 10.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giropiedra), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piedra.Draw(staticShader);

		//calendario
		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 30.0f, -100.0f));
		//model = glm::translate(tmp, glm::vec3(0));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0, movcalendarioy, movcalendariox));
		model = glm::rotate(model, glm::radians(girocalendario), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		calendario.Draw(staticShader);

		//Piramide
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, -350.0f));
		model = glm::rotate(model, glm::radians(girocalendario), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		piramide.Draw(staticShader);

		//Pilar destruido
		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, -350.0f));
		model = glm::scale(model, glm::vec3(80.0f));
		staticShader.setMat4("model", model);
		pilar.Draw(staticShader);

		//Disco de la muerte
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 10.0f, -350.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(girodisco), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		disco.Draw(staticShader);

		//Craneo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(220.0f, 10.0f, -170.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		craneo.Draw(staticShader);

		//Tren
		model = glm::translate(glm::mat4(1.0f), glm::vec3(225.0f, 0.0f, 350.0f));
		model = glm::scale(model, glm::vec3(1.3f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		tren.Draw(staticShader);


		//Cabeza olmeca
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-280.0f, 10.0f, -150.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroCabeza), glm::vec3(0.0f, 1.0f, 0.0f)); 
		staticShader.setMat4("model", model);
		cabezaolmeca.Draw(staticShader);

		//Cofre azteca
		model = glm::translate(glm::mat4(1.0f), glm::vec3(220.0f, 0.0f, -220.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		cofre.Draw(staticShader);

		//Totem
		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 35.0f, 150.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		totem.Draw(staticShader);

		//Puerta izq
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::translate(model, glm::vec3(posX, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		puertaizq.Draw(staticShader);

		//Banca1 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 0.0f, -125.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		//Banca2 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, -125.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		//Banca3 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -200.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);


		//Puerta der
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::translate(model, glm::vec3(posX2, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		puertader.Draw(staticShader);

		


		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f + movAuto_y, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.9f));
		staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(giroBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(giroBrazoIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(giroCabeza), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		posY--;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		posY++;

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		giroBrazoDer += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		giroBrazoDer -= 3.0f;

	//Brazo IZQ
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		giroBrazoIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		giroBrazoIzq -= 3.0f;

	//Pierna DER
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotRodDer += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotRodDer -= 3.0f;

	//Cabeza Giro
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		giroCabeza += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		giroCabeza -= 3.0f;



	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && var3 == false)
		var3 = true;
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && var3 == true)
		var3 = false;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)  //TECLA DE RESETEO
		movAuto_z = 0.0f;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}