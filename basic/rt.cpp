//basic ray tracer demo
#include <iostream>
#include <cmath>

using namespace std;

template <typename T>
class Vector3<T> {
	public:
		//parameters
		T x,y,z;
		//constructors
		Vector3(): x(0),y(0),z(0){};
		Vector3(T val): x(val),y(val),z(val){};
		Vector3(T xVal, T yVal, T zVal): x(xVal),y(yVal),z(zVal){};
		//operators
		Vector3<T> operator+(Vector3<T> &v){ return Vector3<T>( x + v.x, y + v.y, z + v.z)};	
		Vector3<T> operator-(Vector3<T> &v){ return Vector3<T>( x - v.x, y - v.y, z - v.z)};
		Vector3<T> operator-(){ return Vector3<T>( -x, -y, -z)};
		Vector3<T> operator*(Vector3<T> &v){ return Vector3<T>( x * v.x, y * v.y, z * v.z)};
		Vector3<T>& operator+=(Vector3<T> &v){ x + v.x, y + v.y, z + v.z; return *this};	
		//member functions
		Vector3& normalise(){
			T magnitude = getMagnitude();
			if(magnitude > 0){
				T multiplier = 1/magnitude;
				x *= multiplier;
				y *= multiplier;
				z *= multiplier;
			}
			return *this;
		};
		T distance(const Vector3f &v){ Vector3<T> line = this - Ve }

		T getMagnitude(){ return sqrt( x * x + y * y + z * z); }; 
};
typedef Vector3<float> Vector3f;

class Sphere{
	public:
		//parameters 
		Vector3f center, color;
		float radius, transparency, reflectivity; 
		//member function
		bool hasIntersection(const Vector3f &rayOri, const Vector3f &rayDir){

		}
}






int main(){
	const int 	imageHeight = 640, 
				imageWidth = 960;

	for(int i = 0 ; i < imageHeight; i++){
		for(int j = 0; j < imageWidth; j++){
			
		}
	}			
}