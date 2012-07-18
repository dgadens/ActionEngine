#include "Gjk.h"
#include "Vector3.h"
#include "MathHelper.h"

int Gjk::BitsToIndices[16] = { 0, 1, 2, 0x11, 3, 0x19, 0x1a, 0xd1, 4, 0x21, 0x22, 0x111, 0x23, 0x119, 0x11a, 0x8d1 };

Gjk::Gjk()
{
	closestPoint = new Vector3();

	//cria o array dos determinantes
	for (int i = 0; i < 0x10; i++)
        this->det[i] = new FLOAT[4];

	this->edges = new Vector3*[4];
	for (int i = 0; i < 4; i++)
        this->edges[i] = new Vector3[4];

	this->y = new Vector3[4];
};

Gjk::~Gjk()
{
	delete closestPoint;
	closestPoint = nullptr;

	//remove o array dos determinantes
	for (int i = 0; i < 0x10; i++)
	{
		delete [] this->det[i];
		this->det[i] = nullptr;
	}
        

	for (int i = 0; i < 4; i++)
	{
        delete [] this->edges[i];
		this->edges[i] = nullptr;
	}

	delete[] y;
	y = nullptr;

};

void Gjk::ComputeClosestPoint(Vector3 *result)
{
	FLOAT num3 = 0.0f;
	Vector3 zero(0);
	this->maxLengthSq = 0.0f;
	for (int i = BitsToIndices[this->FullSimplex()]; i != 0; i = i >> 3)
	{
		int index = (i & 7) - 1;
		FLOAT num4 = this->det[this->simplexBits][index];
		num3 += num4;

		zero.X += this->y[index].X * num4;
		zero.Y += this->y[index].Y * num4;
		zero.Z += this->y[index].Z * num4;

		this->maxLengthSq = MathHelper::Max(this->maxLengthSq, this->yLengthSq[index]);
	}

	Vector3::Divide(&zero, num3, result);
};

BOOL Gjk::IsSatisfiesRule(INT xBits, INT yBits)
{
	for (int i = BitsToIndices[yBits]; i != 0; i = i >> 3)
    {
        int index = (i & 7) - 1;
        int num3 = ((int)1) << index;
        if ((num3 & xBits) != 0)
        {
            if (this->det[xBits][index] <= 0.0f)
            {
                return false;
            }
        }
        else if (this->det[xBits | num3][index] > 0.0f)
        {
            return false;
        }
    }

    return true;
};

void Gjk::UpdateDeterminant(INT xmIdx)
{
	int index = ((int)1) << xmIdx;
	this->det[index][xmIdx] = 1.0f;
	int num14 = BitsToIndices[this->simplexBits];
	int num8 = num14;
	for (int i = 0; num8 != 0; i++)
	{
		int num = (num8 & 7) - 1;
		int num12 = ((int)1) << num;
		int num6 = num12 | index;
		this->det[num6][num] = Vector3::Dot(&edges[xmIdx][num], &y[xmIdx]);
		this->det[num6][xmIdx] = Vector3::Dot(&edges[num][xmIdx], &y[num]);
		int num11 = num14;
		for (int j = 0; j < i; j++)
		{
			int num3 = (num11 & 7) - 1;
			int num5 = ((int)1) << num3;
			int num9 = num6 | num5;
			int num4 = (this->edgeLengthSq[num][num3] < this->edgeLengthSq[xmIdx][num3]) ? num : xmIdx;
			this->det[num9][num3] = (this->det[num6][num] * Vector3::Dot(&edges[num4][num3], &y[num])) + (this->det[num6][xmIdx] * Vector3::Dot(&edges[num4][num3], &y[xmIdx]));
			num4 = (this->edgeLengthSq[num3][num] < this->edgeLengthSq[xmIdx][num]) ? num3 : xmIdx;
			this->det[num9][num] = (this->det[num5 | index][num3] * Vector3::Dot(&edges[num4][num], &y[num3])) + (this->det[num5 | index][xmIdx] * Vector3::Dot(&edges[num4][num], &y[xmIdx]));
			num4 = (this->edgeLengthSq[num][xmIdx] < this->edgeLengthSq[num3][xmIdx]) ? num : num3;
			this->det[num9][xmIdx] = (this->det[num12 | num5][num3] * Vector3::Dot(&edges[num4][xmIdx], &y[num3])) + (this->det[num12 | num5][num] * Vector3::Dot(&edges[num4][xmIdx], &y[num]));
			num11 = num11 >> 3;
		}
		num8 = num8 >> 3;
	}
	if ((this->simplexBits | index) == 15)
	{
		int num2 = (this->edgeLengthSq[1][0] < this->edgeLengthSq[2][0]) ? ((this->edgeLengthSq[1][0] < this->edgeLengthSq[3][0]) ? 1 : 3) : ((this->edgeLengthSq[2][0] < this->edgeLengthSq[3][0]) ? 2 : 3);
		this->det[15][0] = ((this->det[14][1] * Vector3::Dot(&edges[num2][0], &y[1])) + (this->det[14][2] * Vector3::Dot(&edges[num2][0], &y[2]))) + (this->det[14][3] * Vector3::Dot(&edges[num2][0], &y[3]));
		num2 = (this->edgeLengthSq[0][1] < this->edgeLengthSq[2][1]) ? ((this->edgeLengthSq[0][1] < this->edgeLengthSq[3][1]) ? 0 : 3) : ((this->edgeLengthSq[2][1] < this->edgeLengthSq[3][1]) ? 2 : 3);
		this->det[15][1] = ((this->det[13][0] * Vector3::Dot(&edges[num2][1], &y[0])) + (this->det[13][2] * Vector3::Dot(&edges[num2][1], &y[2]))) + (this->det[13][3] * Vector3::Dot(&edges[num2][1], &y[3]));
		num2 = (this->edgeLengthSq[0][2] < this->edgeLengthSq[1][2]) ? ((this->edgeLengthSq[0][2] < this->edgeLengthSq[3][2]) ? 0 : 3) : ((this->edgeLengthSq[1][2] < this->edgeLengthSq[3][2]) ? 1 : 3);
		this->det[15][2] = ((this->det[11][0] * Vector3::Dot(&edges[num2][2], &y[0])) + (this->det[11][1] * Vector3::Dot(&edges[num2][2], &y[1]))) + (this->det[11][3] * Vector3::Dot(&edges[num2][2], &y[3]));
		num2 = (this->edgeLengthSq[0][3] < this->edgeLengthSq[1][3]) ? ((this->edgeLengthSq[0][3] < this->edgeLengthSq[2][3]) ? 0 : 2) : ((this->edgeLengthSq[1][3] < this->edgeLengthSq[2][3]) ? 1 : 2);
		this->det[15][3] = ((this->det[7][0] * Vector3::Dot(&edges[num2][3], &y[0])) + (this->det[7][1] * Vector3::Dot(&edges[num2][3], &y[1]))) + (this->det[7][2] * Vector3::Dot(&edges[num2][3], &y[2]));
	}
};

BOOL Gjk::UpdateSimplex(INT newIndex)
{
    int yBits = this->simplexBits | (((int)1) << newIndex);
    int xBits = ((int)1) << newIndex;
    for (int i = this->simplexBits; i != 0; i--)
    {
        if (((i & yBits) == i) && this->IsSatisfiesRule(i | xBits, yBits))
        {
            this->simplexBits = i | xBits;
            this->ComputeClosestPoint(this->closestPoint);
            return true;
        }
    }
    bool flag = false;
    if (this->IsSatisfiesRule(xBits, yBits))
    {		
        this->simplexBits = xBits;
        *this->closestPoint = this->y[newIndex];
        this->maxLengthSq = this->yLengthSq[newIndex];
        flag = true;
    }
    return flag;
};

void Gjk::Reset()
{
    this->simplexBits = 0;
    this->maxLengthSq = 0.0f;
};

BOOL Gjk::AddSupportPoint(Vector3 *newPoint)
{
    int index = (BitsToIndices[this->simplexBits ^ 15] & 7) - 1;
    this->y[index] = *newPoint;
    this->yLengthSq[index] = Vector3::LengthSquared(newPoint);
    for (int i = BitsToIndices[this->simplexBits]; i != 0; i = i >> 3)
    {
        int num2 = (i & 7) - 1;
        Vector3 vector(0); 
		Vector3::Subtract(&y[num2], newPoint, &vector);
        this->edges[num2][index] = vector;
        Vector3 vectorOne(1); 
		Vector3::Subtract(&vectorOne, &vector, &edges[index][num2]);
		FLOAT length = Vector3::LengthSquared(&vector);
        this->edgeLengthSq[index][num2] = this->edgeLengthSq[num2][index] = length;
    }

    this->UpdateDeterminant(index);

    return this->UpdateSimplex(index);
}
