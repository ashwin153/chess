#include "parser.h"

#include <thread>
#include <regex>

namespace chess {

void Parser::parse_game(const std::string& pgn) {
	//std::regex rgx("\\s*?\\d+[.]\\s+");
	//std::sregex_token_iterator iter(pgn.begin(), pgn.end(), rgx, -1);
	//std::sregex_token_iterator end;
	//for(; iter != end; ++iter) {
	//	std::cout << *iter << '\n';
	//}
}

void Parser::parse(std::istream& in) {
}

} // namespace chess
