#include <iostream>
#include </usr/local/include/eigen3/Eigen/Core>

using namespace std;

class matrixXf{

	public:
		Eigen::MatrixXf matrix;


		static Eigen::MatrixXf multiple(Eigen::MatrixXf first, Eigen::MatrixXf second){
			return first*second;
		}

		virtual void showNeko(){
			std::cout<<"neko"<<std::endl;
		}

		virtual void initRand(int i, int j){
			this->matrix = Eigen::MatrixXf::Random(i, j);
		}

		virtual void show(){
			std::cout<<matrix<<std::endl;
		}

		virtual matrixXf mult(matrixXf a){
			matrixXf result;
			result.matrix = multiple(this->matrix, a.matrix);
			return result;
		}

		virtual matrixXf bumabuma(){
			matrixXf result;
			result.initRand(2, 3);
			return result;
		}

};




matrixXf *getmatrixXf(){
	matrixXf *nyaon = new matrixXf();
	return nyaon;
}

