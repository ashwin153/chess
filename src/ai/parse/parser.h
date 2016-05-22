#ifndef AI_PARSER_H
#define AI_PARSER_H

#include "../struct/blocking_queue.h"
#include <iostream>

namespace chess {

/*! Parser
 * This class parses PGN games from an input stream and writes their contents
 * to an output stream. This class is used to translate the massive PGN file
 * all-games.pgn into a format that the core chess API can comprehend.
 */
class Parser {
private:
	BlockingQueue<std::string> _games;
	std::ostream& _log;
	std::ostream& _out;
	int _nthreads;

	/*! Parse Game Thread
	 * This function parses games in PGN and outputs the result to the
	 * out stream and any debug information (parse errors, etc.) to the log.
	 */ 
	void parse_game(const std::string& pgn);

public:
	/*! Construct Parser
	 * Constructs a parser that writes parsed data to the out stream, debug 
	 * and error information to the log stream, and uses nthreads.
	 */
	Parser(std::ostream& out, std::ostream& log, int nthreads);


	/*! Parse File
	 * Parses the contents of the specified stream. Assumes that the stream is a
	 * list of PGN games. Parallelizes the translation from PGN games to the
	 * format understood by the core chess API.
	 * @param[in] in Input stream
	 */
	void parse(std::istream& in);
};

}

#endif // AI_PARSER_H
