#include "neural.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

namespace chess {

Neuron::Neuron(int ninputs) {
	// Seed the random number generator with the current time.
	srand(time(NULL));
	for (int i = 0; i < ninputs + 1; i++)
		_weights.push_back(2.0 * rand() / RAND_MAX - 1.0);
}

double Neuron::eval(const std::vector<double>& in) {
	// Linear combination of weights and inputs plus bias
	double sum = _weights.back();
	for (int i = 0; i < inputs(); i++)
		sum += in[i] * _weights[i];

	// Fast sigmoid approximation
	return sum / (1 + std::abs(sum));
}

void NeuralNetwork::train(const std::vector<std::vector<double>>& in, 
						  const std::vector<std::vector<double>>& out) {

	/* Iteratively apply the inputs to the network in order
	 * to determine the outputs at each neuron and save these
	 * values into a forward matrix.
	 */

	/* Backpropagate errors back down the netowrk making sure
	 * to update weights along the way.
	 */	
}

std::vector<double> NeuralNetwork::predict(const std::vector<double>& in) {
	return std::vector<double>();
}

} // namespace chess
