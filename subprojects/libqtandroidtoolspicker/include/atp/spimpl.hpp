/**
 * @file
 * Simplified implementation of the Smart PIMPL.
 */

// NOLINTBEGIN(readability-identifier-naming, readability-identifier-length)

#pragma once

#include <cassert>
#include <memory>
#include <type_traits>

/**
 * @brief A simplified Smart Pointer to IMPLementation.
 *
 * This is partly inspired by
 * [spimpl](http://oliora.github.io/2015/12/29/pimpl-and-rule-of-zero.html)
 * (GitHub repo [here](https://github.com/oliora/samples)) and by
 * [`std::experimental::propagate_const`](https://en.cppreference.com/w/cpp/experimental/propagate_const).
 */
namespace ATP::spimpl
{
/**
 * @brief Implementation details.
 */
namespace details
{
/**
 * @brief Default copy function for @ref impl_ptr contents.
 */
template <class T> T * default_copy(T * src)
{
	// NOLINTNEXTLINE(bugprone-sizeof-expression)
	static_assert(sizeof(T) > 0, "default_copy cannot copy incomplete type");
	static_assert(!std::is_void<T>::value,
				  "default_copy cannot copy incomplete type");
	// The following macro is for developers only. Meson does not have an option
	// to enable it. It has to be specified through CXXFLAGS. DO NOT SET IN
	// RELEASE BUILDS!
#ifdef ATPICKER_SPIMPL_COPY_ERROR
	std::terminate();
#endif
	return new T(*src);
}

/**
 * @brief Default deleter function for @ref impl_ptr contents.
 */
template <class T> void default_delete(T * p) noexcept
{
	// NOLINTNEXTLINE(bugprone-sizeof-expression)
	static_assert(sizeof(T) > 0,
				  "default_delete cannot delete incomplete type");
	static_assert(!std::is_void<T>::value,
				  "default_delete cannot delete incomplete type");
	delete p;
}

/**
 * @brief Default deleter type definition.
 */
template <class T> struct default_deleter
{
	/**
	 * @brief Deleter function pointer.
	 * @showinitializer
	 */
	using type = void (*)(T *);
};

/**
 * @brief See @ref default_deleter.
 */
template <class T> using default_deleter_t = typename default_deleter<T>::type;

/**
 * @brief Default copy function type definition.
 */
template <class T> struct default_copier
{
	/**
	 * @brief Copier function pointer.
	 * @showinitializer
	 */
	using type = T * (*)(T *);
};

/**
 * @brief See @ref default_copier.
 */
template <class T> using default_copier_t = typename default_copier<T>::type;
} // namespace details

/**
 * @brief Simplified PIMPL.
 *
 * This class is similar to `spimpl::impl_ptr` in
 * [oliora/samples](https://github.com/oliora/samples), but it doesn't have as
 * many methods. Many methods/constructors in oliora/samples are:
 *
 * 1. not applicable for PIMPL pointers
 *
 *    Types stored in PIMPL pointers do not often use inheritance, so making
 *    sure that a impl pointer storing an instance of a base class can store a
 *    pointer to derived class doesn't have practical use.
 *
 *    Many methods/constructors useful in generic pointers such as
 *    `std::unique_ptr` are not useful for PIMPL pointers.
 * 2. implementing these is not trivial
 *
 *    oliora/samples contains several bugs in these lesser used functions.
 *    Fixing these would require extensive testing (it's easy to run into
 *    unintended edge cases with these types of template substitution/SFINAE).
 * 3. it's simpler to implement these when needed rather than retroactively
 *    fixing it
 *
 *    This is a template class, so any changes to it are ABI incompatible most
 *    of the time. Changing the definition of a template function would require
 *    all dependant libraries/programs to be recompiled.
 *
 *    Having a strict base and then gradually expanding it (when necessary and
 *    useful) is a much more stable approach.
 */
template <class T> class impl_ptr
{
private:
	static_assert(!std::is_array<T>::value,
				  "impl_ptr specialization for arrays is not implemented");

public:
	/**
	 * @brief Raw pointer type of `T`.
	 * @showinitializer
	 */
	using pointer = T *;
	/**
	 * @brief A `const` version of @ref pointer.
	 * @showinitializer
	 */
	using const_pointer = typename std::add_const<T>::type *;
	/**
	 * @brief A reference type of `T`.
	 * @showinitializer
	 */
	using reference = T &;
	/**
	 * @brief A `const` version of @ref reference.
	 * @showinitializer
	 */
	using const_reference = typename std::add_const<T>::type &;
	/**
	 * @brief Type alias to element type (equivalent to `T` template parameter).
	 * @showinitializer
	 */
	using element_type = T;
	/**
	 * @brief Deleter type used in destructor.
	 * @showinitializer
	 */
	using deleter_type = details::default_deleter_t<T>;
	/**
	 * @brief Copier type used in @ref impl_ptr(const impl_ptr &).
	 * @showinitializer
	 */
	using copier_type = details::default_copier_t<T>;
	/**
	 * @brief A `std::unique_ptr` type holding `T`.
	 * @showinitializer
	 */
	using unique_ptr_type = std::unique_ptr<T, deleter_type>;

	/**
	 * @brief Construct an empty unique pointer.
	 */
	constexpr impl_ptr() noexcept
		// It doesn't matter that a default constructed function pointer is used
		// here, because the pointer is empty.
		: ptr_(nullptr, deleter_type{})
	{
	}

	/**
	 * @brief Construct an empty unique pointer.
	 */
	constexpr impl_ptr(std::nullptr_t) noexcept : impl_ptr() {}

	/**
	 * @brief Construct through a pointer.
	 */
	impl_ptr(pointer u) noexcept : ptr_(u, &details::default_delete<T>) {}

	/**
	 * @brief Default move constructor.
	 */
	impl_ptr(impl_ptr && r) noexcept = default;

	/**
	 * @brief Move construct from a `std::unique_ptr`.
	 */
	impl_ptr(std::unique_ptr<element_type> && u) noexcept
		: ptr_(u.release(), &details::default_delete<T>)
	{
	}

	/**
	 * @brief Copy constructor.
	 *
	 * @throws std::bad_alloc If a call to `new` fails.
	 */
	impl_ptr(const impl_ptr & other)
		: ptr_(other ? details::default_copy<T>(other.ptr_.get()) : nullptr,
			   &details::default_delete<T>)
	{
	}

	/**
	 * @brief Default move assignment operator.
	 */
	impl_ptr & operator=(impl_ptr && r) noexcept = default;

	/**
	 * @brief Assign a `std::unique_ptr`.
	 */
	impl_ptr & operator=(std::unique_ptr<element_type> && u) noexcept
	{
		ptr_ = unique_ptr_type(u.release(), &details::default_delete<T>);
		return *this;
	}

	/**
	 * @brief Copy assignment operator.
	 *
	 * @note This function has a strong exception guarantee.
	 *
	 * @throws std::bad_alloc If a call to `new` fails.
	 */
	impl_ptr & operator=(const impl_ptr & other)
	{
		impl_ptr copy = other;

		using std::swap;
		swap(*this, copy);
		return *this;
	}

	/**
	 * @brief Deference operator.
	 *
	 * @warning The behavior is undefined if @ref get() == `nullptr`.
	 */
	reference operator*() { return *ptr_; }
	/**
	 * @brief Const deference operator.
	 *
	 * @copydetails operator*()
	 */
	const_reference operator*() const { return *ptr_; }

	/**
	 * @brief Member access operator.
	 *
	 * @copydetails operator*()
	 */
	pointer operator->() noexcept { return get(); }
	/**
	 * @brief Const member access operator.
	 *
	 * @copydetails operator*()
	 */
	const_pointer operator->() const noexcept { return get(); }

	/**
	 * @brief Function to access the underlying raw pointer.
	 *
	 * @returns
	 * Pointer to stored object if impl_ptr isn't empty, `nullptr` otherwise.
	 */
	pointer get() noexcept { return ptr_.get(); }
	/**
	 * @brief Const function to access the underlying raw pointer.
	 *
	 * @copydetails get()
	 */
	const_pointer get() const noexcept { return ptr_.get(); }

	/**
	 * @brief Swap function.
	 */
	void swap(impl_ptr & u) noexcept
	{
		using std::swap;
		ptr_.swap(u.ptr_);
	}

	/**
	 * @brief Give up the ownership of the pointer and return it.
	 *
	 * The caller is responsible for cleaning up the object (e.g. by use of @ref
	 * get_deleter()).
	 */
	pointer release() noexcept { return ptr_.release(); }

	/**
	 * @brief Give up the ownership of the `std::unique_ptr` and return it.
	 */
	unique_ptr_type release_unique() noexcept { return std::move(ptr_); }

	/**
	 * @brief Does unique_impl_ptr contain data?
	 *
	 * False if the unique_impl_ptr is empty (comparable to `nullptr`).
	 */
	explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }

	/**
	 * @brief Return reference to deleter.
	 */
	typename std::remove_reference<deleter_type>::type & get_deleter() noexcept
	{
		return ptr_.get_deleter();
	}
	/**
	 * @brief Return const reference to deleter.
	 */
	const typename std::remove_reference<deleter_type>::type &
	get_deleter() const noexcept
	{
		return ptr_.get_deleter();
	}

private:
	/**
	 * @brief Underlying `std::unique_ptr` holding the pimpl.
	 */
	unique_ptr_type ptr_;

	// This is needed to allow move ctors from compatible simple_impl_ptrs to
	// access private members of compatible impl ptrs.
	// template <typename, typename> friend class simple_impl_ptr;
};

/**
 * @relates impl_ptr
 * @brief External swap function for @ref impl_ptr.
 *
 * This is a standard external swap function present to satisfy ADL lookup.
 *
 * @sa @ref impl_ptr::swap()
 */
template <class T> inline void swap(impl_ptr<T> & l, impl_ptr<T> & r) noexcept
{
	l.swap(r);
}

/**@{*/
/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T1, class T2>
inline bool operator==(const impl_ptr<T1> & l, const impl_ptr<T2> & r)
{
	return l.get() == r.get();
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T1, class T2>
inline bool operator!=(const impl_ptr<T1> & l, const impl_ptr<T2> & r)
{
	return !(l == r);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T1, class T2>
inline bool operator<(const impl_ptr<T1> & l, const impl_ptr<T2> & r)
{
	using P1 = typename impl_ptr<T1>::const_pointer;
	using P2 = typename impl_ptr<T2>::const_pointer;
	using CT = typename std::common_type<P1, P2>::type;
	return std::less<CT>()(l.get(), r.get());
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T1, class T2>
inline bool operator>(const impl_ptr<T1> & l, const impl_ptr<T2> & r)
{
	return r < l;
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T1, class T2>
inline bool operator<=(const impl_ptr<T1> & l, const impl_ptr<T2> & r)
{
	return !(r < l);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T1, class T2>
inline bool operator>=(const impl_ptr<T1> & l, const impl_ptr<T2> & r)
{
	return !(l < r);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T>
inline bool operator==(const impl_ptr<T> & p, std::nullptr_t) noexcept
{
	return !p;
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T>
inline bool operator==(std::nullptr_t, const impl_ptr<T> & p) noexcept
{
	return !p;
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T>
inline bool operator!=(const impl_ptr<T> & p, std::nullptr_t) noexcept
{
	return static_cast<bool>(p);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T>
inline bool operator!=(std::nullptr_t, const impl_ptr<T> & p) noexcept
{
	return static_cast<bool>(p);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator<(const impl_ptr<T> & l, std::nullptr_t)
{
	using P = typename impl_ptr<T>::const_pointer;
	return std::less<P>()(l.get(), nullptr);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator<(std::nullptr_t, const impl_ptr<T> & p)
{
	using P = typename impl_ptr<T>::const_pointer;
	return std::less<P>()(nullptr, p.get());
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator>(const impl_ptr<T> & p, std::nullptr_t)
{
	return nullptr < p;
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator>(std::nullptr_t, const impl_ptr<T> & p)
{
	return p < nullptr;
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator<=(const impl_ptr<T> & p, std::nullptr_t)
{
	return !(nullptr < p);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator<=(std::nullptr_t, const impl_ptr<T> & p)
{
	return !(p < nullptr);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator>=(const impl_ptr<T> & p, std::nullptr_t)
{
	return !(p < nullptr);
}

/**
 * @relates impl_ptr
 * @brief Comparison operators for @ref impl_ptr.
 */
template <class T> inline bool operator>=(std::nullptr_t, const impl_ptr<T> & p)
{
	return !(nullptr < p);
}
/**@}*/

/**
 * @relatesalso impl_ptr
 * @brief Helper function to construct @ref impl_ptr.
 *
 * This function is similar in purpose to `std::make_unique()`.
 */
template <class T, class... Args> inline impl_ptr<T> make_impl(Args &&... args)
{
	return impl_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace QtAndroidToolsPicker::spimpl

namespace std
{
/**
 * @brief Extension of `std::hash` for @ref
 * UniSettings::spimpl::impl_ptr.
 */
template <class T> struct hash<ATP::spimpl::impl_ptr<T>>
{
	/**
	 * @brief Argument type of @ref operator()().
	 */
	using argument_type = ATP::spimpl::impl_ptr<T>;
	/**
	 * @brief Return type of @ref operator()().
	 */
	using result_type = size_t;

	/**
	 * @brief Return the hash of a @ref UniSettings::spimpl::impl_ptr.
	 */
	result_type operator()(const argument_type & p) const noexcept
	{
		return hash<typename argument_type::const_pointer>()(p.get());
	}
};
} // namespace std

// NOLINTEND(readability-identifier-naming, readability-identifier-length)
