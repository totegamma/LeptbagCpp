import std.stdio;
import std.conv;
import std.random;



double[][] diffEvo(double[][] parents, double Cr, double F){

	double[][] children = parents;

	auto rnd = Random(unpredictableSeed);

	for(uint i=0; i<parents.length; i++){
		for(uint j=0; j<parents[0].length; j++){


			if( Cr > uniform(0.0L, 1.0L, rnd) ){
				uint k, l, m;
				k = uniform(0, to!int(parents.length), rnd);
				l = uniform(0, to!int(parents.length), rnd);
				m = uniform(0, to!int(parents.length), rnd);

				children[i][j] = parents[k][j] + F * (parents[l][j] - parents[m][j]);
			}

		}
	}

	return children;

}

/*
void main(){
	
	int numOfAgent = 10;
	int numOfGene = 5;

	double[][] parents;
	double[][] children;

	parents.length = numOfAgent;

	auto rnd = Random(unpredictableSeed);
	for(uint i=0; i<numOfAgent; i++){
		parents[i].length = numOfGene;
		for(uint j=0; j<numOfGene; j++){
			parents[i][j] = uniform(0.0L, 1.0L, rnd);
		}
	}

	writeln(parents);

	children = diffEvo(parents, 0.5, 0.5);

	writeln(children);

}

*/
