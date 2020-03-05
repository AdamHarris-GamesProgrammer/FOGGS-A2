#pragma once

struct Vector2 {
	float x, y;

	Vector2(float xValue = 0.0f, float yValue = 0.0f) {
		x = xValue;
		y = yValue;
	}

	bool operator==(const Vector2& other) {
		return (this->x == other.x && this->y == other.y);
	}

	bool operator!=(const Vector2 & other) {
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

	Vector2 operator/(const Vector2 & other) {
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

	bool operator==(const Vector3& other) {
		return (this->x == other.x && this->y == other.y, this->z = other.z);
	}
	
	bool operator!=(const Vector3& other) {
		return !(this->x == other.x && this->y == other.y, this->z == other.z);
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

	Vector3 operator/(const Vector3& other) {
		return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
	}
};