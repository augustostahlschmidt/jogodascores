#include "main.h"

int g_width = 1200;
int g_height = 600;
int g_key;

bool g_mouseHasBeenClicked;
double g_mouseX;
double g_mouseY;

float tileWidth;
float tileHeight;

struct ColorTile {
	GLfloat color_r;
	GLfloat color_g;
	GLfloat color_b;
	bool visible;
};

enum Rodada {
	INICIO_DE_JOGO = 0, PRIMEIRA_RODADA = 1, SEGUNDA_RODADA = 2, TERCEIRA_RODADA = 3, QUARTA_RODADA = 4, ULTIMO_DESENHO = 5, FIM_DE_JOGO = 6
};

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MousePositionCallback(GLFWwindow* window, double x, double y);
void WindowResizeCallback(GLFWwindow* window, int width, int height);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main(void) {

	GLFWwindow* window;

	if (!glfwInit())
		std::cout << "GLFW could not be initialized properly!" << std::endl;

	window = glfwCreateWindow(g_width, g_height, "Jogo Das Cores", NULL, NULL);

	if (!window) {
		glfwTerminate();
		std::cout << "GLFW could not create a window properly!" << std::endl;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW could not initialize its entry points properly!" << std::endl;

	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glViewport(0, 0, g_width, g_height);

	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, WindowResizeCallback);
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetCursorPosCallback(window, MousePositionCallback);

	Shader shader = Shader();

	//shader.CreateShader( pass shaders filepath );
	std::cout << "Remember to load shaders here!" << std::endl;

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f
	};

	GLfloat verticesOrtho[] = {
		0.0f,	 0.0f, 0.0f,  // bottom left
		1200.0f, 0.0f, 0.0f,  // bottom right
		1200.0f, 600.0f, 0.0f,  // top right
		0.0f,	 600.0f, 0.0f   // top left 
	};

	GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOrtho), verticesOrtho, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	auto cores = new ColorTile[20][40]();

	srand(time(0));

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 40; j++) {
			cores[i][j].color_r = (float)(rand() % 255) / 255;  
			cores[i][j].color_g = (float)(rand() % 255) / 255; 	
			cores[i][j].color_b = (float)(rand() % 255) / 255;
			cores[i][j].visible = true;
		}
	}

	glm::mat4 proj = glm::ortho(-1.0f, (float)g_width, (float)g_height, -1.0f, 1.0f, -1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = proj * view * model;
	float tileWidth = g_width / 40;
	float tileHeight = g_height / 20;

	ColorTile corSelecionada = cores[0][0];
	Rodada rodada = Rodada::INICIO_DE_JOGO;
	int pontos = 0;
	ColorTile atual;
	bool calcularPontos = false;
	float distanciaMaximaEntreCores = sqrt((pow(-1.0f, 2)) + (pow(-1.0f, 2)) + (pow(-1.0f, 2)));
	std::string str;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		//Tratamento do clique do mouse
		if (g_mouseHasBeenClicked && rodada != Rodada::INICIO_DE_JOGO) {
			int column = g_mouseX / tileWidth;
			int row = g_mouseY / tileHeight;

			corSelecionada = cores[row][column];

			if (corSelecionada.visible == true) {
				cores[row][column].visible = false;
				calcularPontos = true;
			}
			g_mouseHasBeenClicked = false;
		}

		if (calcularPontos && rodada != Rodada::INICIO_DE_JOGO) {
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 40; j++) {
					atual = cores[i][j];
					if (atual.visible) {
						float distancia_r = corSelecionada.color_r - atual.color_r;
						float distancia_g = corSelecionada.color_g - atual.color_g;
						float distancia_b = corSelecionada.color_b - atual.color_b;
						float distancia = sqrt((pow(distancia_r, 2)) + (pow(distancia_r, 2)) + (pow(distancia_r, 2)));
						if (distancia <= (distanciaMaximaEntreCores * 0.15f)) { //15% de tolerância
							cores[i][j].visible = false;

							if (rodada == Rodada::PRIMEIRA_RODADA) { pontos += 10; str = "Primeira rodada!"; }
							else if (rodada == Rodada::SEGUNDA_RODADA) { pontos += 5; str = "Segunda rodada!"; }
							else if (rodada == Rodada::TERCEIRA_RODADA) { pontos += 2; str = "Terceira rodada!"; }
							else if (rodada == Rodada::QUARTA_RODADA) { pontos += 1; str = "Quarta rodada (final)!";  }
						}
					}
				}
			}

			// Exibir pontuação atual no console
			std::cout << str << std::endl;
			std::cout << "Pontuacao: " << pontos << std::endl;
			std::cout << std::endl;

			//Avança de rodada
			if (rodada == Rodada::PRIMEIRA_RODADA) rodada = Rodada::SEGUNDA_RODADA;
			else if (rodada == Rodada::SEGUNDA_RODADA) rodada = Rodada::TERCEIRA_RODADA;
			else if (rodada == Rodada::TERCEIRA_RODADA) rodada = Rodada::QUARTA_RODADA;
			else if (rodada == Rodada::QUARTA_RODADA) 
				rodada = Rodada::ULTIMO_DESENHO;

			calcularPontos = false;
		}

		// Desenho dos tiles
		glBindVertexArray(VAO);
		shader.Use();
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 40; j++) {
				atual = cores[i][j];
				shader.SetFloat("u_OffsetX", j * tileWidth);
				shader.SetFloat("u_OffsetY", i * tileHeight);
				shader.SetMat4("u_MVP", mvp);
				if (atual.visible)
					shader.SetColor(atual.color_r, atual.color_g, atual.color_b, 1.0f);
				else
					shader.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				//Após o primeiro desenho, aciona a primeira rodada do jogo
				if(rodada == Rodada::INICIO_DE_JOGO) rodada = Rodada::PRIMEIRA_RODADA;
			}
		}

		//Tratamento do fim de jogo
		if (rodada == Rodada::FIM_DE_JOGO) {
			std::cout << "Fim de jogo!" << std::endl;
			std::cout << "Pontuacao final de " << pontos << " pontos!" << std::endl;
			std::cout << std::endl;
			std::cout << "Aperte 'r' para jogar de novo ou qualquer outra tecla para sair." << std::endl;
			std::cout << std::endl;

			char input;
			input = _getch();
			if (input == 'r') {
				rodada = Rodada::INICIO_DE_JOGO;
				pontos = 0;
				srand(time(0));
				for (int i = 0; i < 20; i++) {
					for (int j = 0; j < 40; j++) {
						cores[i][j].color_r = (float)(rand() % 255) / 255;
						cores[i][j].color_g = (float)(rand() % 255) / 255;
						cores[i][j].color_b = (float)(rand() % 255) / 255;
						cores[i][j].visible = true;
					}
				}
			}
			else { std::cout << "Finalizando jogo..." << std::endl; Sleep(1000); std::exit(0); }
		}

		//Após o último desenho, aciona o fim de jogo
		if (rodada == Rodada::ULTIMO_DESENHO) rodada = Rodada::FIM_DE_JOGO;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS)
		g_key = key;
}

void WindowResizeCallback(GLFWwindow* window, int width, int height) {
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		g_mouseHasBeenClicked = true;
}

void MousePositionCallback(GLFWwindow* window, double x, double y) {
	g_mouseX = x;
	g_mouseY = y;
}
