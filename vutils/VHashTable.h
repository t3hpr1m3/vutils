/*======================================================================*
 *                                                                      *
 *  Copyright (C) 2004-2016 Josh Williams (vmizzle@gmail.com)           *
 *                                                                      *
 * Permission is hereby granted, free of charge, to any person          *
 * obtaining a copy of this software and associated documentation files *
 * (the "Software"), to deal in the Software without restriction,       *
 * including without limitation the rights to use, copy, modify, merge, *
 * publish, distribute, sublicense, and/or sell copies of the Software, *
 * and to permit persons to whom the Software is furnished to do so,    *
 * subject to the following conditions:                                 *
 *                                                                      *
 * The above copyright notice and this permission notice shall be       *
 * included in all copies or substantial portions of the Software.      *
 *                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  *
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   *
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    *
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
 * SOFTWARE.                                                            *
 *                                                                      *
 *======================================================================*/
#if !defined(__VHASHTABLE_H_INCLUDED__)
#define __VHASHTABLE_H_INCLUDED__

#include <vutils/VPlatform.h>

/* System Headers */

/* Local Headers */
#include <vutils/VVector.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using std::allocator;
using std::pair;
using std::vector;

#define HTABLE_TEMPLATE <class Key, class T, class H, class EQ, class A>
#define HTABLE_TEMP_SPEC <Key, T, H, EQ, A>
#define HTABLE_TYPE_SPEC VHashTable HTABLE_TEMP_SPEC

namespace VUtils {

/* Hash functions */
template<class T>
class Hash {
public:
	size_t operator()(const char *key) const {
		size_t res = 0;
		const char *p = key;
		while (*p)
			res = (res<<1)^*p++;
		return res;
	}
};

/* Comparison functions */
template <class T>
class EqualTo {
public:
	bool EqualTo::operator() (const char* left, const char *right) const {
		return (strcmp(left, right) == 0);
	}

};

template HTABLE_TEMPLATE
class VHashTableEntry {
public:
	VHashTableEntry(Key k, T v, VHashTableEntry *n)
		: m_Key(k), m_Val(v), m_bErased(false), m_Next(n) {}
	inline bool IsErased() const { return m_bErased; }
	inline void SetErased(bool erased = true) { m_bErased = erased; }
	inline void SetValue(Key v) { m_Val = v; }
	inline void SetNext(VHashTableEntry *n) { m_Next = n; }
	inline const Key GetKey() const { return m_Key; }
	inline T& Value() { return m_Val; }
	inline VHashTableEntry* Next() const { return m_Next; }

private:
	Key                 m_Key;
	T                   m_Val;
	bool                m_bErased;
	VHashTableEntry     *m_Next;
};

template <class Key, class T, class H = Hash<Key>, class EQ = EqualTo<Key>,
		 class A = allocator< pair<const Key, T> > >
class VHashTable;

template HTABLE_TEMPLATE
class VHashTableIterator {
public:
	VHashTableIterator() {}
	VHashTableIterator(HTABLE_TYPE_SPEC *table, int index) :
		m_Table(table), m_nIndex(index) {}

private:
	Key                 m_CurPos;
	long                m_nIndex;
	HTABLE_TYPE_SPEC    *m_Table;
};

template HTABLE_TEMPLATE
class VHashTable {
	typedef Key         key_type;
	typedef T           mapped_type;
	typedef pair<const Key, T> value_type;
	typedef A           allocator_type;
	typedef typename A::size_type size_type;
	typedef H           Hasher;
	typedef EQ          KeyEqual;
public:
	typedef VHashTableIterator HTABLE_TEMP_SPEC Iterator;

	friend class VHashTableIterator HTABLE_TEMP_SPEC;

public:
	VHashTable(const T &dv = T(), size_type n = 101, const H &hf = H(),
			const EQ &eq = EQ())
		: m_stNumErased(0), m_Hash(hf), m_EQ(eq), m_DefaultValue(dv) {
		m_vPointers.resize(n);
		m_fMaxLoad = 0.7f;
		m_fGrow = 1.6f;
		m_vValues.reserve((int)(m_fMaxLoad * m_vPointers.size()));
	}
	virtual ~VHashTable() {}

public:
	size_type           Size() const { return m_vValues.size() - m_stNumErased; }
	size_type           SizeReal() const { return m_vPointers.size(); }
	mapped_type&        operator[](const key_type &k) {
		size_type i = m_Hash(k) % m_vPointers.size();

		for (VHashTableEntry HTABLE_TEMP_SPEC *p = m_vPointers[i]; p; p = p->Next()) {
			if (m_EQ(k, p->GetKey())) {
				if (p->IsErased()) {
					p->SetErased(false);
					m_stNumErased--;
					p->SetValue(m_DefaultValue);
				}
				return p->Value();
			}
		}

		if (size_type(m_vPointers.size() * m_fMaxLoad) <= m_vValues.size()) {
			Resize((int)(m_vPointers.size() * m_fGrow));
			return operator[](k);
		}

		m_vValues.push_back(VHashTableEntry HTABLE_TEMP_SPEC(k, m_DefaultValue, m_vPointers[i]));
		m_vPointers[i] = &m_vValues.back();

		return m_vPointers[i]->Value();
	}

	/* Iterator            Begin() const; */
	/* Iterator            End() const; */
	void                Resize(size_type n) {
		size_type i = m_vValues.size();
		if (n <= m_vPointers.size())
			return;

		while (m_stNumErased) {
			if (m_vValues[--i].IsErased()) {
				m_vValues.erase(m_vValues.begin() + i);
				--m_stNumErased;
			}
		}

		m_vPointers.resize(n);
		fill(m_vPointers.begin(), m_vPointers.end(), (VHashTableEntry HTABLE_TEMP_SPEC *)NULL);
		m_vValues.reserve((int)(n * m_fMaxLoad));

		for (size_type i = 0; i < m_vValues.size(); i++) {
			size_type j = m_Hash(m_vValues[i].GetKey()) % m_vPointers.size();
			m_vValues[i].SetNext(m_vPointers[j]);
			m_vPointers[j] = &m_vValues[i];
		}
	}


private:

private:
	float               m_fMaxLoad;
	float               m_fGrow;
	size_type           m_stNumErased;
	Hasher              m_Hash;
	KeyEqual            m_EQ;
	size_type           m_stMaxIndex;

	const T             m_DefaultValue;

	VVector<VHashTableEntry HTABLE_TEMP_SPEC > m_vValues;
	VVector<VHashTableEntry HTABLE_TEMP_SPEC *> m_vPointers;
};

} // End Namespace

#endif // __VHASHTABLE_H_INCLUDED__
