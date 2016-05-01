# Deep Orange

5/5/16
- FICS Chess Database provides chess games in PGN notation from 1999-present
- Convert PGN notation games to canonical move-text format
- Construct a tree in which nodes are moves and edges connect moves by one player to all subsequent moves made by another player. Nodes are scored based on how likely they are to lead to a favorable outcome.
- The AI should not be deterministic (that would be no fun to play against!); therefore, there should be some element of randomness in move selection.
- There is a possibility that this tree will be too large to fit in memory. A smart way to store this tree might be in a database that maps moves to the list of subsequent moves.
- This smells like a Markov Chain


5/11/16
- Markov Chain implemented as mapping from states to next states
- Training data should include for each game
	- Moves
	- Outcome (1 for white win, 0 for stalemate, -1 for black win)
	- White ELO
	- Black ELO
- Transition Probability is dependent on:
	- How many favorable outcomes it leads to:
		- Sum of outcomes for all child games / Number of child games
	- How good the players are:
		- Moves by good players rated more than moves by bad ones
		- ELO weighted moves
		- If ELO is not available, default minimum ELO (2000)
	- Transition probability is a linear combination of these factors
		- Use statistics to determine optimal weights given data set
- How do we deal with State Space Explosion?
	- There are too many possible states
	- Maybe Markov Chains are not the way to go
- Minimax
- http://evjang.com/articles/genadv1
	- Discriminative Model: Evaluate conditional probability P(Y|X)
	- Generative Model: Evaluate joint probability P(X, Y)

	
