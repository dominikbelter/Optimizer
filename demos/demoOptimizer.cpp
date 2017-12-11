#include <iostream>
#include "time.h"
#include "Optimizer/cmaes.h"
#include "Optimizer/pso.h"

double rosenbrock(const Eigen::VectorXd& x) {
    if (x.size() < 2) {
        std::cout << "dimension must be greater than one" << std::endl;
        return 0;
    }
    else {
        double f=0;// = (x(0)-2)*(x(0)-2)+4;//+(x(1)-6)*(x(1)-6);
        for (int i = 0; i < x.size() - 1; i++)
            f += 100 * (pow((pow((x(i, 0)), 2) - x(i + 1, 0)), 2)) + pow((x(i, 0) - 1), 2);
        return f;
    }
}

double quadratic(const Eigen::VectorXd& x) {
    if (x.size() != 1) {
        std::cout << "dimension should be one" << std::endl;
        return 0;
    }
    else {
        double f=2.0*pow(x(0,0)-3.0, 2.0)+0.0*x(0,0)+2.0;
        return f;
    }
}

int main() {
    try {
		tinyxml2::XMLDocument config;
        config.LoadFile("../../resources/configGlobal.xml");
		if (config.ErrorID())
			std::cout << "unable to load config file.\n";
        std::string optimizerType(config.FirstChildElement( "Optimizer" )->FirstChildElement( "name" )->GetText());
        std::string function("rosenbrock");
//        function = "quadratic";

        std::unique_ptr<Optimizer> optimizer;
        if (optimizerType == "CMAES") {
            std::string configFile(config.FirstChildElement( "Optimizer" )->FirstChildElement( "config" )->GetText());
        	optimizer = createCMAES(configFile);
        }
        else if (optimizerType == "PSO") {
            std::string configFile(config.FirstChildElement( "Optimizer" )->FirstChildElement( "config" )->GetText());
            optimizer = createPSO(configFile);
        }
       	else // Default
            optimizer = createCMAES();

        std::map<std::string, std::function<double(const Eigen::VectorXd&)>>  funcMap =
		{
            { "rosenbrock", rosenbrock},
            { "quadratic", quadratic}
		};
		
        // create objects and print configuration
        std::cout << "Current optimizer: " << optimizer->getName() << std::endl;
		std::cout << "Current function: " << function << std::endl;
        
		clock_t start, stop;
        double optTime;
		start = clock();
        optimizer->Optimize(funcMap, function);
		stop = clock();
        optTime = (double)(stop - start) / CLOCKS_PER_SEC;
		optimizer->output();

        Eigen::VectorXd xmin;
        optimizer->getResult(xmin);
        std::cout << "Cost function value: " << funcMap[function](xmin) << "\n";
        std::cout << std::endl << "Execution time: " << optTime << " s" << std::endl;

    }
    catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
