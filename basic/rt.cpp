//basic ray tracer demo
#include <iostream>
#include <cmath>

using namespace std;
// R3 Vectors
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
		T dotProduct(Vector3<T> &v){ return x * v.x + y * v.y + z * v.z; }; 	
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
// Sphere
class Sphere{
	public:
		//parameters 
		Vector3f center, color;
		float radius, transparency, reflectivity; 
		//member function
		bool hasIntersection(const Vector3f &rayOri, const Vector3f &rayDir, float &dOriginToHit){
			if(rayDir.getMagnitude != 1){
				rayDir.normalise();
			}
			Vector3f lOriginToCenter = center - origin;
			float dOriginToCenter = lOriginToCenter.getMagnitude();
			float dProjection =  lOriginToCenter.dotProduct(rayDir);
			//when distance from origin to center is less than the radius, the origin is inside the sphere, for current implementation it is not rendered.
			//when projection is negative, the ray points to the opposite direction, and therefore no intersection. 
			//when distance from the center to ray is larger than the radius, there is no intersection.
			if(dOrigininToCenter < radius || dProjection < 0){
				return false;
			}else{
				float dRayToCenter = sqrt(dOriginToCenter * dOriginToCenter - dProjection * dProjection);
				if(dRayToCenter > radius)
					return false;
				else{
					dOriginToHit = dProjection - sqrt(radius * radius - dRayToCenter * dRayToCenter);
					return true;
				}
			}
			
		};
}

Vector3f trace(const Vector3f &rayOri, const Vector3f &rayDir, const vector<Sphere> &sphereList, const int &depth){
	float minLength = INFINITY;
	Vector3f* hitSphere = NULL;
	for(int i = 0, length = sphereList.size() ; i < length ; i++){
		float dOriginToHit = INFINITY;
		if(sphereList[i].hasIntersection(&rayOri, &rayDir, &dOriginToHit)){
			if(minLength > dOriginToHit){
				minLength = dOriginToHit;
				hitSphere = &sphereList[i];
			}
		}
	}
	if(!hitSphere)
		return Vector3f(0); //return black;
	else{
		Vector3f hitPoint = rayOri + ( rayDir *  minLength );
		Vector3f hitNorm = (hitPoint - hitSphere -> center);
		hitNorm.normalise();
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