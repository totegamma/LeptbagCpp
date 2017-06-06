import core.runtime;
import std.stdio;
import std.math;
import std.json;

//Library-----------------------------------

extern (C++) {
	interface vec3{
	}
	interface quat{
	}
}

extern (C) {
	vec3 createVec3(float x, float y, float z);
	quat createQuat(float w, float x, float y, float z);
}

extern (C++) {
	interface cubeshapeObject{
		void destroy();
		float getXpos();
		float getYpos();
		float getZpos();
	}


}

extern (C) {
	cubeshapeObject cubeshape_create(vec3 position, vec3 size, quat rotation, float weight);
}

extern (C++) {
	interface hingeConstraint{
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();	
	}
}

extern (C) {
	hingeConstraint hingeConstraint_create(cubeshapeObject cubeA, cubeshapeObject cubeB, vec3 positionA, vec3 positionB, vec3 axis);
}

//------------------------------------------

wolf mywolf;


class wolf{

	cubeshapeObject[string] cubeDict;
	hingeConstraint[string] hingeDict;


	this(float x, float y, float z) {
		spawn(x, y, z);
	}



	void spawn(float x, float y, float z){
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);

		//HACK コンパイル時にjsonStringにsimpleWolf.fpmの内容が代入される(要-Jオプション)
		auto jsonString = import("simpleWolf.fpm");

		JSONValue model = parseJSON(jsonString);

		foreach(elem; model.array){
			if(elem["objectType"].str == "MESH"){
				if(elem["rigidBodyShape"].str == "BOX"){
					cubeDict[elem["name"].str] = cubeshape_create(
													createVec3(elem["xpos"].floating + x, elem["ypos"].floating + y, elem["zpos"].floating + z),
													createVec3(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating),
													createQuat(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating),
													elem["mass"].floating);
				}
			}
		}

		foreach(elem; model.array){
			if(elem["objectType"].str == "constraint"){
				float xrot = elem["xrot"].floating;
				float yrot = elem["yrot"].floating;
				float zrot = elem["zrot"].floating;
				hingeDict[elem["name"].str] = hingeConstraint_create(cubeDict[elem["object1"].str], cubeDict[elem["object2"].str],
																		createVec3(elem["object1xpos"].floating, elem["object1ypos"].floating, elem["object1zpos"].floating),
																		createVec3(elem["object2xpos"].floating, elem["object2ypos"].floating, elem["object2zpos"].floating),
																		createVec3(	-sin(xrot)*sin(zrot) + cos(xrot)*cos(yrot)*cos(zrot),// デフォルトではy軸方向ヒンジ
																					-sin(yrot)*cos(zrot),                                // ベクトルの回転でどの向きになるか
																					cos(xrot)*sin(zrot) + sin(xrot)*cos(yrot)*cos(zrot)));// 計算する。
				if(elem["useLimit"].str == "True"){
					hingeDict[elem["name"].str].setLimit(elem["limitLower"].floating, elem["limitUpper"].floating);
				}
			}
		}



	}

	void move(int sequence){

	}


	void despawn(){

	}
}


//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	mywolf = new wolf(0, 1.5, -5);
}


extern (C) void tick(){
}



//------------------------------------------
