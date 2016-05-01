#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include "position.h"
#include "scene/mesh.h"

namespace chess {

/* Object Type
 * The type of the object is used to differentiate it from other objects at
 * rendering time. They all the rendering code to make decisions about what
 * to display based on information about the object itself.
 */
enum class Type {
	kWhitePiece,
	kBlackPiece,
	kWhiteTile,
	kBlackTile
};

/*! Object
 * Objects represent clickable and movable elements of a chess game. Both pieces 
 * and tiles are examples of chess objects. Objects also store important 
 * information that is used at rendering time to decide how to display it.
 */
class Object : public Mesh {
private:
	Position _initial;
	Type _type;

public:
	/*! Constructs an object at the specified position on the chess board.
	 * This method assumes that all objects begin at Position(0, 0) in model
	 * space. Therefore, movement of the object can be represented by simple
	 * translations between positions.
	 */
	Object(const std::string& path, const Position& initial, Type type) 
		: Mesh(path), _initial(initial), _type(type) {}

	/*! Destroys the mesh owned by this object. */
	virtual ~Object() {}

	/* Type
	 * Returns the object type of this object. This allows the object to be
	 * differentiated from other objects even though they are the same type.
	 * @return Object type
	 */
	inline Type type() const {
		return _type;
	}

	/*! Current Position
	 * Returns the current location of the object on the chess board. This method
	 * is used to communicate with the core Chess API about the movement of
	 * pieces in the game.
	 * @return Current object position
	 */
	inline virtual Position loc() const {
		return _initial;
	}

	/*! Model Matrix
	 * The model matrix for a object is defined as the translation that moves it
	 * to the correct place on the chess board.
	 * @return Model matrix
	 */
	inline glm::mat4 model_matrix() const {	
		glm::vec3 dimy = glm::vec3(0, 0, +24.5f);
		glm::vec3 dimx = glm::vec3(-24.5f, 0, 0);
		glm::vec3 tran = (float) loc().x * dimx + (float) loc().y * dimy;
		return glm::translate(Mesh::model_matrix(), tran);
	}
};

/*! Piece Object
 * This class represents an object that follows a piece around the board. Unlike
 * regular objects, pieces have a dynamic position.
 */
class PieceObject : public Object {
private:
	Piece* _piece;

public:
	PieceObject(const std::string& path, Piece* piece) 
		: Object(path, piece->loc(), (piece->owner().is_white()) ? 
				Type::kWhitePiece : Type::kBlackPiece), _piece(piece) {}

	inline Position loc() const override {
		return _piece->loc();
	}	
};
}

#endif // GL_OBJECT_H
