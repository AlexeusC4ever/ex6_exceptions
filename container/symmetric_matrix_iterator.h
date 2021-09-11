#ifndef SYM_MATRIX_ITER_H_
#define SYM_MATRIX_ITER_H_

template<class T>
class Sym_iter_base
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;


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
	using pointer = T*;

	Sym_Mat_Iterator(Position<T> pos_) :
		Sym_iter_base<T>(pos_) {}

	reference operator*() const { return this->pos.dereference(); }

	reference operator[](int n) const { auto tmp = *this; tmp += n; return *tmp; }

	pointer operator->() { return this->pos.pointer(); }
};

#endif
