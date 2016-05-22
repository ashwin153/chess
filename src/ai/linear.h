#ifndef AI_LINEAR_H
#define AI_LINEAR_H

#include "model.h"

namespace ai {

class LinearRegression : public Model<double, double> {
private:
	double _alpha;
	double _beta;

public:
	
	void train(const std::vector<double>& x, const std::vector<double>&  y);
	double predict(const double& in);
 
};

}

#endif
