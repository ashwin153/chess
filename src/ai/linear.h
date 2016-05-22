#ifndef LINEAR_H
#define LINEAR_H

#include "model.h"

namespace chess {

/*! Represents a linear regression model.
 * Linear regression models find the line-of-best-fit between some
 * independent x value and some dependent y value. This class was
 * included mostly to test the model interface and partly as a
 * baseline for accuracy and precision when building more and more
 * complicated models.
 */
class LinearRegression : public Model<double, double> {
private:
	/*! Regression Intercept. */
	double _alpha;

	/*! Regression Slope. */
	double _beta;

public:	
	void train(const std::vector<double>& x, 
			   const std::vector<double>&  y);
	double predict(const double& in);
};

} // namespace chess

#endif // LINEAR_H
