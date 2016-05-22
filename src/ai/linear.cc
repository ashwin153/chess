#include "linear.h"

namespace chess {

void LinearRegression::train(const std::vector<double>& x, 
							 const std::vector<double>& y) {

	// Calculate the number of training data points
	int n = std::min(x.size(), y.size());

	// Calculate the mean of the x and y values
	double mx = 0.0, my = 0.0;
	for (int i = 0; i < n; i++) {
		mx += x[i] / n;
		my += y[i] / n;
	}

	// Calculate the slope of the regression line
	double xy = 0.0, xx = 0.0;
	for (int i = 0; i < n; i++) {
		xy += (x[i] - mx) * (y[i] - my);
		xx += (x[i] - mx) * (x[i] - mx);
	}
	_beta = xy / xx;

	// Calculate the intercept of the regression line
	_alpha = my - _beta * mx;
}

double LinearRegression::predict(const double& x) const {
	return _alpha + _beta * x;
}

} // namespace chess
