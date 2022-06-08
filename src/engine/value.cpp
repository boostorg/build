/*
 * Copyright 2022 René Ferdinand Rivera Morell
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "jam.h"
#include "mem.h"
#include "object.h"
#include "output.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <unordered_set>
#include <vector>

static inline void hash_fnv1a(
	std::uint64_t & value, void * data, std::size_t size)
{
	value = 0xcbf29ce484222325;
	std::uint8_t * d = static_cast<std::uint8_t *>(data);
	std::uint8_t * e = d + size;
	while (d != e) value = (value * 0x100000001B3) ^ *(d++);
}
inline void hash_fnv1a(std::uint32_t & value, void * data, std::size_t size)
{
	value = 0x811c9dc5;
	std::uint8_t * d = static_cast<std::uint8_t *>(data);
	std::uint8_t * e = d + size;
	while (d != e) value = (value * 0x1000193) ^ *(d++);
}
#if 0
static inline void hash_fnv1a(std::uint32_t & value32,
	std::uint64_t & value64,
	void * data,
	std::size_t size)
{
	value32 = 0x811c9dc5;
	value64 = 0xcbf29ce484222325;
	std::uint8_t * d = static_cast<std::uint8_t *>(data);
	std::uint8_t * e = d + size;
	while (d != e)
	{
		value32 = (value32 * 0x1000193) ^ *d;
		value64 = (value64 * 0x100000001B3) ^ *d;
		d += 1;
	}
}
#endif
static inline bool str_view_equal(const char * str_a,
	std::size_t size_a,
	const char * str_b,
	std::size_t size_b)
{
	return (str_a == str_b)
		|| ((str_a != nullptr) && (str_b != nullptr) && (size_a == size_b)
			&& std::equal(str_a, str_a + size_a, str_b));
}

namespace b2 {

struct value_base : value
{
	virtual type get_type() const override { return type::null; }
	virtual str_view as_string() const override { return { nullptr, 0 }; }
	virtual double as_number() const override
	{
		using nl = std::numeric_limits<double>;
		return nl::has_quiet_NaN ? nl::quiet_NaN() : nl::lowest();
	}
	virtual object * as_object() const override { return nullptr; }
};

struct value_str : value_base
{
	static value_str * make(const char * v, std::size_t n)
	{
		std::size_t m = n + 1 < sizeof(value_str::value)
			? sizeof(value_str::value)
			: n + 1;
		void * o
			= BJAM_MALLOC(sizeof(value_str) - sizeof(value_str::value) + m);
		return b2::jam::ctor_ptr<value_str>(o, v, n);
	}
	value_str(const char * v, std::size_t n)
		: size(n)
	{
		char * vv = value;
		std::memcpy(vv, v, n);
		vv[n] = '\0';
		hash_fnv1a(hash64, vv, n);
	}
	virtual type get_type() const override { return type::string; }
	virtual bool equal_to(const value & o) const override
	{
		return str_view_equal(
			value, size, o.as_string().str, o.as_string().size);
	}
	virtual str_view as_string() const override { return { value, size }; }

	std::size_t size;
	char value[8];
};

struct value_number : value_base
{
	static value_number * make(double v)
	{
		return b2::jam::make_ptr<value_number>(v);
	}
	value_number(double v)
		: value(v)
	{
		hash_fnv1a(hash64, (void *)&value, sizeof(value));
	}
	virtual type get_type() const override { return type::number; }
	virtual bool equal_to(const value & o) const override
	{
		return as_number() == o.as_number();
	}
	virtual double as_number() const override { return value; }
	double value;
};

struct value_object : value_base
{
	static value_object * make(object * v)
	{
		return b2::jam::make_ptr<value_object>(v);
	}
	value_object(object * v)
		: value(v)
	{
		hash_fnv1a(hash64, (void *)&v, sizeof(v));
	}
	virtual type get_type() const override { return type::object; }
	virtual bool equal_to(const value & o) const override
	{
		return as_object() == o.as_object();
	}
	virtual object * as_object() const override { return value.get(); }
	std::unique_ptr<object> value;
};

struct value_str_view : value_base
{
	value_str_view(const char * v, std::size_t n)
		: value(v)
		, size(n)
	{
		hash_fnv1a(hash64, (void *)v, n);
	}
	virtual ~value_str_view() {}
	virtual bool equal_to(const value & o) const override
	{
		return str_view_equal(
			value, size, o.as_string().str, o.as_string().size);
	}
	virtual str_view as_string() const override { return { value, size }; }

	const char * value = nullptr;
	std::size_t size = 0;
};

struct value_hash_f
{
	inline std::size_t operator()(const value * o) const { return o->hash64; }
};

struct value_eq_f
{
	inline bool operator()(const value * a, const value * b) const
	{
		return (a->hash64 == b->hash64) && a->equal_to(*b);
	}
};

using value_cache_t = std::unordered_set<value *, value_hash_f, value_eq_f>;

static value_cache_t value_cache;

value_ptr value::make(const char * string, std::size_t size)
{
	value_str_view test_val(string, size);
	auto existing = value_cache.find(&test_val);
	if (existing != value_cache.end()) return *existing;
	value_ptr result = value_str::make(string, size);
	value_cache.insert(result);
	return result;
}

value_ptr value::make(object * obj)
{
	value_object test_val(obj);
	auto existing = value_cache.find(&test_val);
	if (existing != value_cache.end()) return *existing;
	value_ptr result = value_object::make(test_val.value.release());
	value_cache.insert(result);
	return result;
}

value_ptr value::make(double v)
{
	value_number test_val(v);
	auto existing = value_cache.find(&test_val);
	if (existing != value_cache.end()) return *existing;
	value_ptr result = value_number::make(v);
	value_cache.insert(result);
	return result;
}

void value::done(void)
{
	for (value * o : value_cache)
	{
		b2::jam::free_ptr(o);
	}
	value_cache.clear();
}

} // namespace b2
