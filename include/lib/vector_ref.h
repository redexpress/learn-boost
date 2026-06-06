#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

namespace dev
{
template <class _T>
class vector_ref
{
public:
    using value_type = _T;
    using element_type = _T;
    using mutable_value_type =
        typename std::conditional<std::is_const<_T>::value, typename std::remove_const<_T>::type,
            _T>::type;

    static_assert(std::is_trivial<value_type>::value,
        "vector_ref can only be used with trivial types due to its low-level treatment of data.");

    vector_ref() : m_data(nullptr), m_count(0) {}

    vector_ref(_T* _data, size_t _count) : m_data(_data), m_count(_count) {}

    vector_ref(typename std::conditional<std::is_const<_T>::value, std::string const*,
        std::string*>::type _data)
      : m_data(reinterpret_cast<_T*>(_data->data())), m_count(_data->size() / sizeof(_T))
    {}

    vector_ref(typename std::conditional<std::is_const<_T>::value,
        std::vector<typename std::remove_const<_T>::type> const*, std::vector<_T>*>::type _data)
      : m_data(_data->data()), m_count(_data->size())
    {}

    vector_ref(typename std::conditional<std::is_const<_T>::value, std::string const&,
        std::string&>::type _data)
      : m_data(reinterpret_cast<_T*>(_data.data())), m_count(_data.size() / sizeof(_T))
    {}

    explicit operator bool() const { return m_data && m_count; }

    bool contentsEqual(std::vector<mutable_value_type> const& _c) const
    {
        if (!m_data || m_count == 0)
            return _c.empty();
        else
            return _c.size() == m_count && !memcmp(_c.data(), m_data, m_count * sizeof(_T));
    }

    std::vector<mutable_value_type> toVector() const
    {
        return std::vector<mutable_value_type>(m_data, m_data + m_count);
    }

    std::vector<unsigned char> toBytes() const
    {
        return std::vector<unsigned char>(reinterpret_cast<unsigned char const*>(m_data),
            reinterpret_cast<unsigned char const*>(m_data) + m_count * sizeof(_T));
    }

    std::string toString() const
    {
        return std::string((char const*)m_data, ((char const*)m_data) + m_count * sizeof(_T));
    }

    template <class _T2>
    explicit operator vector_ref<_T2>() const
    {
        assert(m_count * sizeof(_T) / sizeof(_T2) * sizeof(_T2) / sizeof(_T) == m_count);
        return vector_ref<_T2>(reinterpret_cast<_T2*>(m_data), m_count * sizeof(_T) / sizeof(_T2));
    }

    operator vector_ref<_T const>() const { return vector_ref<_T const>(m_data, m_count); }

    _T* data() const { return m_data; }
    size_t count() const { return m_count; }
    size_t size() const { return m_count; }
    bool empty() const { return !m_count; }

    vector_ref<_T> next() const
    {
        if (!m_data)
            return *this;
        else
            return vector_ref<_T>(m_data + m_count, m_count);
    }

    vector_ref<_T> cropped(size_t _begin) const
    {
        if (m_data && _begin < m_count)
            return vector_ref<_T>(m_data + _begin, m_count - _begin);
        else
            return vector_ref<_T>();
    }

    vector_ref<_T> cropped(size_t _begin, size_t _count) const
    {
        if (m_data && _begin < m_count && _begin + _count <= m_count)
            return vector_ref<_T>(m_data + _begin, _count);
        else
            return vector_ref<_T>();
    }

    bool overlapsWith(vector_ref<_T> const& _t) const
    {
        if (!m_data || !_t.m_data)
            return false;
        _T const* d1 = m_data;
        _T const* d2 = _t.m_data;
        return d1 + m_count > d2 && d2 + _t.m_count > d1;
    }

    void copyTo(_T* _dest) const
    {
        if (m_data)
            memcpy(_dest, m_data, m_count * sizeof(_T));
    }

    void copyTo(std::vector<mutable_value_type>& _dest) const
    {
        if (m_data)
            _dest.assign(m_data, m_data + m_count);
    }

    bool operator==(vector_ref<_T> const& _cmp) const
    {
        if (!m_data || !_cmp.m_data)
            return m_data == _cmp.m_data;
        else
            return m_count == _cmp.m_count && !memcmp(m_data, _cmp.m_data, m_count * sizeof(_T));
    }

    bool operator!=(vector_ref<_T> const& _cmp) const { return !operator==(_cmp); }

    _T& operator[](size_t _i) const
    {
        assert(m_data && _i < m_count);
        return m_data[_i];
    }

    _T* begin() const { return m_data; }
    _T* end() const { return m_data + m_count; }

private:
    _T* m_data;
    size_t m_count;
};
}  // namespace dev