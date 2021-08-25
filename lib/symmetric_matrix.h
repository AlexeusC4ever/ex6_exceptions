#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <memory>
#include <iostream>
#include <iterator>

template<class T>
class Symmetric_Matrix;

//template<class T1, class T2>
//void construct(T1* pT1, const T2& rcT2)
//{
//	new (pT1) T1(rcT2);
//}

template<class T1, class T2>
void construct(T1* pT1, T2&& rcT2)
{
	new (pT1) T1(std::forward<T2>(rcT2));
}

template<class T>
void destroy(T* pT)
{
	pT->~T();
}

template<class FwdIter>
void destroy(FwdIter first, FwdIter last)
{
	while (first != last)
	{
		destroy(&*first);
		++first;
	}
}

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



	size_t get_number_in_mem() { return number_in_memory; }
	T& dereference() const { return *elem; } // Получение текущего элемента.
	T* pointer() const { return elem; }
	bool equal(const Position& other) const { return other.pointer() == elem; } // Проверка на равенство.

	size_t number_in_mem(size_t num)
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

template<class T>
class Sym_iter_base : public std::iterator<std::random_access_iterator_tag, T>
{
public:
	Position<T> pos;

	Sym_iter_base(Position<T> pos_) : pos(pos_) {}
	Sym_iter_base() = default;
	Sym_iter_base(const Sym_iter_base&) = default;
	Sym_iter_base& operator=(const Sym_iter_base&) = default;
	~Sym_iter_base() = default;

	Sym_iter_base& operator++() { pos.increment(); return *this; }
	Sym_iter_base operator++(int) { auto old = *this; ++(*this); return old; }

	Sym_iter_base& operator--() { pos.decrement(); return *this; }
	Sym_iter_base operator--(int) { auto old = *this; --(*this); return old; }

	Sym_iter_base& operator+=(int n) { pos.advance(n); return *this; }
	Sym_iter_base& operator-=(int n) { return *this += -n; }

};

template<class T>
Sym_iter_base<T> operator-(Sym_iter_base<T> it, int n) { it -= n; return it; }

template<class T>
int operator-(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return rhs.pos.distance_to(lhs.pos); }

template<class T>
Sym_iter_base<T> operator+(Sym_iter_base<T> it, int n) { it += n; return it; }

template<class T>
Sym_iter_base<T> operator+(int n, Sym_iter_base<T> it) { return it + n; }

template<class T>
bool operator<(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return lhs.pos.distance_to(rhs.pos) > 0; }

template<class T>
bool operator>(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return rhs < lhs; }

template<class T>
bool operator<=(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return !(rhs > lhs); }

template<class T>
bool operator>=(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return !(lhs < rhs); }

template<class T>
bool operator==(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return lhs.pos.equal(rhs.pos); }

template<class T>
bool operator!=(const Sym_iter_base<T>& lhs, const Sym_iter_base<T>& rhs) { return !(lhs == rhs); }


template<class T>
class Const_Sym_Mat_Iterator :
	public Sym_iter_base<T>
{
public:
	using reference = const T&;
	using pointer = const T*;
	

	Const_Sym_Mat_Iterator(Position<T> pos_) :
		Sym_iter_base<T>(pos_) {}

	reference operator*() const { return this->pos.dereference(); }

	reference operator[](int n) const { auto tmp = *this; tmp += n; return *tmp; }

	pointer operator->() { return this->pos.pointer(); }
};


template<class T>
class Sym_Mat_Iterator : 
	public Sym_iter_base<T>
{
public:
	using reference = T&;
	using pointer = const T*;

	Sym_Mat_Iterator(Position<T> pos_):
		Sym_iter_base<T>(pos_) {}

	reference operator*() const { return this->pos.dereference(); }

	reference operator[](int n) const { auto tmp = *this; tmp += n; return *tmp; }

	pointer operator->() { return this->pos.pointer(); }
};



template<class T>
class Symmetric_Matrix_impl
{
protected:
	T* data;
	size_t size;
	size_t capacity;
	std::shared_ptr<int> ref_counter;
	size_t shape;

public:
	Symmetric_Matrix_impl(size_t shape_ = 0) :
		shape(shape_),
		ref_counter(new int(1)),
		data(static_cast<T*> (0 == shape_ ? nullptr :
			operator new(get_actual_size(shape_) * sizeof(T)))),
		size(get_actual_size(shape_)),
		capacity(get_actual_size(shape_)) {}

	size_t get_actual_size(size_t sh) noexcept { return sh * (sh + 1) / 2; }

	void swap(Symmetric_Matrix_impl& rImpl) noexcept
	{
		std::swap(data, rImpl.data);
		std::swap(ref_counter, rImpl.ref_counter);
		std::swap(size, rImpl.size);
		std::swap(capacity, rImpl.capacity);
		std::swap(shape, rImpl.shape);
	}

	~Symmetric_Matrix_impl()
	{
		if (ref_counter.use_count() == 1)
		{
			destroy(data, data + size);
			operator delete(data);
		}
	}
};

template<class T>
class Symmetric_Matrix : private Symmetric_Matrix_impl<T>
{
public:
	friend Position<T>;
	using iterator = Sym_Mat_Iterator<T>;
	using const_iterator = Const_Sym_Mat_Iterator<T>;

	Symmetric_Matrix(size_t shape_ = 0) :
		Symmetric_Matrix_impl<T>(shape_) {}

	Symmetric_Matrix(const Symmetric_Matrix& a) = default;

	Symmetric_Matrix& operator = (Symmetric_Matrix& a) 
	{
		Symmetric_Matrix<T> tmp(a);
		this->swap(tmp);
		return *this;
	}

	Symmetric_Matrix(Symmetric_Matrix<T>&& a) noexcept
	{
		this->swap(a);
	}

	Symmetric_Matrix& operator = (Symmetric_Matrix<T>&& a)
	{
		Symmetric_Matrix<T> tmp;
		this->swap(tmp);
		this->swap(a);
		return *this;
	}

	void push_back(const T& new_el)
	{
		size_t required_size = this->size + this->shape + 1;

		Symmetric_Matrix<T> tmp;
		tmp.reserve(required_size);

		for (size_t i = 0; i < this->size; ++i)
			construct(tmp.data + i, *(this->data + i));

		for (size_t i = this->size; i < required_size; ++i)
			construct(tmp.data + i, new_el);

		tmp.size = required_size;
		tmp.shape = this->shape + 1;

		this->swap(tmp);
	}

	void emplace_back(T&& new_el)
	{
		size_t required_size = this->size + this->shape + 1;

		Symmetric_Matrix<T> tmp;
		tmp.reserve(required_size);

		for (size_t i = 0; i < this->size; ++i)
			construct(tmp.data + i, *(this->data + i));

		for (size_t i = this->size; i < required_size; ++i)
			construct(tmp.data + i, std::forward<T>(new_el));

		tmp.size = required_size;
		tmp.shape = this->shape + 1;

		this->swap(tmp);
	}

	void push_front(const T& new_el)
	{
		size_t required_size = this->size + this->shape + 1;

		Symmetric_Matrix<T> tmp;
		tmp.reserve(required_size);

		for (size_t i = 0; i < required_size - this->size; ++i)
			construct(tmp.data + i, new_el);

		for (size_t i = required_size - this->size; i < required_size; ++i)
			construct(tmp.data + i, *(this->data + i - (this->shape + 1)));

		tmp.size = required_size;
		tmp.shape = this->shape + 1;

		this->swap(tmp);
	}

	void emplace_front(T&& new_el)
	{
		size_t required_size = this->size + this->shape + 1;

		Symmetric_Matrix<T> tmp;
		tmp.reserve(required_size);

		for (size_t i = 0; i < required_size - this->size; ++i)
			construct(tmp.data + i, std::forward<T>(new_el));

		for (size_t i = required_size - this->size; i < required_size; ++i)
			construct(tmp.data + i, *(this->data + i - (this->shape + 1)));

		tmp.size = required_size;
		tmp.shape = this->shape + 1;

		this->swap(tmp);
	}

	void erase(iterator el_for_erase)
	{
		//deep_copy(this->capacity);
		size_t el_in_memory = el_for_erase.pos.get_number_in_mem();

		Symmetric_Matrix<T> tmp(this->shape);
		tmp.reserve(this->capacity);

		size_t size_for_copy = (el_in_memory <= this->size - this->shape ?
			this->size - this->shape: this->size - this->shape - 1);


		for (size_t i = 0, j = 0; i < size_for_copy; ++i) {
			if (j == el_in_memory) ++j;
			construct(tmp.data + i, *(this->data + j));
			++j;
		}
		--tmp.shape;
		tmp.size -= this->shape;

		this->swap(tmp);
	}

	void deep_copy(size_t n)
	{
		if (this->ref_counter.use_count() > 1)
		{ 
			Symmetric_Matrix<T> tmp;
			tmp.reserve(n);
			tmp.shape = this->shape;
			tmp.size = this->size;

			for (size_t i = 0; i < n; ++i)
				construct(tmp.data + i, *(this->data + i));

			this->swap(tmp);
		}
		else if (n > this->capacity)
		{
			reserve(n);
		}
	}

	void reserve(size_t n)
	{
		if (n < this->capacity) return;

		std::shared_ptr<int> new_ref_counter(new int(1));
		T* new_memory = static_cast<T*> (operator new(n * sizeof(T)));

		try {
			for (size_t i = 0; i < this->size; ++i)
				construct(new_memory + i, *(this->data + i));
		}
		catch (...) {
			operator delete(new_memory);
			throw;
		}

		std::swap(this->data, new_memory);
		std::swap(this->ref_counter, new_ref_counter);
		this->capacity = n;

		if (new_ref_counter.use_count() == 1)
		{
			destroy(new_memory, new_memory + this->size);
			operator delete(new_memory);
		}
	}

	const_iterator cbegin() const
	{
		Position<T> tmp(this->data, 0, this->shape);
		return const_iterator(tmp);
	}
	const_iterator cend() const
	{
		size_t fake_last_number = this->shape * this->shape;
		Position<T> tmp(this->data + this->size, fake_last_number, this->shape);
		return const_iterator(tmp);
	}

	iterator begin()
	{
		deep_copy(this->capacity);
		Position<T> tmp(this->data, 0, this->shape);
		return iterator(tmp);
	}
	iterator end()
	{
		deep_copy(this->capacity);
		size_t fake_last_number = this->shape * this->shape;
		Position<T> tmp(this->data + this->size, fake_last_number, this->shape);
		return iterator(tmp);
	}

	void fill(const T& a)
	{
		for (size_t i = 0; i < this->size; ++i) {
			construct(this->data + i, a);
		}
	}

	size_t get_shape() { return this->shape; }
	int get_ref_counter() { return this->ref_counter.use_count(); }
	size_t get_size() { return this->size; }
	size_t get_capacity() { return this->capacity; }
};

template<class T>
void print(Symmetric_Matrix<T> a)
{
	size_t count = 0;
	for (auto it : a)
	{
		std::cout << it << "\t";
		++count;
		if (count % a.get_shape() == 0) std::cout << std::endl;
	}
}

#endif  // _MATRIX_H_
