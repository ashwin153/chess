#ifndef GAME_PIECE_H_
#define GAME_PIECE_H_

namespace chess {

class Piece {
private:

public:
	virtual ~Piece();
	
};

class Pawn : public Piece {

};

class Knight : public Piece {

};

class Bishop : public Piece {

};

class Rook : public Piece {

};

class Queen : public Rock, public Bishop {

};

class King : public Piece {

};
 
} // namespace chess

#endif // GAME_PIECE_H_
