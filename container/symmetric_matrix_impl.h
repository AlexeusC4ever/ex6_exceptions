#ifndef SYM_MATRIX_IMPL_H_
#define SYM_MATRIX_IMPL_H_

template<class T>
void destroy(T* pT);

template<class FwdIter>
void destroy(FwdIter first, FwdIter last);

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

	~Symmetric_Matrix_impl() noexcept
	{
		if (ref_counter.use_count() == 1)
		{
			destroy(data, data + size);
			operator delete(data);
		}
	}
};

#endif //SYM_MATRIX_IMPL_H_
