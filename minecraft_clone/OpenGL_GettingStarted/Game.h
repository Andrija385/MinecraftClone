#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <mutex>
#include <atomic>
#include <optional>

#include "Shader.h"
#include "Camera.h"
#include "Chunk.h"
#include "World.h"
#include "stb_image.h"


class Game
{
public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void run();

private:
	GLFWwindow* window;
	unsigned int SCR_WIDTH, SCR_HEIGHT;

	Camera camera;
	float lastX, lastY;
	bool firstMouse;

	float deltaTime;
	float lastFrame;

	Shader* shader;
	World* world;
	unsigned int VBO, VAO, textureAtlas;

	int verticesCount;

	std::mutex meshMutex;
	std::atomic<bool> meshReady = false;
	std::vector<Vertex> newMesh; // Shared between threads

	std::thread workerThread;
	std::atomic<bool> threadRunning = false;

	void initWindow();
	void loadResources();
	void setupMesh();
	void updateMesh();
	void update();

	void processInput();
	void render();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// For static callbacks to access instance
	static Game* instance;

	void handleChunkChanged();
	void onChunkChanged(glm::vec2 chunkCoordinates);

	void handleMeshReady();
	void applyMesh(const std::vector<Vertex>& mesh);
};

