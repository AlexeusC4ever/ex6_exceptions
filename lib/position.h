#ifndef POSITION_H_
#define POSITION_H_

template<class T>
class Position {
private:
	T* elem;
	size_t number;
	size_t number_in_memory;
	size_t shape;

public:
	Position(T* elem_ = nullptr, size_t number_ = 0, size_t shape_ = 0) :
		elem(elem_), number(number_), shape(shape_) {
		number_in_memory = number_in_mem(number_);
	}

	Position(const Position&) = default;

	size_t get_number_in_mem() const { return number_in_memory; }
	T& dereference() const { return *elem; } // Получение текущего элемента.
	T* pointer() const { return elem; }
	bool equal(const Position& other) const { return other.pointer() == elem; } // Проверка на равенство.

	size_t number_in_mem(size_t num) const
	{
		size_t i = num / this->shape;
		size_t j = num % this->shape;
		size_t el_from_memory = 0;
		if (i > j) std::swap(i, j);
		if (i >= 0 && i < this->shape && j >= 0 && j < this->shape) {
			el_from_memory = (2 * this->shape - i + 1) * i / 2 + j - i;
		}
		else if (num == shape * shape) {
			el_from_memory = shape * (shape + 1) / 2;
		}
		else throw std::out_of_range("out_of_range");
		return el_from_memory;
	}

	T& get_el(size_t num)
	{
		size_t el_from_memory = number_in_mem(num);
		size_t difference = el_from_memory - number_in_memory;
		number_in_memory = el_from_memory;
		return *(this->elem + difference);
	}

	void increment()
	{
		elem = &get_el(number + 1);
		++number;
	} // Перемещение вперед.

	void decrement()
	{
		elem = &get_el(number + 1);
		--number;
	} // Перемещение назад.

	void advance(int n)
	{
		elem = &get_el(number + n);
		number += n;
	}  // Перемещение на "n" элементов.

	int distance_to(const Position& other) const
	{
		int res = other.number - number;
		if (res < 0) res = -res;
		return res;
	} // Расстояние до другой позиции.
};

#endif  // POSITION_H_