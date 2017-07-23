import std.stdio;


extern (C++) interface matrixXf{
	void showNeko();
	void initRand(int i, int j);
	void show();
	matrixXf mult(matrixXf a);
	matrixXf bumabuma();
}
extern (C++) matrixXf getmatrixXf();



void main(){

	matrixXf neko;
	neko = getmatrixXf();
	writeln("make neko done");
	neko.initRand(2, 2);
	writeln("initRand done");
	neko.show();
	writeln("neko show done");
	matrixXf nago;
	nago = getmatrixXf();
	nago.initRand(2, 2);
	nago.show();
	writeln("nago show done");
	nago = nago.mult(neko);
	nago.show();
	writeln("mult done");


}
