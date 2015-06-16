#ifndef	VECTOR_H
#define	VECTOR_H

#include <stdlib.h>			// qsort, bsearch
#include <sys/types.h>		// uint

/**
 * Implementacja struktury danych okre¶lanej jako dynamiczna tablica.
 * Przechowuje wska¼niki do "czegokolwiek" (void *). Po dostarczeniu jej
 * funkcji porównuj±cej potrafi sortowaæ oraz wyszukiwaæ po¿±dane elementy.
 */

class Vector {
public:
	/** utwórz pusty wektor */
	Vector(uint = 100, uint = 100);

	/** Usuñ tablicê */
	~Vector() { free(data); }

	/** dodaj nowy element do tablicy */
	uint add(void *);

	/** zmieñ element o indeksie index na nowy */
	void set(uint index, void *element);

	/** podaj mi i-ty element tablicy */
	void *operator[](uint) const;

	/** zamieñ miejscami dwa elementy tablicy */
	void swap(uint i, uint j);

	/** usuñ i-ty element tablicy */
	void *remove(uint i);

	/** podaj mi aktualn± liczbê elementów w tej tablicy */
	uint getCount() const { return count; }

	/** ustaw liczbê elementów w tablicy - mo¿na j± tylko zmniejszyæ. */
	void setCount(uint);

	/** wyrzuæ wszysktie elementy */
	void clear() { setCount(0); }

	/** posortuj elementy tablicy wg podanej funkcji porównuj±cej. */
	void sort(int (*cmpf)(const void *, const void *)) { qsort(data, count, sizeof(void *), cmpf); }

	/** wyszukaj element w tablicy (maj±c podan± funkcjê porównuj±c±). */
	void *search(const void *key, int (*cmpf)(const void *, const void *)) const { return bsearch(key, data, count, sizeof(void *), cmpf); }

	/** podaj mi aktualn± pojemno¶æ tablicy */
	uint getCapacity() const { return capacity; }

	/** ustaw j± */
	void setCapacity(uint);

	/** o ile zwiêksza siê pojemno¶æ tablicy, gdy siê skoñczy w niej miejsce? */
	uint getIncrement() const { return increment; }

	/** ustaw tê warto¶æ */
	void setIncrement(uint);

private:	
	/** actual capacity */
	uint capacity;

	/** capacity increment */
	uint increment;

	/** number of valid elements */
	uint count;

	void **data;
};

#endif
