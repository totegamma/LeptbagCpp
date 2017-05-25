#include <iostream>
#include <math.h>
#include <deque>

#include "Eigen/Core"

class qNetwork{
	public:
		int hiddenLayer1, hiddenLayer2;
		Eigen::MatrixXf W1,W2,W3;
		Eigen::VectorXf b1,b2,b3;
		Eigen::MatrixXf grad[6];
		//遅延評価のために出力したQのセットの中の最大値，およびそのときの重みとバイアスを遅延区間分保存しておく
		std::deque<Eigen::MatrixXf> input;
		std::deque<Eigen::MatrixXf> qout;
		std::deque<Eigen::MatrixXf> weight[3];
		std::deque<Eigen::VectorXf> bias[3];
		double learningRate = 0.01; //学習率


		qNetwork( int inputSize, int hiddenLayer1, int hiddenLayer2){
			this->hiddenLayer1 = hiddenLayer1;
			this->hiddenLayer2 = hiddenLayer2;
			this->W1 = Eigen::MatrixXf::Random(inputSize, hiddenLayer1);
			this->b1 = Eigen::VectorXf::Random(hiddenLayer1);
			this->W2 = Eigen::MatrixXf::Random(hiddenLayer1, hiddenLayer2);
			this->b2 = Eigen::VectorXf::Random(hiddenLayer2);
			this->W3 = Eigen::MatrixXf::Random(hiddenLayer2, 1);
			this->b3 = Eigen::VectorXf::Random(1);
		}

		qNetwork(Eigen::MatrixXf w1, Eigen::MatrixXf w2, Eigen::MatrixXf w3, 
				Eigen::MatrixXf b1,Eigen::MatrixXf b2, Eigen::MatrixXf b3){
			this->hiddenLayer1 = w1.cols();
			this->hiddenLayer2 = w2.cols();
			this->W1 = w1;
			this->b1 = b1;
			this->W2 = w2;
			this->b2 = b2;
			this->W3 = w3;
			this->b3 = b3;
		}


		Eigen::MatrixXf forward(Eigen::MatrixXf input){

			this->input.push_front(input);
			//std::cout<<this->W1<<std::endl<<std::endl;

			input = input * this->W1;
			//std::cout<<input<<std::endl<<std::endl;
			input = input.rowwise() + this->b1.transpose();
			input = input.array().tanh().matrix();
			input = input * this->W2;
			input = input.rowwise() + this->b2.transpose();
			input = input.array().tanh().matrix();
			input = input * this->W3;
			input = input.rowwise() + this->b3.transpose();

			this->qout.push_front(input); //Qは保存しておく
			//重みを保存しておく
			this->weight[0].push_front( this->W1 );
			this->weight[1].push_front( this->W2 );
			this->weight[2].push_front( this->W3 );
			this->bias[0].push_front( this->b1 );
			this->bias[1].push_front( this->b2 );
			this->bias[2].push_front( this->b3 );


			return input;

		}


		Eigen::MatrixXf gradTanh(Eigen::MatrixXf input, Eigen::MatrixXf dout){
			input = 2.0 * input;

			Eigen::MatrixXf plus = input.array().exp() + 1;

			return 4.0 * plus.array().inverse() * dout.array();


		}


		void gradient(Eigen::MatrixXf weightSet[], Eigen::VectorXf biasSet[],
				Eigen::MatrixXf data, Eigen::MatrixXf dout){


			Eigen::MatrixXf prePara[2];
			prePara[0] = (data * weightSet[0]).rowwise() + biasSet[0].transpose();
			prePara[1] = ( prePara[0].array().tanh().matrix() * weightSet[1] ).rowwise()
						+ biasSet[1].transpose();

			this->grad[0] = dout.colwise().sum();
			this->grad[1] = prePara[1].array().tanh().matrix().transpose() * dout;
			this->grad[2] = this->gradTanh( prePara[1], dout*weightSet[2].transpose() ).colwise().sum().transpose();
			this->grad[3] = prePara[0].array().tanh().matrix().transpose() * this->gradTanh( prePara[1], dout*weightSet[2].transpose() );
			this->grad[4] = this->gradTanh( prePara[0], this->gradTanh(prePara[1], dout*weightSet[2].transpose()) * weightSet[1].transpose() ).colwise().sum().transpose();
			this->grad[5] = data.transpose() * this->gradTanh( prePara[0], this->gradTanh(prePara[1], dout*weightSet[2].transpose()) * weightSet[1].transpose() );



		}

		//中間ファイルを生成しないようにするために式が長いのでここを読みたかったらchoroから計算グラフをもらってください
		void backward(Eigen::MatrixXf dout){


			Eigen::MatrixXf inputData = this->input.back();

			Eigen::MatrixXf w[3];
			Eigen::VectorXf b[3];

			for(int i=0; i<3; i++){
				w[i] = this->weight[i].back();
				b[i] = this->bias[i].back();
			}


			this->gradient(w, b, inputData, dout);


			this->b3 = this->b3 - learningRate * this->grad[0];
			this->W3 = this->W3 - learningRate * this->grad[1];
			this->b2 = this->b2 - learningRate * this->grad[2];
			this->W2 = this->W2 - learningRate * this->grad[3];
			this->b1 = this->b1 - learningRate * this->grad[4];
			this->W1 = this->W1 - learningRate * this->grad[5];
			//std::cout<<this->gradTanh( prePara[0], this->gradtan * this->weight[1].back().transpose() )<<std::endl<<std::endl;


			this->qout.pop_back();
			this->input.pop_back();
			for(int w=0; w<3; w++) this->weight[w].pop_back();
			for(int b=0; b<3; b++) this->bias[b].pop_back();

		}





};
