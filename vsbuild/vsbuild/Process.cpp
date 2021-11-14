#include "Process.h"

GLvoid framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.processMouseMovement(xoffset, yoffset);
}

GLvoid mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	//�������
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		if (isMagic == false)
		{
			isMagic = true;
			ma = initMagic(camera.position, 100, camera.front, vec3(0.8, 0.2, 0.2), 0.05);
		}
}

GLvoid scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

void processInput(GLFWwindow * window, Camera & player, Camera & camera)
{
	//������ʱ��
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	//�˳���Ϸ
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	//���
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		player.movementSpeed = ACC_VELOCITY;
		camera.movementSpeed = ACC_VELOCITY;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		player.movementSpeed = DEFAULT_VELOCITY;
		camera.movementSpeed = DEFAULT_VELOCITY;
	}

	//ǰ�������ƶ�
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (viewType == GPV)
			camera.processKeyBoard(FORWARD, (GLfloat)deltaTime, viewType);
		else if ((viewType == FPV) && collisionDetection(player.position, harryWidth / 2))
		{
			player.processKeyBoard(FORWARD, deltaTime, viewType);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (viewType == GPV)
			camera.processKeyBoard(BACKWARD, deltaTime, viewType);
		else if ((viewType == FPV) && collisionDetection(player.position, harryWidth / 2))
		{
			player.processKeyBoard(BACKWARD, deltaTime, viewType);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (viewType == GPV)
			camera.processKeyBoard(LEFT, deltaTime, viewType);
		else if ((viewType == FPV) && collisionDetection(player.position, harryWidth / 2))
		{
			player.processKeyBoard(LEFT, deltaTime, viewType);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (viewType == GPV)
			camera.processKeyBoard(RIGHT, deltaTime, viewType);
		else if ((viewType == FPV) && collisionDetection(player.position, harryWidth / 2))
		{
			player.processKeyBoard(RIGHT, deltaTime, viewType);
		}
	}

	//���ּ�123�л��ӽ�
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		viewType = FPV;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		viewType = GPV;
	}

	//ƽ�й⿪��
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		dirlight = true;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		dirlight = false;
	//��պ��л�
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		isNight = true;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		isNight = false;
	//gamma��������
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		isgamma = true;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		isgamma = false;
	//hdr��������
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		ishdr = true;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		ishdr = false;
	// ��Ӱ����
	// ----------------------------------------------
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		isShadow = true;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		isShadow = false;
	
	// ������Ϸ
	// --------
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		state = state + 1;
}


