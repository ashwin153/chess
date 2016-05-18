#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include "tracer/ray.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>

namespace chess {

/*! 
 * This class represents a first-person shoot (FPS) camera. The camera can be 
 * zoomed, paned, rolled, and rotated and the matrix transforming world 
 * coordinates to camera coordinates or view matrix can be calculated at any 
 * point in time.
 */
class Camera {
private:
	glm::vec3 _eye;
	glm::vec3 _look;
	glm::vec3 _up;
	glm::vec3 _tangent;

	float _zoom_speed;
	float _pan_speed;
	float _rotate_speed;

public:
	Camera(glm::vec3 eye, float zs, float ps, float rs);

	/*!
	 * Returns the view matrix induced by the camera position and coordinate
	 * system. This matrix is used at render time to move from world->eye.
	 * @return View matrix.
	 */
	inline glm::mat4 view_matrix() const {
		return glm::lookAt(_eye, _eye + _look, _up);
	}

	/*!
	 * Returns the ray through the specified normalized window point.
	 * @param[in] x Normalized window x [0, 1]
	 * @param[in] y Normalized window y [0, 1]
	 * @return Ray through window point.
	 */
	inline Ray ray(float x, float y) {
		// Move x and y to [-0.5, 0.5]
		x -= 0.5f;
		y -= 0.5f;
	
		// Calculate the ray through the point
		return Ray(_eye, glm::normalize(_look + x * _tangent + y * _up));
	}

	/*!
	 * Returns the current position of the camera in space.
	 * @return Position of camera.
	 */
	inline glm::vec3 eye() const {
		return _eye;
	}

	/*!
	 * Zooms the camera in or out depending on the sign of the input parameter.
	 * The magnitude of the zoom is proportional to the size of i.
	 * @param[in] i Zoom factor.
	 */
	void zoom(int i);

	/*!
	 * Pans the camera right or left depending on the sign of the input 
	 * parameter. The magnitude of the pan is proportional to the size of i.
	 * @param[in] i Pan factor.
	 */
	void pan(int i);

	/*!
	 * Rolls the camera clockwise or couter-clockwise depending on the sign of
	 * the input parameter. The magnitude of the roll is proportional to the
	 * size of i. Rolls around the look direction.
	 * @param[in] i Roll factor.
	 */
	inline void roll(int i) {
		rotate(i, _look);
	}

	/*!
	 * Rotates the camera along the vector in world coordinates.
	 * @param[in] i Rotate factor.
	 * @param[in] vector Rotation vector.
	 */
	void rotate(int i, glm::vec3 vector);

};

} // namespace chess

#endif // GL_CAMERA_H
