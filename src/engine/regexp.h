/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#ifndef B2_REGEXP_H
#define B2_REGEXP_H

#include "config.h"

#include "mem.h"
#include "types.h"
#include "value.h"

#include <functional>
#include <memory>

#define NSUBEXP 10

namespace b2 { namespace regex {

// The resulting matches for a regex match. Expression 0 is the full match.
// And expressions [1,NSUBEXP] are the subexpressions matched.
struct regex_expr
{
	char const * startp[NSUBEXP] = { nullptr };
	char const * endp[NSUBEXP] = { nullptr };
};

// The compiled regex program to match with.
struct regex_prog;

struct program
{
	program() = default;
	program(program &&) = default;
	program(const program &) = default;
	explicit program(const char * pattern);

	// types
	struct result_iterator;

	void reset(const char * pattern);

	result_iterator search(const char * str_begin, const char * str_end);
	result_iterator search(const char * str_begin);

	private:
	const regex_prog * compiled = nullptr;

	static regex_prog & compile(const char * patter);
};

struct program::result_iterator
{
	using iterator_category = std::forward_iterator_tag;
	using value_type = value::str_view;
	using difference_type = std::ptrdiff_t;
	using pointer = const value_type *;
	using reference = const value_type &;

	result_iterator(const regex_prog & c, const char * b, const char * e);
	result_iterator(const result_iterator & o) = default;
	result_iterator(result_iterator && o) = default;

	inline result_iterator & operator++()
	{
		advance();
		return *this;
	}
	inline reference operator*() const { return match; }
	inline pointer operator->() const { return &match; }
	explicit inline operator bool() const { return match.str != nullptr; }
	inline value_type operator[](std::size_t i) const
	{
		return i <= NSUBEXP ? value_type { expressions.startp[i],
			std::size_t(expressions.endp[i] - expressions.startp[i]) }
							: value_type { nullptr, 0 };
	}

	private:
	const regex_prog * compiled = nullptr;
	regex_expr expressions;
	value_type match;
	value_type rest;

	void advance();
};

inline program::result_iterator program::search(
	const char * str_begin, const char * str_end)
{
	return result_iterator(*compiled, str_begin, str_end);
}

inline program::result_iterator program::search(const char * str_begin)
{
	return this->search(str_begin, str_begin + std::strlen(str_begin));
}

}} // namespace b2::regex

#endif
