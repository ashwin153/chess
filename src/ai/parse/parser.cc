#include "parser.h"
#include "sample.h"

#include "core/game.h"

#include <thread>
#include <regex>
#include <ctime>
#include <chrono>
#include <iomanip>

namespace chess {

void Parser::parse_game() {
	// Define the regex patterns outside the while loop to save time; these regex
	// patterns will not change in the body of the loop and therefore do not need
	// to be recreated every iteration. Aggregated over thousands of iterations
	// this should save considerable cycles at runtime.
	std::regex rslt_regex("\\[Result \"(.*?)\"\\]");
	std::regex welo_regex("\\[WhiteElo \"([0-9]+)\"\\]");
	std::regex belo_regex("\\[BlackElo \"([0-9]+)\"\\]");	
	std::regex move_regex("(\\[.*?\\]|{.*}|\\d+\\.)");
	std::regex turn_regex("\\s+");

	// Typically, you want to define variables in the smallest possible scope;
	// however, the constructor and destructor will get called after every loop
	// iteration. Therefore, we can squeeze out a few cycles by pulling out the
	// declaration of loop variables.
	std::smatch match;
	Sample samp;

	// One of the challenges with the producer-consumer problem is informing
	// consumers that the producer has completed production. To solve this
	// problem, the empty string is a "poison" pill that indicates to consumers
	// that the producer has finished and that it is permissible to terminate.
	while (!_games.peek().empty()) {
		std::string pgn = _games.pop();

		// Extract white and black ratings
		std::regex_search(pgn, match, welo_regex);
		samp.white_elo = std::stoi(match[1]);
		std::regex_search(pgn, match, belo_regex);
		samp.black_elo = std::stoi(match[1]);

		// Extract game result
		std::regex_search(pgn, match, rslt_regex);
		samp.result = (match[1] == "1-0") ? 1 : (match[1] == "0-1") ? -1 : 0;

		// Extract moves
		std::string moves = std::regex_replace(pgn, move_regex, "");
		std::sregex_token_iterator it(moves.begin(), moves.end(), turn_regex, -1);
		std::sregex_token_iterator end;
		
		Game game;
		for (; it != end; ++it)
			game.make(*it);
		samp.moves = game.history();

		// Serialize sample
		_out << samp;
	}
}

void Parser::log(std::string msg) {
	auto now  = std::chrono::system_clock::now();
	auto nowt = std::chrono::system_clock::to_time_t(now); 	
	_log << std::put_time(std::localtime(&nowt), "%d/%m/%Y %X: ") << msg << "\n";
}

void Parser::parse(std::istream& in) {
}

} // namespace chess
