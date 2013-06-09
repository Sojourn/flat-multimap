#ifndef TABLE_H
#define TABLE_H

#include <cassert>
#include <vector>
#include <iterator>
#include <algorithm>

// An associative multi-map which stores data in a sorted vector for fast lookups.
template<class Key, class Value>
class Table
{
public:
	typedef std::pair<Key, Value> Row_t;
	typedef typename std::vector<Row_t>::iterator iterator;
	typedef typename std::vector<Row_t>::const_iterator const_iterator;

	inline static bool less_than(const Row_t &rhs, const Row_t &lhs)
	{
		return rhs.first < lhs.first;
	}

	inline static bool compare(const Row_t &rhs, const Row_t &lhs)
	{
		return rhs.first == lhs.first;
	}

	// O(n)
	inline iterator insert(const Key &key, Value value = Value())
	{
		auto index = std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, value), less_than);
		return _rows.insert(index, Row_t(key, std::move(value)));
	}

	// O(nlog(n))
	inline void insertAll(std::vector<Row_t> &&rows)
	{
		_rows.insert(
			std::end(_rows),
			std::make_move_iterator(std::begin(rows)),
			std::make_move_iterator(std::end(rows)));
		std::sort(std::begin(_rows), std::end(_rows));
	}

	// O(n)
	inline void remove(const Key &key)
	{
		auto index = std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
		assert((index != _rows.cend()) && (index->first == key));
		
		_rows.erase(index);
	}

	inline void erase(const iterator &index)
	{
		_rows.erase(index);
	}

	inline void erase(const_iterator &index)
	{
		_rows.erase(index);
	}

	// O(log(n))
	inline bool contains(const Key &key) const
	{
		auto index = std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
		return (index != _rows.cend()) && (index->first == key);
	}

	// O(log(n))
	inline size_t count(const Key &key) const
	{
		auto range = equal_range(key);
		return range.second - range.first;
	}

	// O(log(n))
	inline Value &operator[](const Key &key)
	{
		auto index = std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
		if((index != _rows.end()) && index->first == key)
		{
			return index->second;
		}
		else
		{
			return _rows.insert(index, Row_t(key, Value()))->second;
		}
	}

	// O(log(n))
	inline const Value &operator[](const Key &key) const
	{
		auto index = std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
		assert((index != _rows.cend()) && (index->first == key));

		return index->second;
	}

	// O(log(n))
	inline iterator lower_bound(const Key &key)
	{
		return std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
	}

	// O(log(n))
	inline iterator upper_bound(const Key &key)
	{
		return std::upper_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
	}

	// O(log(n))
	inline const_iterator lower_bound(const Key &key) const
	{
		return std::lower_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
	}

	// O(log(n))
	inline const_iterator upper_bound(const Key &key) const
	{
		return std::upper_bound(std::begin(_rows), std::end(_rows), Row_t(key, Value()), less_than);
	}

	// O(log(n))
	inline std::pair<iterator, iterator> equal_range(const Key &key)
	{
		return std::make_pair(lower_bound(key), upper_bound(key));
	}

		// O(log(n))
	inline std::pair<const_iterator, const_iterator> equal_range(const Key &key) const
	{
		return std::make_pair(lower_bound(key), upper_bound(key));
	}

	// O(n)
	inline const void clear()
	{
		_rows.clear();
	}

	inline size_t size() const { return _rows.size(); }
	inline iterator begin() { return _rows.begin(); }
	inline iterator end() { return _rows.end(); }
	inline const_iterator cbegin() const { return _rows.begin(); }
	inline const_iterator cend() const { return _rows.end(); }

private:
	std::vector<Row_t> _rows;
};

#endif // TABLE_H