#ifndef GL_FRUSTRUM_H
#define GL_FRUSTRUM_H

#include "render/window.h"
#include <glm/glm.hpp>

namespace chess {

/*! Viewing Frustrum
 * "The region of space in the modeled world that may appear on screen" -
 * (Wikipedia) The viewing frustrum looks like a trapezoid parameterized by
 * the location and dimensions of its near and far spliting planes as well
 * as by the angle (field) of view.
 */ 
class Frustrum {
private:
	const Window* _window; 	// Window
	float _fov; 			// Field of view
	float _near; 			// Near plane
	float _far; 			// Far plane

public:
	Frustrum(const Window* window, float fov, float near, float far) 
		: _window(window), _fov(fov), _near(near), _far(far) {}

	/*! Projection Matrix
	 * Calculates and returns the perspective projection for this viewing
	 * frusturm. Wraps a call to the glm::perspective function.
	 * @return Perspective projection matrix
	 */
	inline glm::mat4 projection_matrix() const {
		return glm::perspective(
				(float) (_fov * M_PI / 180.0f), 
				_window->aspect_ratio(), _near, _far);
	}

};

}

#endif // GL_FRUSTRUM_H
