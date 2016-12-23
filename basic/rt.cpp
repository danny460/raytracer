
#include <cstdlib> 
#include <cstdio> 
#include <cmath>
#include <fstream> 
#include <vector> 
#include <iostream> 
using namespace std;
// 3D Vectors
template <typename T>
class Vector3 {
	public:
		//parameters
		T x,y,z;
		//constructors
		Vector3(): x(0),y(0),z(0){};
		Vector3(T val): x(val),y(val),z(val){};
		Vector3(T xVal, T yVal, T zVal): x(xVal),y(yVal),z(zVal){};
		//operators
		Vector3<T> operator+(const Vector3<T> &v ) const { return Vector3<T>( x + v.x, y + v.y, z + v.z); };	
		Vector3<T> operator+=(const Vector3<T> &v)       { x += v.x, y += v.y, z += v.z; return *this;};	
		Vector3<T> operator-(const Vector3<T> &v ) const { return Vector3<T>( x - v.x, y - v.y, z - v.z); };
		Vector3<T> operator-(					 ) const { return Vector3<T>( -x, -y, -z); };
		Vector3<T> operator*(const Vector3<T> &v ) const { return Vector3<T>( x * v.x, y * v.y, z * v.z); };
		Vector3<T> operator*(const float &f 	 ) const { return Vector3<T>( x * f, y * f, z * f); };
		T 		  dotProduct(const Vector3<T> &v ) const { return x * v.x + y * v.y + z * v.z; }; 	
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
		T getMagnitude() const{ return sqrt( x * x + y * y + z * z); }; 
};
typedef Vector3<float> Vector3f;
// Sphere
class Sphere{
	public:
		//parameters 
		Vector3f center, color, emissionColor;
		float radius; 
		// constructor
		Sphere(	const Vector3f &centerVal, 
				const Vector3f &colorVal, 
				const Vector3f &emissionColorVal, 
				const float &radiusVal) : 
				center(centerVal), color(colorVal), emissionColor(emissionColorVal), radius(radiusVal){
					//empty
		};
		//member function
		bool hasIntersection(const Vector3f &rayOri, const Vector3f &rayDir, float &dOriginToHit) const{
			Vector3f lOriginToCenter = center - rayOri;
			float dOriginToCenter = lOriginToCenter.getMagnitude();
			float dProjection =  lOriginToCenter.dotProduct(rayDir);
			//when distance from origin to center is less than the radius, the origin is inside the sphere, for current implementation it is not rendered.
			//when projection is negative, the ray points to the opposite direction, and therefore no intersection. 
			//when distance from the center to ray is larger than the radius, there is no intersection.
			if(dOriginToCenter < radius || dProjection < 0){
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
};
//Calulate pixel color
//Only work with diffuse object. 
Vector3f trace(const Vector3f &rayOri, const Vector3f &rayDir, const vector<Sphere> &sphereList, const vector<Sphere> &lightList){
	float minLength = INFINITY;
	const Sphere* hitSphere = NULL;
	Vector3f surfaceColor(0);
	for(int i = 0, length = sphereList.size() ; i < length ; i++){
		float dOriginToHit = INFINITY;
		if(sphereList[i].hasIntersection(rayOri, rayDir, dOriginToHit)){
			if(minLength > dOriginToHit){
				minLength = dOriginToHit;
				hitSphere = &sphereList[i];
			}
		}
	}
	if(!hitSphere)
		return Vector3f(1); //white background
	else{
		Vector3f hitPoint = rayOri + ( rayDir *  minLength );
		Vector3f hitNorm = hitPoint - hitSphere -> center;
		float bias = 1e-4;
		hitNorm.normalise();
		for(int i = 0, numOfLights = lightList.size(); i < numOfLights; i++){
			float transmission = 1;
			Vector3f dirToLight = lightList[i].center - hitPoint;
			dirToLight.normalise(); 
			for(int j = 0, numOfSpheres = sphereList.size(); j < numOfSpheres; j++){
				float dOriginToHit = INFINITY;
				if(sphereList[j].hasIntersection(hitPoint + hitNorm * bias, dirToLight, dOriginToHit)){
					transmission = 0;
					break;
				}
			}
			surfaceColor += hitSphere -> color * lightList[i].emissionColor * transmission * max(float(0), hitNorm.dotProduct(dirToLight));
		}
	}
	return surfaceColor + hitSphere -> emissionColor; 
}

//Output Render result to PPM image
void outputPPM(const Vector3f *image, unsigned int width, unsigned int height){
	std::ofstream ofs("./output.ppm", std::ios::out | std::ios::binary); 
	ofs << "P6\n" << width << " " << height << "\n255\n"; 
	for(int i = 0, size = width * height ; i < size ; i++){
		ofs << (unsigned char)(min(float(1), image[i].x) * 255) 
			<< (unsigned char)(min(float(1), image[i].y) * 255)
			<< (unsigned char)(min(float(1), image[i].z) * 255); 
	}
	ofs.close();
	delete [] image;
}

//Generating Camera Ray for Tracing
void render(const vector<Sphere> &sphereList, const vector<Sphere> &lightList){
	unsigned int width = 640, height = 480;
	Vector3f *image = new Vector3f[width * height];
	Vector3f *pixel = image;
	Vector3f rayOri(0);
	//camera parameters 
	float 	invWidth = 1 / float(width), 
		  	invHeight = 1 / float(height),
		  	fov = 30, 
		  	angle = tan( fov / 2 * M_PI / float(180)),
		  	aspectRatio = width * invHeight;

	//generate camera ray
	for(int y = 0 ; y < height ; y++){
		for(int x = 0 ; x < width; x++){
			float dirX = (2 * (x + 0.5) * invWidth - 1) * aspectRatio * angle;
			float dirY = (1 - 2 * (y + 0.5) * invHeight) * angle;
			Vector3f rayDir(dirX,dirY,-1);
			rayDir.normalise();
			*(pixel++) = trace(rayOri, rayDir, sphereList, lightList); 
		}
	}
	//output ppm image
	outputPPM(image , width, height);
}


int main(){
	vector<Sphere> sphereList;			
	vector<Sphere> lightList;
	//create scene
	//position,surface color, emission color, radius
	sphereList.push_back(Sphere(Vector3f( 0.0, -10004, -20), Vector3f(0.20, 0.20, 0.20), Vector3f(0), 10000 )); 
    sphereList.push_back(Sphere(Vector3f( 0.0,      0, -20), Vector3f(1.00, 0.32, 0.36), Vector3f(0),     4 )); 
    sphereList.push_back(Sphere(Vector3f( 5.0,     -1, -15), Vector3f(0.90, 0.76, 0.46), Vector3f(0),     2 )); 
    sphereList.push_back(Sphere(Vector3f( 5.0,      0, -25), Vector3f(0.65, 0.77, 0.97), Vector3f(0),     3 )); 
    sphereList.push_back(Sphere(Vector3f(-5.5,      0, -15), Vector3f(0.90, 0.90, 0.90), Vector3f(0),     3 )); 
    // light
    lightList.push_back( Sphere(Vector3f( 0.0,     20, -30), Vector3f(0.00, 0.00, 0.00), Vector3f(3), 	  0 )); 
	render(sphereList, lightList);
}