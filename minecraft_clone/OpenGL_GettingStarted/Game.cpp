#include "Game.h"
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Game* Game::instance = nullptr;

Game::Game(unsigned int width, unsigned int height)
    :SCR_WIDTH(width), SCR_HEIGHT(height),
    camera(glm::vec3(0.0f, 10.0f, -3.0f)),
    lastX(width / 2.0f), lastY(height / 2.0f), firstMouse(true),
    deltaTime(0.0f), lastFrame(0.0f)
{
    instance = this;
    initWindow();
    loadResources();
    setupMesh();
}

Game::~Game() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
    delete world;
    glfwTerminate();
}

void Game::initWindow() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MC u cpp", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::loadResources() {
    shader = new Shader("VertexShader.vs", "FragmentShader.fs");

    world = new World(0);
    std::vector<Vertex> vertices = world->generateMesh();
    this->verticesCount = (int)vertices.size();

    // Create VAO/VBO/EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load texture
    glGenTextures(1, &textureAtlas);
    glBindTexture(GL_TEXTURE_2D, textureAtlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("images/texture_atlas_2.png", &width, &height, &nrChannels, 0);
    std::cout << width << ' ' << height << ' ' << nrChannels << std::endl;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture\n";
    }
    stbi_image_free(data);

    shader->use();
    shader->setInt("texture1", 0);
}

void Game::setupMesh() {
    // Already handled in loadResources for simplicity
}

void Game::run() {
    float lastTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    int frameCounter = 0;
    float fps = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        update();
        render();

        // FPS calculation
        float currentTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        float deltaTime = currentTime - lastTime;
        if (deltaTime >= 1.0f) {
            //system("cls");
            fps = static_cast<float>(frameCounter) / deltaTime;
            frameCounter = 0;
            lastTime = currentTime;
            //std::cout << "FPS: " << fps << std::endl;
            //std::cout << "Camera Position: " << this->camera.Position.x<<' '<<this->camera.Position.y<<' '<<this->camera.Position.z << std::endl;  
        }
        frameCounter++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void Game::render() {
    glClearColor(0.530f, 0.807f, 0.921f, 1.000f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas);

    shader->use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    shader->setMat4("projection", projection);
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (instance == nullptr) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (instance->firstMouse) {
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstMouse = false;
    }

    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos;

    instance->lastX = xpos;
    instance->lastY = ypos;

    instance->camera.ProcessMouseMovement(xoffset, yoffset);
}

void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (instance)
        instance->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Game::handleChunkChanged()
{
    glm::vec2 curChunkCoord = this->world->toChunkCoordinates(this->camera.Position);
    if (curChunkCoord != this->camera.ChunkCoordinates) {
        this->camera.ChunkCoordinates = curChunkCoord;
        onChunkChanged(curChunkCoord);
    }
}

void Game::onChunkChanged(glm::vec2 chunkCoordinates)
{
    if (threadRunning)return;

    threadRunning = true;
    workerThread = std::thread([this, chunkCoordinates]() {
        this->world->updateChunks(chunkCoordinates);
        std::vector<Vertex>generated = this->world->generateMesh();

        {
            std::lock_guard<std::mutex>lock(this->meshMutex);
            this->newMesh = std::move(generated);
            this->meshReady = true;
        }

        threadRunning = false;
    });
    workerThread.detach();
}

void Game::update() {
    handleChunkChanged();
    handleMeshReady();
}
void Game::applyMesh(const std::vector<Vertex>& mesh) {
    this->verticesCount = (int)mesh.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), mesh.data(), GL_STATIC_DRAW);
}

void Game::handleMeshReady() {
    if (meshReady) {
        std::lock_guard<std::mutex> lock(meshMutex);
        applyMesh(newMesh);
        meshReady = false;
    }
}



void Game::updateMesh() {
    std::vector<Vertex> vertices = world->generateMesh();
    this->verticesCount = (int)vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}
