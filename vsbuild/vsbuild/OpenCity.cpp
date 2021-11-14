#include "Const.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include "CreateSkyBox.h"
#include "Element.h"
#include "Rand.h"
#include "Render.h"
#include "Process.h"
#include "Map.h"

//#define __APPLE__  // ���ʹ�� MacOS ϵͳ����Ҫ�����н��ע��
// -------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION

using namespace std;

// ��Ϸ�ӽǣ�������Ϸ���л���FPV - ��һ�ӽǣ�GPV -�ϵ��ӽǣ�
// --------------------------------------------------------
GLuint viewType = FPV;

// ************************* ȫ�ֱ��� ************************* //
// ************************************************************ //

GLfloat lastX = (GLfloat)SCR_WIDTH / 2.0f;
GLfloat lastY = (GLfloat)SCR_HEIGHT / 2.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLboolean firstMouse = true;
GLboolean dirlight = true;
GLboolean haveParticle = true;
GLboolean isParticle = false;
GLboolean isMagic = false;
GLboolean isNight = false;
GLboolean isShadow = true;
GLboolean ishdr = false;
GLboolean isgamma = false;

GLuint quadVAO = 0;
GLuint quadVBO;
GLuint screenVAO = 0;
GLuint screenVBO;
GLuint cVAO = 0;
GLuint cVBO = 0;
GLuint planeVAO;

GLuint state = PIC1;
GLuint elementCount = 0;

Particle p;
Magic ma;

Camera    camera(vec3(initX, initY, initZ));
Element * elementVector[100];

// ��Ŵ�map�ļ��ж�ȡ��λ����Ϣ
// -----------------------------
vector<vec3> boxPosition;
vec3         cupPosition;
vector<vec3> monsterPosition;

// ************************* MAIN ************************* //
// ******************************************************** // 
int main()
{
	//******************** ��ʼ���׶� ********************//
	//****************************************************//

	// ��ʼ�� glfw����������Ӧ�İ汾��Ϣ
	// ---------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // ���ʹ�� MacOS ϵͳ����Ҫ�����д�����ע��
#endif

	// ���� glfw ����
	// ----------------------------------------------------------------------------------
	GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenCity", NULL, NULL);
	if (window == NULL)
	{
		cout << "CREATE::GLFW::WHINDOW::FAILED" << endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	// �Դ���ע��ص���������Ӧ��ꡢ���̵��¼����Դ��ڽ��е���
	// ---------------------------------------------------------------
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// ���ù��ģʽΪֻ���ڴ������ұ�����
	// ---------------------------------------------------------
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ��ʼ�� glad ����ָ�룬ʹ�� glad �������� OpenGL �ĺ���ָ��
	// ----------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "INITIALIZE::GLAD::FAILED" << endl;
		exit(-1);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//******************** ���ؽ׶� ********************//
	//**************************************************//

	// ����ӽ����������
	// ----------------------------------------- 
	Camera player = (vec3(initX, initY, initZ));

	// ��ȡ��ͼ�ļ���������ͼ
	// ------------------------------------
	createMap("resource/map/map.txt");

	// ��ȡ shader �ļ�
	// ----------------------------------------------------------
	Shader boxShader("resource/vs/box.vs", "resource/fs/box.fs");
	Shader newBoxShader("resource/vs/newbox.vs", "resource/fs/newbox.fs");
	Shader lightShader("resource/vs/lightcube.vs", "resource/fs/lightcube.fs");
	Shader skyboxShader("resource/vs/skybox.vs", "resource/fs/skybox.fs");
	Shader planeShader("resource/vs/plane.vs", "resource/fs/plane.fs");
	Shader hdrShader("resource/vs/hdr.vs", "resource/fs/hdr.fs");
	Shader interfaceShader("resource/vs/interface.vs", "resource/fs/interface.fs");
	Shader depthShader("resource/vs/depth.vs", "resource/fs/depth.fs");

	// ����ģ�� obj �ļ�
	// ----------------------------------------------
	Model model1("resource/obj/airCraft/airCraft.obj");
	Model model2("resource/obj/tower/tower.obj");

	// ��������
	// ------------------------------------
	GLuint skyboxTexture = createSkyBox(0);
	GLuint otherTexture = createSkyBox(1);
	GLuint planeTexture = loadTexture("resource/material/ground2.jpg");
	GLuint cupTexture = loadTexture("resource/material/gold2.png");
	GLuint plantTexture = loadTexture("resource/material/plant5.png");
	GLuint diffuse = loadTexture("resource/planet3/diffuse.jpg");
	GLuint specular = loadTexture("resource/planet3/specular.png");
	GLuint interface = loadTexture("resource/interface/SimCity-BuildIt.jpg");

	// �� cube ��������ꡢ����������������
	// --------------------------------------
	GLuint cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// �󶨵��� light-cube ������Ϣ
	// ----------------------------
	GLuint lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// �󶨵ذ� plane ��������ꡢ����������������
	// -------------------------------------------
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);

	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// ����պ����������
	// -------------------------
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), &skyBoxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);

	GLuint hdrFBO, colorBuffer;
	glGenFramebuffers(1, &hdrFBO);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "FRAMEBUFFER::NOT_COMPLETE" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ��Ӱ shader
	// --------------------------
	GLuint depthMapFBO, depthMap;
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ����ѭ����⡢����
	// -----------------------------------
	while (!glfwWindowShouldClose(window))
	{
		mat4 projection = perspective(radians(camera.zoom), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 200.0f);
		mat4 view = camera.getViewMatrix();
		mat4 model = mat4(1.0f);
		mat4 lightProjection, lightView, lightSpaceMatrix;

		// ��Ӧ�豸�����ź�
		// ---------------------------------
		processInput(window, player, camera);

		// ��ʼ��λ���ɫ����Ϣ
		// ----------------------------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (state == PIC1)
		{
			interfaceShader.use();
			interfaceShader.setInt("pic", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, interface);
			renderScreen();

			glfwSwapBuffers(window);
			glfwPollEvents();
			continue;
		}

		if (isShadow == true)
		{
			vec3 lightPos(-3.0f, -1.0f, -3.0f);
			GLfloat nearPlane = 1.0f, farPlane = 17.5f;
			lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

			lightView = lookAt(camera.position, camera.position + camera.front, camera.up);
			lightSpaceMatrix = lightProjection * lightView;

			depthShader.use();
			depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuse);

			renderScene(depthShader);

			depthShader.use();
			for (unsigned int i = 0; i < elementCount; i++)
			{
				if (elementVector[i]->type != 1) continue;
				mat4 model1Model = mat4(1.0f);
				model1Model = translate(model1Model, elementVector[i]->position);
				model1Model = scale(model1Model, vec3(elementVector[i]->zoomRate, elementVector[i]->zoomRate, elementVector[i]->zoomRate));
				model1Model = rotate(model1Model, elementVector[i]->rotateAngle, vec3(0.0f, 1.0f, 0.0f));
				depthShader.setMat4("model", model1Model);
				model1.draw(depthShader);
			}

			for (unsigned int i = 0; i < elementCount; i++)
			{
				if (elementVector[i]->type != 2) continue;
				mat4 model2Model = mat4(1.0f);
				model2Model = translate(model2Model, elementVector[i]->position);
				model2Model = scale(model2Model, vec3(elementVector[i]->zoomRate, elementVector[i]->zoomRate, elementVector[i]->zoomRate));
				model2Model = rotate(model2Model, elementVector[i]->rotateAngle, vec3(0.0f, 1.0f, 0.0f));
				depthShader.setMat4("model", model2Model);
				model2.draw(depthShader);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (viewType == FPV)
		{
			camera.position = player.position + vec3(0.0f, 1.5f, 0.0f);
			player.yaw = camera.yaw;
			player.front = camera.front;
			player.right = camera.right;
			player.up = player.up;
		}

		// ģ�� 1 ����
		boxShader.use();

		// ƽ�й���ز���
		boxShader.setInt("dir", dirlight);
		boxShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		boxShader.setVec3("dirLight.ambient", 0.55f, 0.55f, 0.55f);
		boxShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		boxShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// ������ز���
		boxShader.setInt("material.diffuse", 0);
		boxShader.setInt("material.specular", 1);
		boxShader.setInt("material.shadowMap", 2);
		boxShader.setFloat("material.shininess", 32.0f);

		//���Դ��ز���
		boxShader.setVec3("pointLights[0].position", player.position);
		boxShader.setVec3("pointLights[0].ambient", 0.5f, 0.50f, 0.5f);
		boxShader.setVec3("pointLights[0].diffuse", 0.35f, 0.350f, 0.35f);
		boxShader.setVec3("pointLights[0].specular", 0.40f, 0.40f, 0.40f);
		boxShader.setFloat("pointLights[0].constant", 1.0f);
		boxShader.setFloat("pointLights[0].linear", 0.09);
		boxShader.setFloat("pointLights[0].quadratic", 0.032);

		// ����任��ز���
		// -----------------------------------------
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);
		boxShader.setVec3("viewPos", camera.position);
		boxShader.setVec3("lightPos", camera.position);
		boxShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		for (unsigned int i = 0; i < elementCount; i++)
		{
			if (elementVector[i]->type != 1) continue;
			mat4 model1Model = mat4(1.0f);
			model1Model = translate(model1Model, elementVector[i]->position + vec3(0.0f, 2.0f, 0.0f));
			model1Model = scale(model1Model, vec3(elementVector[i]->zoomRate, elementVector[i]->zoomRate, elementVector[i]->zoomRate));			//stoneManModel = rotate(stoneManModel, monsterVector[i]->rotateAngle, vec3(0.0f, 1.0f, 0.0f));
			boxShader.setMat4("model", model1Model);
			model1.draw(boxShader);
		}

		// ģ�� 2 ����
		// --------------------------------------------
		for (unsigned int i = 1; i < elementCount; i++)
		{
			if (elementVector[i]->type != 2) continue;
			mat4 model2Model = mat4(1.0f);
			model2Model = translate(model2Model, elementVector[i]->position);
			model2Model = scale(model2Model, vec3(elementVector[i]->zoomRate, elementVector[i]->zoomRate, elementVector[i]->zoomRate));
			//ogreModel = rotate(ogreModel, elementVector[i]->rotateAngle, vec3(0.0f, 1.0f, 0.0f));
			boxShader.setMat4("model", model2Model);
			model2.draw(boxShader);
		}

		if (isShadow == true)
		{
			// box ����
			// -------------
			boxShader.use();

			// ƽ�й���ز���
			// -------------------------------
			boxShader.setInt("dir", dirlight);
			boxShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			boxShader.setVec3("dirLight.ambient", 0.55f, 0.55f, 0.55f);
			boxShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			boxShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

			// ������ز���
			// -------------------------------------
			boxShader.setInt("material.diffuse", 0);
			boxShader.setInt("material.specular", 1);
			boxShader.setInt("material.shadowMap", 2);
			boxShader.setFloat("material.shininess", 32.0f);

			// ���Դ��ز���
			boxShader.setVec3("pointLights[0].position", player.position);
			boxShader.setVec3("pointLights[0].ambient", 0.5f, 0.80f, 0.5f);
			boxShader.setVec3("pointLights[0].diffuse", 0.35f, 0.80f, 0.35f);
			boxShader.setVec3("pointLights[0].specular", 0.40f, 0.80f, 0.40f);
			boxShader.setFloat("pointLights[0].constant", 1.0f);
			boxShader.setFloat("pointLights[0].linear", 0.09);
			boxShader.setFloat("pointLights[0].quadratic", 0.032);

			// ����任��ز���
			// -----------------------------------------
			boxShader.setMat4("projection", projection);
			boxShader.setMat4("view", view);
			boxShader.setVec3("viewPos", camera.position);
			boxShader.setVec3("lightPos", camera.position);
			boxShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			// ��cube����
			// --------------------------
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, plantTexture);

			//�����������
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specular);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glBindVertexArray(cubeVAO);

			//��������cube
			for (unsigned int i = 0; i < boxPosition.size(); i++)
			{
				//����cube��λ�ƺͷ�����ľ��󲢻���
				mat4 cubeModel = mat4(1.0f);
				cubeModel = translate(cubeModel, boxPosition[i]);
				cubeModel = scale(cubeModel, vec3(2.0, 2.0, 2.0));
				boxShader.setMat4("model", cubeModel);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (unsigned int i = 0; i < boxPosition.size(); i++)
			{
				//����cube��λ�ƺͷ�����ľ��󲢻���
				mat4 cubeModel = mat4(1.0f);
				cubeModel = translate(cubeModel, boxPosition[i]);
				cubeModel = translate(cubeModel, vec3(0.0, 2.0, 0.0));
				cubeModel = scale(cubeModel, vec3(2.0, 2.0, 2.0));
				boxShader.setMat4("model", cubeModel);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// ���� plane
		// -------------
		boxShader.use();

		// ƽ�й���ز���
		// -------------------------------
		boxShader.setInt("dir", dirlight);
		boxShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		boxShader.setVec3("dirLight.ambient", 0.55f, 0.55f, 0.55f);
		boxShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		boxShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

		// ������ز���
		// -------------------------------------
		boxShader.setInt("material.diffuse", 0);
		boxShader.setInt("material.specular", 1);
		boxShader.setFloat("material.shininess", 32.0f);

		// ���Դ��ز���
		// --------------------------------------------
		boxShader.setVec3("pointLights[0].position", player.position);
		boxShader.setVec3("pointLights[0].ambient", 0.5f, 0.5f, 0.5f);
		boxShader.setVec3("pointLights[0].diffuse", 0.3f, 0.3f, 0.3f);
		boxShader.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.5f);
		boxShader.setFloat("pointLights[0].constant", 1.0f);
		boxShader.setFloat("pointLights[0].linear", 0.09);
		boxShader.setFloat("pointLights[0].quadratic", 0.032);

		//����仯����
		boxShader.setMat4("projection", projection);
		boxShader.setMat4("model", model);
		boxShader.setMat4("view", view);
		boxShader.setVec3("viewPos", camera.position);
		boxShader.setVec3("lightPos", camera.position);
		boxShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//���ƶ�άƽ��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//����skybox
		skyboxShader.use();
		mat4 skyBoxView = mat4(mat3(camera.getViewMatrix()));
		skyboxShader.setMat4("view", skyBoxView);
		skyboxShader.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		if (isNight == false)
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		else
			glBindTexture(GL_TEXTURE_CUBE_MAP, otherTexture);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		hdrShader.setInt("hdrBuffer", 0);
		hdrShader.setInt("hdr", ishdr);
		hdrShader.setInt("gm", isgamma);
		hdrShader.setFloat("exposure", 1.0f);
		renderQuad();

		//˫���彻��
		glfwSwapBuffers(window);
		//��������λ���¼�
		glfwPollEvents();
	}

	//�ͷ��ڴ�
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);

	//����glfw
	glfwTerminate();
	return 0;
}

