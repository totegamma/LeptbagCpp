import std.stdio;
import std.conv;
import std.random;
import chorodog;

Random rnd;


chorodog[] diffEvo(chorodog[] parents, float Cr, float F){


	chorodog[] children;
	children.length = parents.length;
	foreach(int i, ref child; children) child = new chorodog(to!float(i)*5.0f, 0.0f, 0.0f, true);

	auto rnd = Random(unpredictableSeed);


	foreach(int j, child; children){


			uint k, l, m;
			k = uniform(0, to!int(parents.length), rnd);
			l = uniform(0, to!int(parents.length), rnd);
			m = uniform(0, to!int(parents.length), rnd);



			foreach(int i, dnas; child.dna ){
				foreach(string s, eachdna; dnas){
					if( Cr > uniform(0.0f, 1.0L, rnd) ){
						eachdna = parents[k].dna[i][s] + F * (parents[l].dna[i][s] - parents[m].dna[i][s]);
					}else{
						eachdna = parents[j].dna[i][s];
					}
				}
			}

	}


	return children;
}
