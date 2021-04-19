//
// Created by decil on 4/19/21.
//

#ifndef PHANTOM_VECTOR3_H
#define PHANTOM_VECTOR3_H

struct Vector3
{

    Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }


    float x;
    float y;
    float z;

    // Normalizes in place
    void NormalizeInPlace();
    // Returns normalized vector
    Vector3 Normalize();

    float DotProduct();
    Vector3 CrossProduct();

    float Distance(const Vector3& a, const Vector3& b);

    float Length();

};

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v1.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{

}

Vector3 operator/(const Vector3& v1, const Vector3& v2)
{

}

Vector3 operator*(const Vector3& v1, const Vector3& v2)
{

}


#endif //PHANTOM_VECTOR3_H
