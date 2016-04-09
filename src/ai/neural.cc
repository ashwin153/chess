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
	for (int i = 0; i < _weights.size() - 1; i++)
		sum += in[i] * _weights[i];

	// Fast sigmoid approximation
	return sum / (1 + std::abs(sum));
}

NeuralNetwork::NeuralNetwork() {

}

NeuralNetwork::~NeuralNetwork() {

}

void NeuralNetwork::train(const std::vector<std::vector<double>>& in, 
						  const std::vector<std::vector<double>>& out) {

	for (int n = 0; n < in.size(); n++) {
		// Iteratively apply the inputs to the network in order
		// to determine the outputs at each neuron and save these
		// values into a forward matrix.
		std::vector<std::vector<double>> outputs;
		outputs.push_back(in[n]);
		for (int i = 0; i < _layers.size(); i++) {
			std::vector<double> output;
			for (int j = 0; j < _layers[i].size(); j++)
				output.push_back(_layers[i][j].eval(outputs[i]));
			outputs.push_back(output);
		}

		// Backpropagate errors back down the netowrk making sure
		// to update weights along the way.
		std::vector<std::vector<double>> errors;
		for (int i = _layers.size() - 1; i >= 0; i--) {
			std::vector<double>> error;

			for (int j = 0; j < _layers[i].size(); j++) {
				if (i == _layers.size() - 1) {
					// If the neuron is an output neuron, then the error
					// is determined by the deviation from the expected
					// value provided by out[n].
					error.push_back(outputs[i+1][j] * 
						(1-outputs[i+1][j]) * (out[n][j]-outputs[i+1][j]));
				} else {
					// Otherwise, the error is specified by the backpropation
					// algorithm. Wrote this a long time ago, no idea how it works.
					double sigma = 0.0;
					for (int k = 0; k < _layers[i+1].size(); k++)
						sigma += _layers[i+1][k].weights[j] * errors[i+1][k];
					error.push_back(outputs[i+1][j] * (1-outputs[i+1][j]) * sigma);
				}
		
				// Update the weights of the neuron. Higher lrate means faster
				// training, but it makes the weights and objective function
				// diverge which could inhibit learning. Better algorithms like
				// Quickprop and RPROP use dynamic learning rates, but this
				// should be good enough for now.
				std::vector<double> delta;
				for (int k = 0; k < _layers[i][j].weights.size(); k++)
					delta.push_back((k == weights.size() - 1) ?
						_layers[i][j].weights[k]+lrate*errors[i][j] :
						_layers[i][j].weights[k]+lrate*errors[i][j]*outputs[i][k]);
				_layers[i][j].weights = delta;
			}
	
			errors.push_back(error);
		}
	}
}

std::vector<double> NeuralNetwork::predict(const std::vector<double>& in) {
	return std::vector<double>();
}

} // namespace chess
