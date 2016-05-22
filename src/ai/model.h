#ifndef MODEL_H
#define MODEL_H

#include <vector>

namespace chess {

/*! Generic superclass for all statistical/ML models
 * This class is parameterized by an input and output argument type.
 * The goal is to create models whose public interface is so robust,
 * that they can be easily swapped in the code with minimial configuration
 * necessary after they have been initialized.
 */
template<typename I, typename O>
class Model {
public:
	/*! Trains the network on the specified input output data. 
	 * The size of the input and output arrays must be the same.
	 * Successive calls to train may or may not override existing
	 * model parameters.
	 * @param[in] in Input data
	 * @param[in] out Output data
	 */
	virtual void train(const std::vector<I>& in, const std::vector<O>& out) = 0;
	
	/*! Predicts the values of the input data using the model.
	 * @param[in] in Input data
	 * @return Predicted values
	 */
	virtual O predict(const I& in) = 0;
};

} // namespace chess

#endif // MODEL_H
