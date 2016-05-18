#ifndef AI_PARSER_H
#define AI_PARSER_H

#include "blocking_queue.h"

#include <iostream>

namespace chess {

/*!
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

	/*!
	 * This function parses games in PGN and outputs the result to the
	 * out stream and any debug information (parse errors, etc.) to the log.
	 * @param[in] pgn Portable game notation movetext.
	 */ 
	void parse_game(const std::string& pgn);

public:
	/*!
	 * Constructs a parser that writes parsed data to the out stream, debug 
	 * and error information to the log stream, and uses nthreads.
	 * @param[in] out Output data stream.
	 * @param[in] log Logging stream.
	 * @param[in] nthreads Number of threads to use.
	 */
	Parser(std::ostream& out, std::ostream& log, int nthreads);

	/*!
	 * Parses the contents of the specified stream. Assumes that the stream is a
	 * list of PGN games. Parallelizes the translation from PGN games to the
	 * format understood by the core chess API.
	 * @param[in] in Input file stream.
	 */
	void parse(std::istream& in);
};

} // namespace chess

#endif // AI_PARSER_H
