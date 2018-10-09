#include "Scene.h"


Scene::Scene() {

}

void Scene::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(camera->FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(camera->BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(camera->LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(camera->RIGHT, deltaTime);
}

void Scene::mouse_callback(GLFWwindow* window, double xpos, double ypos) {

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

	globalCamera->ProcessMouseMovement(xoffset, yoffset);
	
}

void Scene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	globalCamera->ProcessMouseScroll(yoffset);
}

int Scene::setupGLFW() {

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Trabalho GA", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

}

int Scene::init(string base_path, string obj_filename)
{	
	globalCamera = camera;
	if (this->setupGLFW() == -1) {
		return -1;
	}
	
	OBJReader* reader = new OBJReader();
	this->mesh = reader->read(base_path + obj_filename);

	MtlReader* mtlReader = new MtlReader();
	this->materials = mtlReader->readMaterialsFile(base_path + this->mesh->materialsFile);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	/*
	glGenBuffers(1, &vVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->getVertexes().size() * sizeof(glm::vec3), mesh->getVertexes().data(), GL_STATIC_DRAW);

	glGenBuffers(1, &nVBO);
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->getNormals().size() * sizeof(glm::vec3), mesh->getNormals().data(), GL_STATIC_DRAW);

	glGenBuffers(1, &tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->getMappings().size() * sizeof(glm::vec2), mesh->getMappings().data(), GL_STATIC_DRAW);
	*/
	for (Group* g : this->mesh->getGroups()) {
		g->setup(mesh, vVBO, nVBO, tVBO);
	}

	//for (Material* m : this->materials.begin()) {
	for (auto& it : this->materials) {
		Material* m = it.second;
		glGenTextures(1, &m->textureId);
		glBindTexture(GL_TEXTURE_2D, m->textureId);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		string filepath = base_path + m->map_kd;
		unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	shader = new Shader("vertexShader2.txt", "fragmentShader2.txt");
	shader->use();
	shader->setFloat4("inColor", 0.0f, 0.0f, 1.0f, 1.0f);
	shader->setInt("texture1", 0);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	shader->setMat4("projection", Projection);
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("model", Model);
	shader->setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
	shader->setVec3("lightPos", glm::vec3(1.2, 1.0, 4.0));

	return 0;
}


Scene::~Scene()
{
}

void Scene::draw() {

	do {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();
		// bind Texture
		shader->setMat4("view", camera->GetViewMatrix());

		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
		shader->setMat4("projection", projection);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);

		for (Group* g : mesh->getGroups()) {
			
			if (this->materials.find(g->getMaterial()) != this->materials.end()) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, this->materials[g->getMaterial()]->textureId);
			}
			//TODO bind texture before drawing
			g->draw(vVBO, nVBO, tVBO);
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

}