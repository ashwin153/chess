#include "game.h"

#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace chess {

Game::Game() : _turn(0) {
	_white = new Player(true);
	_black = new Player(_white);	
}

/*Game::Game(const std::string& pgn) {
	std::regex reg_numb("\\s*?\\d+[.]\\s+");
	
	std::sregex_token_iterator iter(pgn.begin(), pgn.end(), reg_numb, -1);
	std::sregex_token_iterator end;
	for ( ; iter != end; ++iter) {
		// Deal with each move
		std::cout << *iter << '\n';

		// Special Cases: Pawn Promotion, Castling, and En Passant
		// Throw exception for unhandled cases (Pawn Promotion to non-queen)
		 
	}
}*/

// Pass in the Board State so we don't have to step over and over again.
// Should make the AI considerably faster.
Game::Game(std::vector<Move> moves) : Game() {
   	_history = moves;
	step(moves.size());	
}

Game::~Game() {
	delete _white;
	delete _black;
	_white = nullptr;
	_black = nullptr;
}

void Game::step(int times) {
	for (int i = 0; i < times && _turn < _history.size(); i++) {	
		next()->make(_history[_turn]);
		_turn++;
	}

	// Pre-computing the valid set makes checking move validity O(log n),
	// because we only have to generate the valid set once for a given turn.
	_valid = moves<Piece>();
}

void Game::back(int times) {
	for (int i = 0; i < times && _turn >= 0; i++) {
		next()->undo();
		_turn--;
	}

	// Pre-computing the valid set makes checking move validity O(log n),
	// because we only have to generate the valid set once for a given turn.
	_valid = moves<Piece>();
}

bool Game::make(const Move& move) {
	if (_valid.end() == _valid.find(move))
		return false;

	_history.push_back(move);
	_history.erase(_history.begin() + _turn, _history.end());
	_history.push_back(move);	
	step(1);
	return true;
}

bool Game::make(const std::string& pgn) {
	std::regex rgx("^([PNBRQK])?([a-h])?([1-8])?[x]?([a-h][1-8])=?([NBRQ])?$");	
	std::smatch result;
		
	if (pgn == "O-O") {
		// Kingside Castle
		for (auto move : moves<King>())
			if (move.type == MoveType::kCastleKingside)
				return make(move);
	} else if (pgn == "O-O-O") {
		// Queenside Castle
		for (auto move : moves<King>())
			if (move.type == MoveType::kCastleQueenside)
				return make(move);
	} else if (regex_match(pgn, result, rgx)) {
		// Regular Move
		std::string piece   = result[1].str();
		std::string sfile   = result[2].str();
		std::string srank   = result[3].str();
		Position next       = Position(result[4].str());
		std::string promote = result[5].str();

		// Find Valid Moves		
		std::set<Move> valid;
		if (piece.empty() || piece == "P")
			valid = moves<Pawn>();
		else if (piece == "N")
			valid = moves<Knight>();
		else if (piece == "B")
			valid = moves<Bishop>();
		else if (piece == "R")
			valid = moves<Rook>();
		else if (piece == "Q")
			valid = moves<Queen>();
		else if (piece == "K")
			valid = moves<King>();

		// Find Pawn Promotion Type
		MoveType promote_type;
		if (promote.empty() || promote == "Q")
			promote_type = MoveType::kPromoteQueen;
		else if (promote == "N")
			promote_type = MoveType::kPromoteKnight;
		else if (promote == "B")
			promote_type = MoveType::kPromoteBishop;
		else if (promote == "R")
			promote_type = MoveType::kPromoteRook;

		// Filter Moves
		for (auto move : valid)
			if (move.nxt == next && 
					(sfile.empty() || move.cur.file() == sfile[0]) &&
					(srank.empty() || move.cur.rank() == std::stoi(srank)) &&
					(promote.empty() || move.type == promote_type))
				return make(move);
	}

	return false;
}

std::string Game::to_string() const {
	// Build up a textual version of the game
	std::string text;
	for (int x = 0; x < 8; x++) {
		text += std::to_string(8 - x);
		for (int y = 0; y < 8; y++) {
			Piece* wpiece = _white->piece(Position(x, y));
			Piece* bpiece = _black->piece(Position(x, y));
			if (!wpiece && !bpiece) text += " ―";
			else if (wpiece) text += " " + wpiece->to_string();
			else if (bpiece) text += " " + bpiece->to_string();
		}	
		text += "\n";
	}
	
	text += " ";
	for (char i = 'a'; i <= 'h'; i++) {
		text += ' ';
		text += i;
	}

	// Return the textual representation
	return text + "\n";
}

}
