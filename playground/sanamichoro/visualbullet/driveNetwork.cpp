#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include </usr/local/include/eigen3/Eigen/Core>


class driveNetwork{

	public:

	Eigen::MatrixXf b1 = Eigen::MatrixXf::Random(1, 8);
	Eigen::MatrixXf hidden = Eigen::MatrixXf::Random(8, 12);
	Eigen::MatrixXf b2 = Eigen::MatrixXf::Random(1, 12);
	Eigen::MatrixXf output = Eigen::MatrixXf::Random(12, 8);

	double probOfCross = 0.5;
	double probOfMutation = 0.1;
	double F = 0.5;
	double Cr = 0.4;

	driveNetwork(){

	}

	driveNetwork(driveNetwork mother, driveNetwork father){
	}

	Eigen::MatrixXf forward(Eigen::MatrixXf input){

		input = input + b1;
		input = input * this->hidden;
		input = input.array().tanh().matrix();
		input = input + b2;
		input = input * this->output;

		return input;

	}

	void cross(driveNetwork first, driveNetwork second){

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<double> crossRate(0, 1.0);
		std::uniform_int_distribution<double> mutateRate(0, 1.0);

		Eigen::MatrixXf b1_cross = Eigen::MatrixXf::Random(1, 8);
		Eigen::MatrixXf hidden_cross = Eigen::MatrixXf::Random(8, 12);
		Eigen::MatrixXf b2_cross = Eigen::MatrixXf::Random(1, 12);
		Eigen::MatrixXf output_cross = Eigen::MatrixXf::Random(12, 8);

		for(int i=0; i<8; i++){
			if( b1_cross(1, i) > this->probOfCross) this->b1(1, i) = first.b1(1, i);
			else this->b1(1, i) = second.b1(1, i);
			if(b1_cross(1, i) < probOfMutation) this->b1(1, i) = b1_cross(1, i);
		}

		for(int i=0; i<12; i++){
			for(int j=0; j<8; j++){
				if( hidden_cross(i, j) > this->probOfCross) this->hidden(j, i) = first.hidden(j, i);
				else this->hidden(j, i) = second.hidden(j, i);
				if(hidden_cross(i, j) < probOfMutation) this->hidden(i, j) = hidden_cross(i, j);
			}
		}

		for(int i=0; i<12; i++){
			if( b1_cross(1, i) > this->probOfCross) this->b2(1, i) = first.b2(1, i);
			else this->b2(1, i) = second.b2(1, i);
			if(b2_cross(1, i) < probOfMutation) this->b2(1, i) = b2_cross(1, i);
		}

		for(int i=0; i<8; i++){
			for(int j=0; j<12; j++){
				if(output_cross(j, i) > this->probOfCross) this->output(j, i) = first.output(j, i);
				else this->output(j, i) = second.output(j, i);
				if(output_cross(j, i) < probOfMutation) this->output(j, i) = output_cross(j, i);
			}
		}

	}

};

