#ifndef SYM_MATRIX_H_
#define SYM_MATRIX_H_

#include <memory>
#include <iostream>
#include <iterator>
#include <exception>
#include "position.h"
#include "symmetric_matrix_impl.h"
#include "symmetric_matrix_iterator.h"

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

template<class T1, class T2>
void construct_interval(T1* pT1_start, T1* pT1_end, T2&& rcT2)
{
	while (pT1_start != pT1_end) 
	{
		construct(pT1_start, std::forward<T2>(rcT2));
		++pT1_start;
	}
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

		//for (size_t i = this->size; i < required_size; ++i)
		//	construct(tmp.data + i, new_el);

		construct_interval(tmp.data + this->size, tmp.data + required_size, new_el);

		tmp.size = required_size;
		tmp.shape = this->shape + 1;

		this->swap(tmp);
	}


	void push_front(const T&& new_el)
	{
		size_t required_size = this->size + this->shape + 1;

		Symmetric_Matrix<T> tmp;
		tmp.reserve(required_size);

		//for (size_t i = 0; i < required_size - this->size; ++i)
		//	construct(tmp.data + i, new_el);

		construct_interval(tmp.data, tmp.data + required_size - this->size, new_el);

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
		Symmetric_Matrix<T> tmp(this->shape);
		for (size_t i = 0; i < this->size; ++i) {
			construct(tmp.data + i, a);
		}
		this->swap(tmp);
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

#endif  // SYM_MATRIX_H_
