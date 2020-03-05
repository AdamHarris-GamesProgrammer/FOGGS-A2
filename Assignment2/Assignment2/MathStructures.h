#pragma once
#include<vector>
#include <string>
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
		return (this->x == other.x && this->y == other.y && this->z = other.z);
	}

	bool operator!=(const Vector3& other) {
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
}

namespace Algorithm {
	//Vector 3 multiplication overload
	Vector3 operator*(const float& left, const Vector3& right) {
		return Vector3(right.x * left, right.y * left, right.z * left);
	}

	bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b) {
		Vector3 cp1 = Math::CrossProduct(b - a, p1 - a);
		Vector3 cp2 = Math::CrossProduct(b - a, p2 - a);

		if (Math::DotProduct(cp1, cp2) >= 0) {
			return true;
		}
		else
		{
			return false;
		}
	}

	Vector3 GenerateTriangleNormal(Vector3 t1, Vector3 t2, Vector3 t3) {
		Vector3 u = t2 - t1;
		Vector3 v = t3 - t1;

		Vector3 normal = Math::CrossProduct(u, v);

		return normal;
	}

	bool InTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3) {
		//Checks to see if the point is within the triangle outlines
		bool withinTriangle = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3) && SameSide(point, tri3, tri1, tri2);

		if (!withinTriangle) {
			return false;
		}

		//Calculates normal point for the triangle
		Vector3 n = GenerateTriangleNormal(tri1, tri2, tri3);

		Vector3 proj = Math::ProjectionCalculation(point, n);

		if (Math::Magnitude(proj) == 0) {
			return true;
		}
		else
		{
			return false;
		}
	}

	//Splits a line into various chunks based on the token e.g. ","
	inline void Split(const std::string& in, std::vector<std::string>& out, std::string token) {
		out.clear();

		std::string temp;

		for (int i = 0; i < int(in.size()); i++) {
			std::string test = in.substr(i, token.size());

			if (test == token) {
				if (!temp.empty()) {
					out.push_back(temp);
					temp.clear();
					i += (int)token.size() - 1;
				}
				else
				{
					out.push_back("");
				}
			}
			else if (i + token.size() >= in.size()) {
				temp = in.substr(i, token.size());
				out.push_back(temp);
				break;
			}
			else
			{
				temp += in[i];
			}
		}
	}

	inline std::string tail(const std::string& in) {
		if (!in.empty()) {
			size_t tokenStart = in.find_first_not_of(" \t");
			size_t tokenEnd = in.find_first_of(" \t", tokenStart);

			if (tokenStart != std::string::npos && tokenEnd != std::string::npos) {
				return in.substr(tokenStart, tokenEnd - tokenStart);
			}
			else if (tokenStart != std::string::npos) {
				return in.substr(tokenStart);
			}
		}
		return "";
	}

	template<class T>
	inline const T& GetElement(const std::vector<T>& elements, std::string& index) {
		int index = std::stoi(index);
		if (index < 0) {
			index = int(elements.size()) + index;
		}
		else
		{
			index--;
		}
		return elements[index];
	}
}