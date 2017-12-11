/// CMA-ES implementation
/// @author: Dominik Belter

#ifndef CMAES_H
#define CMAES_H
#include <iostream>
#include <cmath>
#include <cstdio>
#include <stdlib.h>     
#include <algorithm>
#include "time.h"
#include <string>
#include "Defs/eigen3.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include "Optimizer/optimizer.h"
#include <memory>
#include <random>

using namespace Eigen;

std::unique_ptr<Optimizer> createCMAES(void);
std::unique_ptr<Optimizer> createCMAES(std::string configFile);

class Cmaes : public Optimizer
{
	private:
        /// dimensionality of the problem
		int N;
        /// search range for each axis
        std::vector<std::pair<double,double>> range;
		double sigma;
		double stopfitness;
		double stopeval;
		int counteval;
		int eigeneval;
        /// breakIfNoProgressFor
        int breakIfNoProgressFor;
		double chiN;
		int lambda;
		int mu;
		double mueff;
		double cc, cs, c1, cmu, cmu_a, cmu_b, damps;
		double ps_norm, hsig;
        int verbose;
		
		VectorXd xmean;
        VectorXd bestSolution;
        double bestFitness;
		VectorXd weights;		
		MatrixXd arindex;
		MatrixXd arx;
		MatrixXd arfitness;
		MatrixXd arx_N_x_mu;
		VectorXd xold;
		VectorXd pc; // evolution paths for C and sigma
		VectorXd ps;
		MatrixXd B; // B defines the coordinate system
		VectorXd D; // diagonal D defines the scaling
		MatrixXd C; // covariance matrix C
        MatrixXd invsqrtC;

        MatrixXd sort(MatrixXd& table, MatrixXd& index);
		std::pair<MatrixXd, VectorXd> eig(const MatrixXd& A, const MatrixXd& B);
        void genOffspring(std::map<std::string, std::function<double(const Eigen::VectorXd&)>>  funcMap, std::string function);
		void updateXmean();
		void updateEvoPath();
		void updateMatrixC();
		void updateOtherMatrices();
		
	public:
		/// Pointer
		typedef std::unique_ptr<Cmaes> Ptr;

       	/// Construction
       	Cmaes(void);

       	/// Construction
        Cmaes(std::string configFilename);

		virtual const std::string& getName() const;
      	void setParameters();
        void Optimize(std::map<std::string, std::function<double(const Eigen::VectorXd&)>>  funcMap, std::string function);
        /// print results
		void output();
        /// get result
        void getResult(Eigen::VectorXd& xmin);
};

#endif
