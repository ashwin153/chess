#ifndef AI_NEURAL_NETWORK_H
#define AI_NEURAL_NETWORK_H

#include <vector>

namespace chess {

/*!
 * Neurons are generalized linear model machines. They evaluate the linear
 * combination of a set of weights and some inputs, add a bias term and then 
 * run the output through a differentiable, monotonic, and bounded activation
 * function (typically a sigmoid). This quantity is known as the action 
 * potential of the neuron.
 */
struct Neuron {
	std::vector<double> weights;

	/*!
	 * Constructs a neuron with randomized weights that accepts ninputs. The
	 * Neuron will have ninputs + 1 weights (additional bias term).
	 * @param[in] ninputs Number of inputs to neuron.
	 */			
	Neuron(int ninputs);

	/*!
	 * Constructs a neuron with predetermined weights. The last weight in the
	 * vector is considered to be a bias weight.
	 * @param[in] weights Weight vector.
	 */				 
	Neuron(std::vector<double> weights) : weights(weights) {}
						
	/*!
	 * Evaluates the action potential of the Neuron over the specified input
	 * terms. Performs linear combination and runs output through sigmoid.
	 * @param[in] in Input values.
	 * @return Action potential.
	 */					
	double eval(const std::vector<double>& in);
};

/*!
 * This class represents a simple, single-threaded multilayer perceptron. This
 * feed-forward neural network is trained via backpropagation of errors.
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
