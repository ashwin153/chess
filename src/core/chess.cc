#include "chess.h"

namespace chess {

Chess::Chess() {
	_white = new Player(true);
	_black = new Player(_white);	
}

Chess::~Chess() {
	delete _white;
	delete _black;
	_white = nullptr;
	_black = nullptr;
}

}
