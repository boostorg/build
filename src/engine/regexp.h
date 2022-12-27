/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#ifndef REGEXP_DWA20011023_H
#define REGEXP_DWA20011023_H

#include "config.h"

#include "mem.h"
#include "types.h"
#include "value.h"

#include <functional>
#include <memory>
#include <string>

#define NSUBEXP 10
typedef struct regexp
{
	char const * startp[NSUBEXP];
	char const * endp[NSUBEXP];
	char regstart; /* Internal use only. */
	char reganch; /* Internal use only. */
	char * regmust; /* Internal use only. */
	int32_t regmlen; /* Internal use only. */
	char program[1]; /* Unwarranted chumminess with compiler. */
} regexp;

regexp * regcomp(char const * exp);
int32_t regexec(regexp * prog, char const * string);
void regerror(char const * s);

/*
 * The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte.
 */
#define MAGIC 0234

namespace b2 { namespace regex {
using regex_ptr = std::unique_ptr<regexp, std::function<void(regexp *)>>;

inline regex_ptr make_regex(const string_t & pattern)
{
	// TODO: Fix regcomp to not const-cast pattern.
	return regex_ptr(
		regcomp(pattern.c_str()), [](regexp * r) { BJAM_FREE(r); });
}

inline regex_ptr make_regex(const char * pattern)
{
	// TODO: Fix regcomp to not const-cast pattern.
	return regex_ptr(regcomp(pattern), [](regexp * r) { BJAM_FREE(r); });
}

struct program
{
	program(const char * pattern);

	// types
	struct result_iterator;

	result_iterator search(const char * str_begin, const char * str_end);
	result_iterator search(const char * str_begin);

	private:
	regex_ptr compiled;
};

struct program::result_iterator
{
	using iterator_category = std::forward_iterator_tag;
	using value_type = value::str_view;
	using difference_type = std::ptrdiff_t;
	using pointer = value_type *;
	using reference = value_type &;

	result_iterator(regexp & c, const char * b, const char * e);
	result_iterator(const result_iterator & o) = default;
	result_iterator(result_iterator && o) = default;

	inline result_iterator & operator++()
	{
		advance();
		return *this;
	}
	reference operator*() const;
	pointer operator->() const;
	explicit inline operator bool() const { return match_begin != nullptr; }

	private:
	regexp & compiled;
	const char * match_begin = nullptr;
    const char * match_end = nullptr;

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
