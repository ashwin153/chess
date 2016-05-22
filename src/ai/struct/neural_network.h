#ifndef AI_NEURAL_NETWORK_H
#define AI_NEURAL_NETWORK_H

#include <vector>

namespace chess {

/*! Represents a single neuron in a neural network.
 * Neurons are generalized linear model machines. They evaluate the linear
 * combination of a set of weights and some inputs, add a bias term and then 
 * run the output through a differentiable, monotonic, and bounded activation
 * function (typically a sigmoid). This quantity is known as the action 
 * potential of the neuron.
 */
struct Neuron {
	std::vector<double> weights;

	/*! Construct Randomized Neuron
	 * Constructs a neuron with randomized weights that accepts ninputs. The
	 * Neuron will have ninputs + 1 weights (additional bias term).
	 */			
	Neuron(int ninputs);

	/*! Construct Predetermined Neuron
	 * Constructs a neuron with predetermined weights. The last weight in the
	 * vector is considered to be a bias weight.
	 */				 
	Neuron(std::vector<double> weights) : weights(weights) {}
						
	/*! Evaluate Action Potential
	 * Evaluates the action potential of the Neuron over the specified input
	 * terms. Performs linear combination and runs output through sigmoid.
	 */					
	double eval(const std::vector<double>& in);
};

/*! Feed-forward Neural Network.
 * Simple single-threaded MLP trained via backpropagation.
 */
class NeuralNetwork {		
private:
	std::vector<std::vector<Neuron>> _layers;
	double lrate;

public:
	NeuralNetwork();
	virtual ~NeuralNetwork();

	/*! Train Via Backpropagation */
	void train(const std::vector<std::vector<double>>& in, 
			const std::vector<std::vector<double>>& out);

	/*! Network Output */
	std::vector<double> predict(const std::vector<double>& in);
};

}

#endif // AI_NEURAL_NETWORK_H
