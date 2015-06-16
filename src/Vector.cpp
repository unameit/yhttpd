#include <stdlib.h>
#include "SString.h"
#include "Exception.h"
#include "SystemException.h"
#include "Vector.h"


Vector::Vector(uint initialCapacity, uint initialIncrement)
{
	if (0 == initialIncrement)
		throw Exception("Vector::Vector - Invalid increment value!\nZero not allowed!");

	capacity = initialCapacity;
	increment = initialIncrement;
	count = 0;

	data = (void **) malloc( capacity * sizeof(void *) );
	if (NULL == data)
		throw SystemException(SString("Not enough memory!\nmalloc(%u) failed!", capacity * sizeof(void *) ));

}


uint Vector::add(void *element)
{
	if (NULL == element)
		throw SystemException("Vector::add(NULL) - null pointer exception");

	if (count == capacity) {
		void **tmp = (void **) realloc( data, (capacity + increment) * sizeof(void *) );
		if (NULL == tmp)
			throw SystemException(SString("Not enough memory!\nrealloc(data, %u) failed!", (capacity + increment) * sizeof(void *)));
		data = tmp;
		capacity += increment;
	}
	data[count] = element;
	return count++;
}


void *Vector::operator[](uint i) const
{
	if (i >= count)
		throw Exception(SString("Vector::operator[](%u) - Invalid index (this->count = %u)!", i, count));
	return data[i];
}


void Vector::set(uint i, void *e)
{
	if (i >= count)
		throw Exception(SString("Vector::set(%u, element) - Invalid index (this->count = %u)!", i, count));
	data[i] = e;
}


void Vector::setCapacity(uint newCapacity)
{
	void **tmp = (void **) realloc( data, newCapacity * sizeof(void *) );
	if (NULL == tmp)
		throw SystemException(SString("realloc(data, %u) failed!", newCapacity * sizeof(void *)));

	data = tmp;
	capacity = newCapacity;
	if (count > capacity)
		count = capacity;
}


void Vector::setIncrement(uint newIncrement)
{
	if (0 == newIncrement)
		throw Exception("Vector::setIncrement(0) - Zero is invalid increment value!");

	increment = newIncrement;
}


void Vector::setCount(uint newCount)
{
	if (newCount > count)
		throw Exception(SString("Vector::setCount(%u) - actual count is smaller then requested (this->count = %u)!", newCount, count));

	count = newCount;
}


void Vector::swap(uint i, uint j)
{
	if (i >= count || j >= count)
		throw Exception(SString("Vector::swap(%u, %u) - invalid indexes (this->count = %u)!", i, j, count));
	void *tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}


void *Vector::remove(uint i)
{
	if (i >= count)
		throw Exception(SString("Vector::remove(%u) - invalid index (this->count = %u)!", i, count));
	uint j = count - 1;
	swap(i, j);					// usuwany element zamieñ miescami z ostatnim
	return data[count = j];
}
