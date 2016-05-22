#ifndef NEURAL_H
#define NEURAL_H

#include "model.h"
#include <vector>

namespace chess {

/*! Represents a single neuron in a neural network.
 * Neurons are linear combination machines. They evaluate the linear
 * combination of a set of weights and some inputs, add a bias term
 * and then run the output through a differentiable, monotonic, and 
 * bounded function (typically a sigmoid). This quantity is known
 * as the action potential of the neuron.
 */
struct Neuron {
	std::vector<double> weights;

	/*! Create a randomized neuron that accepts ninputs. */
	Neuron(int ninputs);

	/*! Creates a neuron with the specified weights.
	 * The last weight is considered to be the bias weight;
	 */
	Neuron(std::vector<double> weights) : weights(weights) {}
	
	/*! Evaluates the action potential of the input. */
	double eval(const std::vector<double>& in);
};

/*! Represents a feed-forward neural network.
 * Simple single-threaded feed-forward neural network trained
 * via backpropagation.
 */
class NeuralNetwork : public Model<std::vector<double>, std::vector<double>> {
private:
	std::vector<std::vector<Neuron>> _layers;
	double lrate;

public:
	NeuralNetwork();
	virtual ~NeuralNetwork();

	/*! Trains the network via backpropagation. */
	void train(const std::vector<std::vector<double>>& in, 
			   const std::vector<std::vector<double>>& out);

	/*! Predicts the values of the network. */
	std::vector<double> predict(const std::vector<double>& in);
};

} // namespace chess

#endif // NEURAL_H
