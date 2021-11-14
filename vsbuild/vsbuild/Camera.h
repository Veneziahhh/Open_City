#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Const.h"

enum cameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	// ������������
	// ------------------------------------
	vec3 position, front, up, right, upVec;
	
	// ŷ����
	// ----------------
	GLfloat yaw, pitch;
	
	// ���������ѡ��
	// -------------------------------------------
	GLfloat movementSpeed, mouseSensitivity, zoom;

	// ���캯��
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		upVec = up;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		position = vec3(posX, posY, posZ);
		upVec = vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}
	
	mat4 getViewMatrix()
	{
		return lookAt(position, position + front, up);
	}

	GLvoid processKeyBoard(cameraMovement direction, GLfloat deltaT, GLuint viewType);


	// ��������ƶ��������ŷ���ǵı任
	// --------------------------------
	GLvoid processMouseMovement(GLfloat x, GLfloat y, GLboolean constrainPitch = true)
	{
		x *= mouseSensitivity;
		y *= mouseSensitivity;

		yaw += x;
		pitch += y;

		// ��֤���ǲ���ƫ�����ƫС
		// ------------------------
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.f;
			else if (pitch < -89.f)
				pitch = -89.0f;
		}
		updateCameraVectors();
	}

	// zoom-in zoom-out ����ģ��
	// -------------------------
	GLvoid processMouseScroll(GLfloat y);

private:
	GLvoid updateCameraVectors();
};

#endif // !_CAMERA_H_

