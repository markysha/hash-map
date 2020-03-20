#include <bits/stdc++.h>

using namespace std;

template <typename KeyType, typename ValueType, class Hash = hash<KeyType>>
class HashMap {
private: 
    vector<vector<pair<pair<KeyType, ValueType>, size_t>>> data;
    vector<pair<size_t, size_t>> elements;
    Hash Hasher;
public:
    class iterator {
        typedef pair<const KeyType, ValueType> value_type;
        typedef pair<const KeyType, ValueType>& reference;
        typedef pair<const KeyType, ValueType>* pointer;
        typedef forward_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;
    public:
        HashMap<KeyType, ValueType, Hash>* mother;
        vector<pair<size_t, size_t>>::iterator elementsIterator;
        bool operator==(const iterator& other) const {
            return elementsIterator == other.elementsIterator;
        }
        bool operator !=(const iterator& other) const {
            return !(operator==(other));
        }
        iterator operator ++(int) {
            auto buf = *this;
            ++elementsIterator;
            return buf;
        }
        iterator& operator ++() {
            ++elementsIterator;
            return *this;
        }
        pointer operator ->() {
            auto i = *elementsIterator;
            return reinterpret_cast<pointer>(&mother->data[i.first][i.second].first);
        }
        reference operator *() {
            auto i = *elementsIterator;
            return *reinterpret_cast<pointer>(&mother->data[i.first][i.second].first);
        }
        iterator(HashMap<KeyType, ValueType, Hash>* mother, const vector<pair<size_t, size_t>>::iterator& elementsIterator)
            : mother(mother), elementsIterator(elementsIterator) {}
        iterator() {}
    };
    iterator find(const KeyType& key) {
        if (data.empty()) return end();
        for (const auto& item : data[getRow(key)]) {
            if (item.first.first == key) {
                return iterator(this, elements.begin() + item.second);
            }
        }
        return end();
    }
    iterator begin() {
        return iterator(this, elements.begin());
    }
    iterator end() {
        return iterator(this, elements.end());
    }
    class const_iterator {
        typedef const pair<const KeyType, ValueType> value_type;
        typedef const pair<const KeyType, ValueType>& reference;
        typedef const pair<const KeyType, ValueType>* pointer;
        typedef forward_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;
    public:
        const HashMap<KeyType, ValueType, Hash>* mother;
        vector<pair<size_t, size_t>>::const_iterator elementsIterator;
        bool operator ==(const const_iterator& other) const {
            return elementsIterator == other.elementsIterator;
        }
        bool operator !=(const const_iterator& other) const {
            return !(operator==(other));
        }
        const_iterator operator ++(int) {
            auto buf = *this;
            ++elementsIterator;
            return buf;
        }
        const_iterator& operator ++() {
            ++elementsIterator;
            return *this;
        }
        pointer operator ->() const {
            auto i = *elementsIterator;
            return reinterpret_cast<pointer>(&mother->data[i.first][i.second].first);
        }
        reference operator *() const {
            auto i = *elementsIterator;
            return *reinterpret_cast<pointer>(&mother->data[i.first][i.second].first);
        }
        const_iterator(const HashMap<KeyType, ValueType, Hash>* mother, const vector<pair<size_t, size_t>>::const_iterator& elementsIterator)
            : mother(mother), elementsIterator(elementsIterator) {}
        const_iterator() {}
    };
    const_iterator find(const KeyType& key) const {
        if (data.empty()) return end();
        for (const auto& item : data[getRow(key)]) {
            if (item.first.first == key) {
                return const_iterator(this, elements.begin() + item.second);
            }
        }
        return end();
    }
    const_iterator begin() const {
        return const_iterator(this, elements.begin());
    }
    const_iterator end() const {
        return const_iterator(this, elements.end());
    }
    size_t getRow(const KeyType& key) const {
        return Hasher(key) % data.size();
    }
    void build(size_t newSize) {
        vector<pair<KeyType, ValueType>> items;
        for (const auto& i : elements) items.push_back(data[i.first][i.second].first);
        clear();
        data.resize(newSize);
        elements.clear();
        for (const auto& item : items) {
            insert(item.first, item.second);
        }
    }
    void insert(const KeyType& key, const ValueType& value) {
        if ((double)(elements.size() + 1) > (double)data.size())
            build(data.size() * 2 + 1);
        int row = getRow(key);
        data[row].push_back({make_pair(key, value), elements.size()});
        elements.push_back({row, data[row].size() - 1});
    }
    void insert(const pair<KeyType, ValueType>& item) {
        if (find(item.first) != end()) return;
        insert(item.first, item.second);
    }
    const ValueType& at(const KeyType& key) const {
        auto it = find(key);
        if (it == end())
            throw out_of_range("out_of_range");
        return it->second;
    }
    ValueType& operator [](const KeyType& key) {
        auto it = find(key);
        if (it == end()) {
            insert(key, ValueType());
            it = find(key);
        }
        return it->second;
    }
    void erase(const KeyType& key) {
        auto it = find(key);
        if (it == end()) return;
        auto i = *it.elementsIterator;
        data[i.first][i.second].first = data[i.first].back().first;
        elements[data[i.first].back().second] = elements.back();
        data[elements[data[i.first].back().second].first][elements[data[i.first].back().second].second].second = data[i.first].back().second;
        data[i.first].pop_back();
        elements.pop_back();
    }
    size_t size() const {
        return  elements.size();
    }
    bool empty() const {
        return elements.empty();
    }
    void clear() {
        for (const auto& i : elements) {
            data[i.first].clear();
        }
        elements.clear();
    }
    Hash hash_function() const {
        return Hasher;
    }
    HashMap(const Hash& newHasher = Hash()) : Hasher(newHasher) {}
    HashMap(const initializer_list<pair<KeyType, ValueType>>& data, const Hash& newHasher = Hash()) : Hasher(newHasher) {
        build((data.size() << 1) + 3);
        for (const auto& item : data) {
            insert(item.first, item.second);
        }
    }
    HashMap(const_iterator itBegin, const_iterator itEnd, const Hash& newHasher = Hash()) : Hasher(newHasher) {
        vector<pair<KeyType, ValueType>> items(itBegin, itEnd);
        build((items.size() << 1) + 3);
        for (const auto& item : items) {
            insert(item.first, item.second);
        }
    }
    HashMap(iterator itBegin, iterator itEnd, const Hash& newHasher = Hash()) : Hasher(newHasher) {
        vector<pair<KeyType, ValueType>> items;
        while (itBegin != itEnd) items.push_back(*itBegin++);
        build((items.size() << 1) + 3);
        for (const auto& item : items) {
            insert(item.first, item.second);
        }
    }
};
