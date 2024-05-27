#ifndef GL
#define GL
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <cstring>
#include <tuple>
#include <cmath>
#include <vector>

#include "callbacks.h"
#include "inputs.h"
#include "shader.h"
#include "image.h"

#define WIDTH 600
#define HEIGHT 600

GLFWwindow* initialise(void)
{
	std::cout << "Initialising GLFW" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "Conway's Game of Life", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window with GLFW" << std::endl;
		glfwTerminate();
		return window;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	std::cout << "Initialising GLAD" << std::endl;
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		window = NULL;
		return window;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	return window;
}

void conwayUpdate(Image &canvas, Image &buffer)
{
	for (int x = 0; x < canvas.width; x++) {
		for (int y = 0; y < canvas.height; y++) {
			int neighbours = 0;

			// Iterate in a 3x3 grid around the pixel to count neighbours
			for (int xp = -1; xp <= 1; xp++) {
				for (int yp = -1; yp <= 1; yp++) {
					// If xp and yp are 0, the cell is the current cell (hence don't count as neighbour)
					if (xp != 0 || yp != 0) {
						if (canvas.getPixel(x+xp, y+yp) != 0)
						{
							neighbours++;
						}
					}
				}
			}

			if (canvas.getPixel(x,y) >= 100) {
				// If the cell is alive...
				switch (neighbours)
				{
				// Underpopulation
				case 2: case 3:
					buffer.setPixel(x,y,255);
					break;
				case 0: case 1: default:
					buffer.setPixel(x,y,0);
					break;
				}
			} else {

				// If the cell is dead...
				if (neighbours == 3) {
					buffer.setPixel(x,y,255);
				} else {
					buffer.setPixel(x,y,0);
				}
				

			}
		}
	}
}

int main(void) {
	GLFWwindow* window = initialise();
	if (window==NULL) return -1;

	float vertices[] = {
	   -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	   -1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	Shader program = Shader("resources/shader.vs", "resources/shader.fs");
	program.use();

	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create image canvas to hold GOL
	int frame_number = 0;
	Image canvas(100, 100, 1);
	Image canvas_buffer(100, 100, 1);
	std::vector<Image*> canvases = {&canvas, &canvas_buffer};
	int current_canvas_index = 0;
	// Current canvas is given by *canvases.get(current_canvas_index)
	// Buffer canvas is given by *canvases.get(!current_canvas_index)
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, canvas.width, canvas.height, 0, GL_RED, GL_UNSIGNED_BYTE, canvas.data());
    glGenerateMipmap(GL_TEXTURE_2D);

	// Initialise glider on GOL canvas
	canvas.setPixel(25,25,255);
	canvas.setPixel(26,24,255);
	canvas.setPixel(26,23,255);
	canvas.setPixel(25,23,255);
	canvas.setPixel(24,23,255);

	bool running = true;
	bool KEY_SPACE_lock = false;
	bool KEY_C_lock = false;
	

	while ( !glfwWindowShouldClose(window) )
	{
		processInputs(window);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !KEY_SPACE_lock) {
			KEY_SPACE_lock = true;
			running = !running;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			KEY_SPACE_lock = false;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !KEY_C_lock) {
			KEY_C_lock = true;
			double x_raw, y_raw;
			glfwGetCursorPos(window, &x_raw, &y_raw);
			
			//~ First convert mouse coords to NDC
			//~ Then apply other transforms
			//~ Convert to NDC:
				//~ float x = ((float)x_raw)/WIDTH*2-1;
				//~ float y = ((float)y_raw)/WIDTH*2-1;

			// Convert to texture coordinates
			float x = ((float)x_raw)/WIDTH*canvas.width;
			float y = ((float)y_raw)/HEIGHT*canvas.height;
			
			std::cout << round(x) << ", "<< round(y) << std::endl;
			//std::cout <<  << std::endl;
			canvas.setPixel(
				round(x),
				round(y),
				((canvas.getPixel(round(x),round(y))==0) ? 255 : 0)
			);
			canvas_buffer.setPixel(
				round(x),
				round(y),
				((canvas_buffer.getPixel(round(x),round(y))==0) ? 255 : 0)
			);
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
			KEY_C_lock = false;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		program.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Equivalent of swapping buffers

		if (running) {
			conwayUpdate(
				*canvases[current_canvas_index],
				*canvases[!current_canvas_index]
			);
			current_canvas_index = !current_canvas_index;
		}
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, canvas.width, canvas.height, 0, GL_RED, GL_UNSIGNED_BYTE, (*canvases[current_canvas_index]).data());

		

		glfwSwapBuffers(window);
		glfwPollEvents();	
		frame_number++;
	}

	glfwTerminate();
	return 0;
}