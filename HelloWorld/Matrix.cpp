#include "Matrix.h"

mat3::mat3(vec3 _row1, vec3 _row2, vec3 _row3)
{
	//Sets row1, row2, and row3
	row1 = _row1;
	row2 = _row2;
	row3 = _row3;
}

void mat3::Add(mat3 m)
{
	//Adds the passed parameter to THIS matrix
	this->row1 = this->row1 + m.row1;
	this->row2 = this->row2 + m.row2;
	this->row3 = this->row3 + m.row3;
}

void mat3::Subtract(mat3 m)
{
	//Subtracts the passed parameter from THIS matrix
	this->row1 = this->row1 - m.row1;
	this->row2 = this->row2 - m.row2;
	this->row3 = this->row3 - m.row3;
}

void mat3::print()
{
	//Prints out formatted matrix in console
	std::cout << "[ " << this->row1.x << ", " << this->row1.y << " ]\n";
	std::cout << "[ " << this->row2.x << ", " << this->row2.y << " ]\n";
	std::cout << "[ " << this->row3.x << ", " << this->row3.y << " ]\n";
}

mat3 mat3::Transpose()
{
	//Just swaps the rows and columns within the matrix
	mat3 temp;
	temp.row1 = vec3(row1.x, row2.x, row3.x);
	temp.row2 = vec3(row1.y, row2.y, row3.y);
	temp.row3 = vec3(row1.z, row2.z, row3.z);

	//Returns the transposed matrix
	return temp;
}

float mat3::Determinant()
{
	//Calculates the determinant
	//det(Mat3)
	float temp = (row1.x * ((row2.y * row3.z) - (row2.z * row3.y))) +
				 (row1.y * ((row2.z * row3.x) - (row2.x * row3.z))) +
				 (row1.z * ((row2.x * row3.y) - (row2.y * row3.x)));

	//Returns determinant
	return temp;
}

mat3 mat3::Inverse()
{
	//The inverse is equal to this
	//     -1   =       1
	//[A B]     =   ---------[ D -B]
	//[C D]     =   Det(Mat2)[-C  A]

	//Gets one over determinant
	float oneOverDet = 1.f / Determinant();

	//Creates new matrix to be multiplied
	mat3 temp = mat3(
		vec3(row2.y, -row1.y, row1.z),
		vec3(-row2.x, row2.x, row2.z),
		vec3(-row2.x, row2.x, row3.z)
	);

	//Multiplies temp by oneOverDet
	temp = temp * oneOverDet;

	//Returns the inverse of our matrix
	return temp;
}

mat3::mat3(mat4 m)
{
	//Creates a 3x3 matrix using a 4x4 matrix
	this->row1 = vec3(m.row1.x, m.row1.y, m.row1.z);
	this->row2 = vec3(m.row2.x, m.row2.y, m.row2.z);
	this->row3 = vec3(m.row3.x, m.row3.y, m.row3.z);
}

mat3 mat3::Transpose(mat3 R)
{
	//Justs swaps the rows and columns within the matrix
	mat3 temp;
	temp.row1 = vec3(R.row1.x, R.row2.x, R.row3.x);
	temp.row2 = vec3(R.row1.y, R.row2.y, R.row3.y);
	temp.row3 = vec3(R.row1.z, R.row2.z, R.row3.z);

	//Returns the transposed matrix
	return temp;
}

mat3 mat3::operator+(mat3 m)
{
	//Returns the summed matrix
	return mat3(this->row1 + m.row1, this->row2 + m.row2, this->row3 + m.row3);
}

mat3 mat3::operator-(mat3 m)
{
	//Returns difference
	return mat3(this->row1 - m.row1, this->row2 - m.row2, this->row3 - m.row3);
}

mat3 mat3::operator*(float f)
{
	//Returns product
	return mat3(this->row1 * f, this->row2 * f, this->row3 * f);
}

mat3 mat3::operator*(mat3 m)
{
	vec3 mCol1 = vec3(m.row1.x, m.row2.x, m.row3.x);
	vec3 mCol2 = vec3(m.row1.y, m.row2.y, m.row3.y);
	vec3 mCol3 = vec3(m.row1.z, m.row2.z, m.row3.z);

	mat3 temp = mat3(
		vec3(row1.Dot(mCol1), row1.Dot(mCol2), row1.Dot(mCol3)),
		vec3(row2.Dot(mCol1), row2.Dot(mCol2), row2.Dot(mCol3)),
		vec3(row3.Dot(mCol1), row3.Dot(mCol2), row3.Dot(mCol3))
	);

	return temp;
}

mat3 mat3::operator-()
{
	//Negates all of the rows
	return mat3(-row1, -row2, -row3);
}

vec3 mat3::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = row1
	//* 1 = row2
	//* 2 = row3
	return *hold[i];
}

vec3 mat3::operator*(vec3 vec)
{
	//Multiplies 3x3 matrix by 3D vector
	vec3 temp;
	temp = vec3(
		row1.x * vec.x + row1.y * vec.y + row1.z * vec.z,
		row2.x * vec.x + row2.y * vec.y + row2.z * vec.z, 
		row3.x * vec.x + row3.y * vec.y + row3.z * vec.z
	);

	//Returns calculated 3D vector
	return temp;
}

mat4::mat4(vec4 _row1, vec4 _row2, vec4 _row3, vec4 _row4)
{
	//Sets row1, row2, row3, and row4
	row1 = _row1;
	row2 = _row2;
	row3 = _row3;
	row4 = _row4;
}

void mat4::Add(mat4 m)
{
	//Adds the passed parameter to THIS matrix
	this->row1 = this->row1 + m.row1;
	this->row2 = this->row2 + m.row2;
	this->row3 = this->row3 + m.row3;
	this->row4 = this->row4 + m.row4;
}

void mat4::Subtract(mat4 m)
{
	//Subtracts the passed parameter from THIS matrix
	this->row1 = this->row1 - m.row1;
	this->row2 = this->row2 - m.row2;
	this->row3 = this->row3 - m.row3;
	this->row4 = this->row4 - m.row4;
}

void mat4::print()
{
	//Prints out formatted matrix in console
	std::cout << "[ " << this->row1.x << ", " << this->row1.y << " ]\n";
	std::cout << "[ " << this->row2.x << ", " << this->row2.y << " ]\n";
	std::cout << "[ " << this->row3.x << ", " << this->row3.y << " ]\n";
	std::cout << "[ " << this->row4.x << ", " << this->row4.y << " ]\n";
}

mat4 mat4::Transpose()
{
	//Just swaps the rows and columns within the matrix
	mat4 temp;
	temp.row1 = vec4(row1.x, row2.x, row3.x, row4.x);
	temp.row2 = vec4(row1.y, row2.y, row3.y, row4.y);
	temp.row3 = vec4(row1.z, row2.z, row3.z, row4.z);
	temp.row4 = vec4(row1.w, row2.w, row3.w, row4.w);

	//Returns the transposed matrix
	return temp;
}

float mat4::Determinant()
{
	//Calculates the determinant
	//det(Mat4)
	float temp = (row1.x * row2.y * row3.z * row4.w) + (row1.y * row2.z * row3.w * row4.x)
			   + (row1.z * row2.w * row3.x * row4.y) - (row1.w * row2.z * row3.y * row4.x)
			   - (row1.x * row2.w * row3.z * row4.y) - (row1.y * row2.x * row3.w * row4.z);

	//Returns determinant
	return temp;
}

mat4 mat4::Inverse()
{
	//The inverse is equal to this
	//     -1   =       1
	//[A B]     =   ---------[ D -B]
	//[C D]     =   Det(Mat2)[-C  A]

	//Gets one over determinant
	float oneOverDet = 1.f / Determinant();

	//Creates new matrix to be multiplied
	mat4 temp = mat4(
		vec4(row2.y, -row1.y, row1.z, row1.w),
		vec4(-row2.x, row2.x, row2.z, row1.w),
		vec4(-row2.x, row2.x, row3.z, row1.w),
		vec4(-row2.x, row2.x, row3.z, row1.w)
	);

	//Multiplies temp by oneOverDet
	temp = temp * oneOverDet;

	//Returns the inverse of our matrix
	return temp;
}


mat4::mat4(mat3 rot, vec3 trans)
{
	//Creates a homogeneous transformation matrix using this data
	row1 = vec4(rot.row1.x, rot.row1.y, rot.row1.z, trans.x);
	row2 = vec4(rot.row2.x, rot.row2.y, rot.row2.z, trans.y);
	row3 = vec4(rot.row3.x, rot.row3.y, rot.row3.z, trans.z);
	row4 = vec4(0.f, 0.f, 0.f, 1.f);
}

mat4 mat4::FastInverse(mat4 mat)
{
	//In order to do a faster inverse we assume it's a homogeneous transformation matrix which means
	//| R(3x3) | T(1x3) | inverse is equal to |R(3x3)^T|T(1x3)*-R|
	//|--------|--------|                     |--------|---------|
	//| 0(3x1) | 1(1x1) |                     | 0(3x1) |  1(1x1) |

	//Grab the rotation matrix out of mat
	mat3 rotation = mat3(mat);
	//Grab the translation vector out of mat
	vec3 translation = Translation(mat);
	//Transpose the rotation matrix
	rotation = mat3::Transpose(rotation);
	//Multiply negative rotation by translation
	translation = -rotation * translation;

	//Recreate the homogeneous transformation matrix using the new 3x3 rotation matrix and 3d translation vector
	return mat4(rotation, translation);
}

vec3 mat4::Translation(mat4 mat)
{
	//Grabs the translation data from the homogeneous transformation matrix
	return vec3(mat.row1.z, mat.row2.z, mat.row3.z);
}

mat4 mat4::operator+(mat4 m)
{
	//Returns the summed matrix
	return mat4(this->row1 + m.row1, this->row2 + m.row2, this->row3 + m.row3, this->row4 + m.row4);
}

mat4 mat4::operator-(mat4 m)
{
	//Returns difference
	return mat4(this->row1 - m.row1, this->row2 - m.row2, this->row3 - m.row3, this->row4 - m.row4);
}

mat4 mat4::operator*(float f)
{
	//Returns product
	return mat4(this->row1 * f, this->row2 * f, this->row3 * f, this->row4 * f);
}

mat4 mat4::operator*(mat4 m)
{
	vec4 mCol1 = vec4(m.row1.x, m.row2.x, m.row3.x, m.row4.x);
	vec4 mCol2 = vec4(m.row1.y, m.row2.y, m.row3.y, m.row4.y);
	vec4 mCol3 = vec4(m.row1.z, m.row2.z, m.row3.z, m.row4.z);
	vec4 mCol4 = vec4(m.row1.z, m.row2.z, m.row3.z, m.row4.w);

	mat4 temp = mat4(
		vec4(row1.Dot(mCol1), row1.Dot(mCol2), row1.Dot(mCol3), row1.Dot(mCol4)),
		vec4(row2.Dot(mCol1), row2.Dot(mCol2), row2.Dot(mCol3), row1.Dot(mCol4)),
		vec4(row3.Dot(mCol1), row3.Dot(mCol2), row3.Dot(mCol3), row1.Dot(mCol4)),
		vec4(row3.Dot(mCol1), row3.Dot(mCol2), row3.Dot(mCol3), row1.Dot(mCol4))
	);

	return temp;
}

vec4 mat4::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = row1
	//* 1 = row2
	//* 2 = row3
	//* 3 = row4
	return *hold[i];
}

vec4 mat4::operator*(vec4 vec)
{
	//Multiplies 4x4 matrix by 3D vector
	vec4 temp;
	temp = vec4(
		row1.x * vec.x + row1.y * vec.y + row1.z * vec.z + row1.w * vec.w,
		row2.x * vec.x + row2.y * vec.y + row2.z * vec.z + row2.w * vec.w,
		row3.x * vec.x + row3.y * vec.y + row3.z * vec.z + row3.w * vec.w,
		row4.x * vec.x + row4.y * vec.y + row4.z * vec.z + row4.w * vec.w
	);

	//Returns calculated 3D vector
	return temp;
}

mat2::mat2(vec2 _row1, vec2 _row2)
{
	//Sets row1 and row2
	row1 = _row1;
	row2 = _row2;
}

void mat2::Add(mat2 m)
{
	//Adds the passed parameter to THIS matrix
	this->row1 = this->row1 + m.row1;
	this->row2 = this->row2 + m.row2;
}

void mat2::Subtract(mat2 m)
{
	//Subtracts the passed parameter from THIS matrix
	this->row1 = this->row1 - m.row1;
	this->row2 = this->row2 - m.row2;
}

void mat2::print()
{
	//Prints out formatted matrix in console
	std::cout << "[ " << this->row1.x << ", " << this->row1.y << " ]\n";
	std::cout << "[ " << this->row2.x << ", " << this->row2.y << " ]\n";
}

mat2 mat2::Transpose()
{
	//Just swaps the rows and columns within the matrix
	mat2 temp;
	temp.row1 = vec2(row1.x, row2.x);
	temp.row2 = vec2(row1.y, row2.y);

	//Returns the transposed matrix
	return temp;
}

float mat2::Determinant()
{
	//Calculates the determinant
	//det(Mat2) = AD - BC
	float temp = (row1.x * row2.y) - (row1.y * row2.x);

	//Returns determinant
	return temp;
}

mat2 mat2::Inverse()
{
	//The inverse is equal to this
	//     -1   =       1
	//[A B]     =   ---------[ D -B]
	//[C D]     =   Det(Mat2)[-C  A]

	//Gets one over determinant
	float oneOverDet = 1.f / Determinant();

	//Creates new matrix to be multiplied
	mat2 temp = mat2(
		vec2(row2.y, -row1.y),
		vec2(-row2.x, row2.x)
	);

	//Multiplies temp by oneOverDet
	temp = temp * oneOverDet;

	//Returns the inverse of our matrix
	return temp;
}

mat2 mat2::operator+(mat2 m)
{
	//Returns the summed matrix
	return mat2(this->row1 + m.row1, this->row2 + m.row2);
}

mat2 mat2::operator-(mat2 m)
{
	//Returns difference
	return mat2(this->row1 - m.row1, this->row2 - m.row2);
}

mat2 mat2::operator*(float f)
{
	//Returns product
	return mat2(this->row1 * f, this->row2 * f);
}

mat2 mat2::operator*(mat2 m)
{
	vec2 mCol1 = vec2(m.row1.x, m.row2.x);
	vec2 mCol2 = vec2(m.row1.y, m.row2.y);

	mat2 temp = mat2(
		vec2(row1.Dot(mCol1), row1.Dot(mCol2)),
		vec2(row2.Dot(mCol1), row2.Dot(mCol2))
	);

	return temp;
}

vec2 mat2::operator*(vec2 v)
{
	//Multiplies 3x3 matrix by the 3D vector
	vec2 temp = vec2(
		row1.x * v.x + row1.y * v.y,
		row2.x * v.x + row2.y * v.y
	);

	//Returns calculated 3D vector
	return temp;
}

vec2 mat2::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = row1
	//* 1 = row2
	return *hold[i];
}
