/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2022 René Ferdinand Rivera Morell
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

/*
 * lists.h - the LIST structure and routines to manipulate them
 *
 * The whole of jam relies on lists of objects as a datatype. This module, in
 * conjunction with object.c, handles these relatively efficiently.
 *
 * Structures defined:
 *
 *  LIST - list of OBJECTs
 *  LOL - list of LISTs
 *
 * External routines:
 *
 *  list_append() - append a list onto another one, returning total
 *  list_new() - tack an object onto the end of a list of objects
 *  list_copy() - copy a whole list of objects
 *  list_sublist() - copy a subset of a list of objects
 *  list_free() - free a list of objects
 *  list_print() - print a list of objects to stdout
 *  list_length() - return the number of items in the list
 *
 *  lol_init() - initialize a LOL (list of lists)
 *  lol_add() - append a LIST onto an LOL
 *  lol_free() - free the LOL and its LISTs
 *  lol_get() - return one of the LISTs in the LOL
 *  lol_print() - debug print LISTS separated by ":"
 */

#ifndef LISTS_DWA20011022_H
#define LISTS_DWA20011022_H

#include "config.h"

#include "jam.h"
#include "object.h"
#include "output.h"

/*
 * LIST - list of strings
 */

struct LIST
{
	union
	{
		int32_t size;
		struct LIST * next;
		OBJECT * align;
	} impl;

	LIST() { this->impl.next = nullptr; }
};

typedef LIST * list_ptr;
typedef OBJECT ** LISTITER;

/*
 * LOL - list of LISTs
 */

#define LOL_MAX 19
typedef struct _lol
{
	int32_t count;
	LIST * list[LOL_MAX];
} LOL;

LIST * list_new(OBJECT * value);
LIST * list_append(LIST * destination, LIST * source);
LIST * list_copy(LIST *);
LIST * list_copy_range(LIST * destination, LISTITER first, LISTITER last);
void list_free(LIST * head);
LIST * list_push_back(LIST * head, OBJECT * value);
void list_print(LIST *);
inline int32_t list_length(LIST * l) { return l ? l->impl.size : 0; }
LIST * list_sublist(LIST *, int32_t start, int32_t count);
LIST * list_pop_front(LIST *);
LIST * list_sort(LIST *);
LIST * list_unique(LIST * sorted_list);
int32_t list_in(LIST *, OBJECT * value);
LIST * list_reverse(LIST *);
int32_t list_cmp(LIST * lhs, LIST * rhs);
int32_t list_is_sublist(LIST * sub, LIST * l);
void list_done();

#define L0 ((LIST *)0)

inline LISTITER list_begin(LIST * l)
{
	return l ? (LISTITER)((char *)l + sizeof(LIST)) : 0;
}
inline LISTITER list_end(LIST * l)
{
	return l ? list_begin(l) + l->impl.size : 0;
}
inline LISTITER list_next(LISTITER it) { return ((it) + 1); }
inline LISTITER list_prev(LISTITER it) { return ((it)-1); }
inline OBJECT *& list_item(LISTITER it) { return (*(it)); }
inline bool list_empty(LIST * l) { return ((l) == L0); }
inline OBJECT *& list_front(LIST * l) { return list_item(list_begin(l)); }
inline OBJECT *& list_item(LIST * l, int i)
{
	return list_begin(l)[i < 0 ? l->impl.size + i : i];
}
void lol_add_err();
inline void lol_add(LOL * lol, LIST * l)
{
	if (lol->count < LOL_MAX)
	{
		lol->list[lol->count++] = l;
		return;
	}
	lol_add_err();
}
inline void lol_init(LOL * lol) { lol->count = 0; }
inline void lol_free(LOL * lol)
{
	int32_t i;
	for (i = 0; i < lol->count; ++i) list_free(lol->list[i]);
	lol->count = 0;
}
inline LIST * lol_get(LOL * lol, int32_t i)
{
	return i < lol->count ? lol->list[i] : L0;
}
inline void lol_print(LOL * lol)
{
	int32_t i;
	for (i = 0; i < lol->count; ++i)
	{
		if (i) out_printf(" : ");
		list_print(lol->list[i]);
	}
}
void lol_build(LOL * lol, char const ** elements);

namespace b2 {

struct list_ref;

struct list_cref
{
	struct iterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = OBJECT *;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type *;
		using reference = value_type &;

		inline explicit iterator(LISTITER i)
			: list_i(i)
		{}

		inline iterator operator++()
		{
			list_i = list_next(list_i);
			return *this;
		}
		inline iterator operator++(int)
		{
			iterator result { *this };
			list_i = list_next(list_i);
			return result;
		}
		inline iterator operator--()
		{
			list_i = list_prev(list_i);
			return *this;
		}
		inline iterator operator--(int)
		{
			iterator result { *this };
			list_i = list_prev(list_i);
			return result;
		}
		inline bool operator==(iterator other) const
		{
			return list_i == other.list_i;
		}
		inline bool operator!=(iterator other) const
		{
			return list_i != other.list_i;
		}
		inline reference operator*() const { return list_item(list_i); }
		inline pointer operator->() const { return &list_item(list_i); }
		inline difference_type operator-(const iterator & b) const
		{
			return list_i - b.list_i;
		}

		private:
		LISTITER list_i;

		// MSVC secure/debug std asks for comparisons of iterators.
		friend inline bool operator<(const iterator & a, const iterator & b)
		{
			return a.list_i < b.list_i;
		}
	};

	friend struct iterator;

	inline list_cref() = default;
	inline list_cref(const list_cref &) = default;
	inline list_cref(list_cref && other)
		: list_obj(std::move(other.list_obj))
	{}
	inline explicit list_cref(LIST * l)
		: list_obj(l)
	{}
	inline list_cref & operator=(const list_cref &) = default;

	inline iterator begin() const { return iterator(list_begin(list_obj)); }
	inline iterator end() const { return iterator(list_end(list_obj)); }
	inline bool empty() const { return list_empty(list_obj) || length() == 0; }
	inline int32_t length() const { return list_length(list_obj); }
	inline LIST * operator*() const { return list_obj; }
	inline OBJECT *& operator[](int i) const { return list_item(list_obj, i); }
	inline bool contains(value_ref a) const
	{
		for (auto b : *this)
			if (a == b) return true;
		return false;
	}
	list_ref slice(int i, int j = -1) const;
	inline bool operator==(const list_cref & b) const
	{
		if (length() != b.length()) return false;
		iterator b_i = b.begin();
		for (value_ref v : *this)
			if (v != value_ref(*(b_i++))) return false;
		return true;
	}
	bool operator==(const list_ref & b) const;

	protected:
	LIST * list_obj = nullptr;
};

struct list_ref : private list_cref
{
	using list_cref::iterator;
	using list_cref::begin;
	using list_cref::end;
	using list_cref::empty;
	using list_cref::length;
	using list_cref::operator*;
	using list_cref::operator[];
	using list_cref::contains;
	using list_cref::operator==;

	inline list_ref() = default;
	inline list_ref(list_ref && other)
		: list_cref(other.release())
	{}
	inline list_ref(const list_cref & other)
		: list_cref(list_copy(*other))
	{}
	inline list_ref(const list_ref & other)
		: list_cref(list_copy(other.list_obj))
	{}
	inline explicit list_ref(value_ref o)
		: list_cref(list_new(o))
	{}
	inline explicit list_ref(LIST * l, bool own = false)
		: list_cref(own ? l : list_copy(l))
	{}
	inline list_ref(iterator i, const iterator & e)
	{
		for (; i != e; ++i) this->push_back(value::copy(*i));
	}

	inline ~list_ref() { reset(); }
	inline LIST * release()
	{
		LIST * r = list_obj;
		list_obj = nullptr;
		return r;
	}
	inline list_ref & append(const list_ref & other)
	{
		list_obj = list_append(list_obj, list_copy(*other));
		return *this;
	}
	inline list_ref & append(list_cref other)
	{
		list_obj = list_append(list_obj, list_copy(*other));
		return *this;
	}
	inline void reset(LIST * new_list = nullptr)
	{
		std::swap(list_obj, new_list);
		if (new_list) list_free(new_list);
	}
	inline list_ref & pop_front()
	{
		list_obj = list_pop_front(list_obj);
		return *this;
	}
	inline list_ref & push_back(OBJECT * value)
	{
		list_obj = list_push_back(list_obj, value);
		return *this;
	}
	template <typename... T>
	inline list_ref & push_back(T... value)
	{
		list_obj = list_push_back(list_obj, value::make(value...));
		return *this;
	}
	template <typename T>
	inline list_ref & operator+(T value)
	{
		return push_back(value);
	}
	inline list_ref & operator+(const list_ref & other)
	{
		return append(other);
	}
	inline list_ref & operator+(const list_cref & other)
	{
		return append(other);
	}
	inline list_ref & operator=(list_ref && other)
	{
		reset(other.list_obj);
		other.list_obj = nullptr;
		return *this;
	}
	inline list_ref & slice(int i, int j = -1)
	{
		list_obj
			= list_sublist(list_obj, i, (j < 0 ? length() + j : j) - i + 1);
		return *this;
	}
};

inline list_ref list_cref::slice(int i, int j) const
{
	return list_ref(
		list_sublist(list_obj, i, (j < 0 ? length() + j : j) - i + 1), true);
}

inline bool list_cref::operator==(const list_ref & b) const
{
	return *this == list_cref(*b);
}

struct lists
{
	inline lists() { lol_init(&lol); }
	inline ~lists() { lol_free(&lol); }
	inline void push_back(const list_cref & l) { lol_add(&lol, list_copy(*l)); }
	inline list_cref operator[](int32_t i) const
	{
		return list_cref(lol_get(&lol, i));
	}
	inline bool empty() const { return length() == 0; }
	inline int32_t length() const { return lol.count; }
	inline void print() const { lol_print(&lol); }
	inline operator LOL *() const { return &lol; }
	inline lists & operator|(const list_cref & l)
	{
		push_back(l);
		return *this;
	}
	inline lists & operator|(LIST * l) { return (*this) |= list_cref(l); }
	inline lists & operator|(list_ref && l) { return (*this) |= std::move(l); }
	inline lists & operator|=(list_ref && l)
	{
		list_ref to_add(std::move(l));
		lol_add(&lol, to_add.release());
		return *this;
	}
	// inline list_ref

	private:
	mutable LOL lol;
};

} // namespace b2

#endif
