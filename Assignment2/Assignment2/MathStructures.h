#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

struct Vector2 {
	float x, y;

	Vector2(float xValue = 0.0f, float yValue = 0.0f) {
		x = xValue;
		y = yValue;
	}

	bool operator==(const Vector2& other) {
		return (this->x == other.x && this->y == other.y);
	}

	bool operator!=(const Vector2& other) {
		return !(this->x == other.x && this->y == other.y);
	}

	Vector2 operator+(const Vector2& other) {
		return Vector2(this->x + other.x, this->y + other.y);
	}

	Vector2 operator-(const Vector2& other) {
		return Vector2(this->x - other.x, this->y - other.y);
	}

	Vector2 operator*(const Vector2& other) {
		return Vector2(this->x * other.x, this->y * other.y);
	}

	Vector2 operator/(const Vector2& other) {
		return Vector2(this->x / other.x, this->y / other.y);
	}
};

struct Vector3 {
	float x, y, z;

	Vector3(float xValue = 0.0f, float yValue = 0.0f, float zValue = 0.0f) {
		x = xValue;
		y = yValue;
		z = zValue;
	}

	bool operator==(const Vector3& other) const {
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}

	bool operator!=(const Vector3& other) const {
		return !(this->x == other.x && this->y == other.y && this->z == other.z);
	}

	Vector3 operator+(const Vector3& other) {
		return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	Vector3 operator-(const Vector3& other) {
		return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	Vector3 operator*(const Vector3& other) {
		return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
	}

	Vector3 operator/(const float& other) const {
		return Vector3(this->x / other, this->y / other, this->z / other);
	}

	Vector3 operator*(const float& scalar)  {
		return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
	}

	void Normalise() {
		float length = Length();
	}

	float LengthSq() const {
		return(x * x + y * y + z * z);
	}

	float Length() const {
		return(sqrt(LengthSq()));
	}
};

namespace Math {
	//Vector 3 Cross product
	Vector3 CrossProduct(const Vector3 a, const Vector3 b) {
		return Vector3(a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	float Magnitude(const Vector3 in) {
		return (sqrtf(powf(in.x, 2) + powf(in.y, 2) + powf(in.z, 2)));
	}

	//Vector 3 dot product calculations
	float DotProduct(const Vector3 a, const Vector3 b) {
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	//Angle between two vector 3 objects
	float AngleBetweenVectors(const Vector3 a, const Vector3 b) {
		float angle = DotProduct(a, b);
		angle /= Magnitude(a) * Magnitude(b);
		return angle = acosf(angle);
	}

	//Projection calculation
	Vector3 ProjectionCalculation(const Vector3 a, const Vector3 b) {
		Vector3 bn = b / Magnitude(b);
		return bn * DotProduct(a, bn);
	}

	float DegreeToRadians(float degrees) {
		return ((degrees) * 3.1452 / 180.0f);
	}

	static Vector3 Normalise(const Vector3& vec) {
		Vector3 temp = vec;
		temp.Normalise();
		return temp;
	}
}



