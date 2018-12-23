/*
Compile-time string manipulation library for modern C++
version 0.0.31
https://github.com/snw1/static-string-cpp

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2017-2018 Andrew Sheetov <andrew.sheetov@gmail.com>

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SNW1_STATIC_STRING_H
#define SNW1_STATIC_STRING_H

#include <array>
#include <string>
#include <limits>

namespace snw1 {

#define ITOSS(x) int_to_static_string<(x), char>()
#define ITOSW(x) int_to_static_string<(x), wchar_t>()
#define UTOSS(x) uint_to_static_string<(x), char>()
#define UTOSW(x) uint_to_static_string<(x), wchar_t>()
#define SSTOI(x) static_string_to_int((x))
#define SSTOU(x) static_string_to_uint((x))

// forward declarations
template<typename Char, size_t Size> struct basic_static_string;
template<typename Char> constexpr basic_static_string<Char, 1> static_string_concat();
template<typename Char, typename Arg, typename ... Args> constexpr auto static_string_concat(Arg&& arg, Args&& ... args);
template<typename Char, size_t Size> std::basic_string<Char> to_string(const basic_static_string<Char, Size>& str);
template<typename Char, size_t Size> constexpr auto static_string_reverse(const basic_static_string<Char, Size>& str);
template<size_t Begin, size_t End, typename Char, size_t Size> constexpr auto static_string_substring(const basic_static_string<Char, Size>& str);
template<typename Char, size_t Size> constexpr size_t static_string_find(const basic_static_string<Char, Size>& str, Char ch, size_t from = 0, size_t nth = 0);
template<typename Char, size_t Size, size_t SubSize> constexpr size_t static_string_find(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& substr, size_t from = 0, size_t nth = 0);
template<typename Char, size_t Size, size_t SubSize> constexpr size_t static_string_find(const basic_static_string<Char, Size>& str, const Char (& substr)[SubSize], size_t from = 0, size_t nth = 0);
template<typename Char, size_t Size> constexpr size_t static_string_rfind(const basic_static_string<Char, Size>& str, Char ch, size_t from = Size - 2, size_t nth = 0);
template<typename Char, size_t Size, size_t SubSize> constexpr size_t static_string_rfind(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& substr, size_t from = Size - SubSize, size_t nth = 0);
template<typename Char, size_t Size, size_t SubSize> constexpr size_t static_string_rfind(const basic_static_string<Char, Size>& str, const Char (& substr)[SubSize], size_t from = Size - SubSize, size_t nth = 0);
template<typename Char, size_t Size> constexpr bool static_string_contains(const basic_static_string<Char, Size>& str, Char ch);
template<typename Char, size_t Size, size_t SubSize> constexpr bool static_string_contains(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& substr);
template<typename Char, size_t Size, size_t SubSize> constexpr bool static_string_contains(const basic_static_string<Char, Size>& str, const Char (& substr)[SubSize]);
template<size_t End, typename Char, size_t Size> constexpr auto static_string_prefix(const basic_static_string<Char, Size>& str);
template<size_t Begin, typename Char, size_t Size> constexpr auto static_string_suffix(const basic_static_string<Char, Size>& str);
template<typename Char, size_t Size> constexpr unsigned long long static_string_hash(const basic_static_string<Char, Size>& str);
template<size_t Index, typename Char, size_t Size> constexpr auto static_string_split(const basic_static_string<Char, Size>& str);
template<typename Char, size_t Size> constexpr size_t static_string_count(const basic_static_string<Char, Size>& str, Char ch);
template<typename Char, size_t SubSize, size_t Size> constexpr bool static_string_starts_with(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& prefix);
template<typename Char, size_t SubSize, size_t Size> constexpr bool static_string_starts_with(const basic_static_string<Char, Size>& str, const Char (& prefix)[SubSize]);
template<typename Char, size_t SubSize, size_t Size> constexpr bool static_string_ends_with(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& suffix);
template<typename Char, size_t SubSize, size_t Size> constexpr bool static_string_ends_with(const basic_static_string<Char, Size>& str, const Char (& suffix)[SubSize]);

constexpr size_t static_string_npos = std::numeric_limits<size_t>::max();

namespace __static_string_detail {

template<size_t ... Indexes>
struct index_sequence {};

template<size_t Size, size_t ... Indexes>
struct make_index_sequence : make_index_sequence<Size - 1, Size - 1, Indexes ...> {};

template<size_t ... Indexes>
struct make_index_sequence<0, Indexes ...> : index_sequence<Indexes ...> {};

template<size_t Size, size_t ... Indexes>
struct make_reverse_index_sequence : make_reverse_index_sequence<Size - 1, Indexes ..., Size - 1> {};

template<size_t ... Indexes>
struct make_reverse_index_sequence<0, Indexes ...> : index_sequence<Indexes ...> {};

template<size_t Begin, size_t End, size_t ... Indexes>
struct make_index_subsequence : make_index_subsequence<Begin, End - 1, End - 1, Indexes ...> {};

template<size_t Pos, size_t ... Indexes>
struct make_index_subsequence<Pos, Pos, Indexes ...> : index_sequence<Indexes ...> {};

template<typename Char, Char ... Chars>
struct char_sequence {};

template<unsigned long long Value, typename Char, Char ... Chars>
struct make_uint_char_sequence :
    make_uint_char_sequence<Value / 10, Char, static_cast<Char>('0') + Value % 10, Chars ...> {};

template<typename Char, Char ... Chars>
struct make_uint_char_sequence<0, Char, Chars ...> : char_sequence<Char, Chars ...> {};

template<typename Char>
struct make_uint_char_sequence<0, Char> : char_sequence<Char, static_cast<Char>('0')> {};

template<bool Negative, long long Value, typename Char, Char ... Chars>
struct make_int_char_sequence {};

template<long long Value, typename Char, Char ... Chars>
struct make_int_char_sequence<true, Value, Char, Chars ...> :
    make_int_char_sequence<true, Value / 10, Char, static_cast<Char>('0') - (Value % 10), Chars ...> {};

template<long long Value, typename Char, Char ... Chars>
struct make_int_char_sequence<false, Value, Char, Chars ...> :
    make_int_char_sequence<false, Value / 10, Char, static_cast<Char>('0') + Value % 10, Chars ...> {};

template<typename Char, Char ... Chars>
struct make_int_char_sequence<true, 0, Char, Chars ...> : char_sequence<Char, static_cast<Char>('-'), Chars ...> {};

template<typename Char, Char ... Chars>
struct make_int_char_sequence<false, 0, Char, Chars ...> : char_sequence<Char, Chars ...> {};

template<typename Char>
struct make_int_char_sequence<false, 0, Char> : char_sequence<Char, static_cast<Char>('0')> {};

template<typename Char, size_t Size>
constexpr basic_static_string<Char, Size> make(const basic_static_string<Char, Size>& str) {
    return str;
}

template<typename Char, size_t Size>
constexpr basic_static_string<Char, Size> make(const Char (& str)[Size]) {
    return make(str, make_index_sequence<Size - 1>{});
}

template<typename Char, Char ... Chars>
constexpr basic_static_string<Char, sizeof ... (Chars) + 1> make(char_sequence<Char, Chars ...>) {
    return {Chars ..., static_cast<Char>('\0')};
}

template<typename Char, size_t Size, size_t ... Indexes>
constexpr basic_static_string<Char, sizeof ... (Indexes) + 1> make(const Char (& str)[Size],
    index_sequence<Indexes ...>) {
    return {str[Indexes] ..., static_cast<Char>('\0')};
}

template<typename Char, size_t Size, size_t ... Indexes>
constexpr basic_static_string<Char, sizeof ... (Indexes) + 1> make(const basic_static_string<Char, Size>& str,
    index_sequence<Indexes ...>) {
    return {str.data[Indexes] ..., static_cast<Char>('\0')};
}

template<typename Char, size_t Size1, size_t Size2>
constexpr int compare(
    const basic_static_string<Char, Size1>& str1, size_t index1,
    const basic_static_string<Char, Size2>& str2, size_t index2,
    size_t cur_length, size_t max_length) {
    return cur_length > max_length || (index1 >= Size1 && index2 >= Size2) ? 0 :
        index1 >= Size1 ? -1 :
            index2 >= Size2 ? 1 :
                str1.data[index1] > str2.data[index2] ? 1 :
                    str1.data[index1] < str2.data[index2] ? -1 :
                        compare(str1, index1 + 1, str2, index2 + 1, cur_length + 1, max_length);
}

template<typename Char, size_t Size1, size_t ... Indexes1, size_t Size2, size_t ... Indexes2>
constexpr basic_static_string<Char, Size1 + Size2 - 1> concat(
    const basic_static_string<Char, Size1>& str1, index_sequence<Indexes1 ...>,
    const basic_static_string<Char, Size2>& str2, index_sequence<Indexes2 ...>) {
    return {str1.data[Indexes1] ..., str2.data[Indexes2] ..., static_cast<Char>('\0')};
}

template<typename Char, size_t Size1, size_t Size2>
constexpr basic_static_string<Char, Size1 + Size2 - 1> concat(
    const basic_static_string<Char, Size1>& str1,
    const basic_static_string<Char, Size2>& str2) {
    return concat(str1, make_index_sequence<Size1 - 1>{},
        str2, make_index_sequence<Size2 - 1>{});
}

constexpr size_t length(long long value, size_t first) {
    return value == 0 ? first :
        value > 0 ? 1 + length(value / 10, 0) :
            2 + length(-(value / 10), 0);
}

template<typename Char, size_t Size>
constexpr unsigned long long hash(const basic_static_string<Char, Size>& str, size_t index) {
    return index >= Size - 1 ? 5381ULL :
        hash(str, index + 1) * 33ULL + static_cast<unsigned long long>(str.data[index] + 1);
}

template<typename Char, size_t Size>
constexpr size_t count(const basic_static_string<Char, Size>& str, Char ch, size_t index) {
    return index >= Size - 1 ? 0 :
        (str.data[index] == ch ? 1 : 0) + count(str, ch, index + 1);
}

template<typename Char, size_t Size>
constexpr long long to_int(const basic_static_string<Char, Size>& str, size_t index, size_t first) {
    return index < first || index >= Size - 1 ? 0 :
        first == 0 ? (str.data[index] - '0') + 10LL * to_int(str, index - 1, first) :
            -(str.data[index] - '0') + 10LL * to_int(str, index - 1, first);
}

template<typename Char, size_t Size>
constexpr long long to_int(const basic_static_string<Char, Size>& str) {
    return Size < 2 ? 0 :
        str.data[0] == '-' ? to_int(str, Size - 2, 1) :
            to_int(str, Size - 2, 0);
}

template<typename Char, size_t Size>
constexpr unsigned long long to_uint(const basic_static_string<Char, Size>& str, size_t index) {
    return Size < 2 || index >= Size - 1 ? 0 :
        (str.data[index] - '0') + 10ULL * to_uint(str, index - 1);
}

} // namespace __static_string_detail

template<typename Char, size_t Size> struct basic_static_string {
    constexpr size_t length() const {
        return Size - 1;
    }
    constexpr size_t size() const {
        return Size - 1;
    }
    constexpr size_t begin() const {
        return 0;
    }
    constexpr size_t end() const {
        return Size - 1;
    }
    constexpr size_t rbegin() const {
        return Size - 2;
    }
    constexpr size_t rend() const {
        return std::numeric_limits<size_t>::max();
    }
    constexpr bool empty() const {
        return Size < 2;
    }
    template<size_t StrSize> static constexpr auto make(const Char (& str)[StrSize]) {
        return __static_string_detail::make<Char>(str);
    }
    static constexpr auto concat() {
        return static_string_concat<Char>();
    }
    template<typename Arg, typename ... Args> static constexpr auto concat(Arg&& arg, Args&& ... args) {
        return static_string_concat<Char>(std::forward<Arg>(arg), std::forward<Args>(args) ...);
    }
    constexpr auto reverse() const {
        return static_string_reverse(*this);
    }
    template<size_t Begin, size_t End> constexpr auto substring() const {
        return static_string_substring<Begin, End>(*this);
    }
    template<size_t End> constexpr auto prefix() const {
        return static_string_prefix<End>(*this);
    }
    template<size_t Begin> constexpr auto suffix() const {
        return static_string_suffix<Begin>(*this);
    }
    constexpr size_t find(Char ch, size_t from = 0, size_t nth = 0) const {
        return static_string_find(*this, ch, from, nth);
    }
    template<size_t SubSize> constexpr size_t find(const basic_static_string<Char, SubSize>& substr, size_t from = 0, size_t nth = 0) const {
        return static_string_find(*this, substr, from, nth);
    }
    template<size_t SubSize> constexpr size_t find(const Char (& substr)[SubSize], size_t from = 0, size_t nth = 0) const {
        return static_string_find(*this, substr, from, nth);
    }
    constexpr size_t rfind(Char ch, size_t from = Size - 2, size_t nth = 0) const {
        return static_string_rfind(*this, ch, from, nth);
    }
    template<size_t SubSize> constexpr size_t rfind(const basic_static_string<Char, SubSize>& substr, size_t from = Size - SubSize, size_t nth = 0) const {
        return static_string_rfind(*this, substr, from, nth);
    }
    template<size_t SubSize> constexpr size_t rfind(const Char (& substr)[SubSize], size_t from = Size - SubSize, size_t nth = 0) const {
        return static_string_rfind(*this, substr, from, nth);
    }
    constexpr bool contains(Char ch) const {
        return static_string_contains(*this, ch);
    }
    template<size_t SubSize> constexpr bool contains(const basic_static_string<Char, SubSize>& substr) const {
        return static_string_contains(*this, substr);
    }
    template<size_t SubSize> constexpr bool contains(const Char (& substr)[SubSize]) const {
        return static_string_contains(*this, substr);
    }
    template<size_t SubSize> constexpr bool starts_with(const basic_static_string<Char, SubSize>& prefix) const {
        return static_string_starts_with(*this, prefix);
    }
    template<size_t SubSize> constexpr bool starts_with(const Char (& prefix)[SubSize]) const {
        return static_string_starts_with(*this, prefix);
    }
    template<size_t SubSize> constexpr bool ends_with(const basic_static_string<Char, SubSize>& suffix) const {
        return static_string_ends_with(*this, suffix);
    }
    template<size_t SubSize> constexpr bool ends_with(const Char (& suffix)[SubSize]) const {
        return static_string_ends_with(*this, suffix);
    }
    constexpr size_t count(Char ch) const {
        return static_string_count(*this, ch);
    }
    template<size_t Index> constexpr auto split() const {
        return static_string_split<Index>(*this);
    }
    constexpr unsigned long long hash() const {
        return static_string_hash(*this);
    }
    constexpr Char operator[](size_t index) const {
        return data[index];
    }
    std::string str() const {
        return to_string(*this);
    }
    std::array<const Char, Size> data;
};

template<size_t Size> using static_string_t = basic_static_string<char, Size>;
template<size_t Size> using static_wstring_t = basic_static_string<wchar_t, Size>;

using static_string = basic_static_string<char, 0>;
using static_wstring = basic_static_string<wchar_t, 0>;

template<typename Char, Char ... Chars>
constexpr basic_static_string<Char, sizeof ... (Chars) + 1> operator"" _ss() {
    return {Chars ..., static_cast<Char>('\0')};
};

template<typename Char, size_t Size>
std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& bos, const basic_static_string<Char, Size>& str) {
    bos << str.data.data();
    return bos;
}

template<typename Char, size_t Size>
std::basic_string<Char> to_string(const basic_static_string<Char, Size>& str) {
    return std::basic_string<Char>(str.data.data());
}

constexpr size_t static_length() {
    return 0;
}

template<typename Char, size_t Size>
constexpr size_t static_length(const Char (& str)[Size]) {
    return Size - 1;
}

template<typename Char, size_t Size>
constexpr size_t static_length(const basic_static_string<Char, Size>& str) {
    return Size - 1;
}

constexpr size_t static_length(long long value) {
    return __static_string_detail::length(value, 1);
}

constexpr size_t static_concat_length() {
    return 0;
}

template<typename Arg, typename ... Args>
constexpr size_t static_concat_length(Arg&& arg, Args&& ... args) {
    return static_length(std::forward<Arg>(arg)) + static_concat_length(std::forward<Args>(args) ...);
}

template<typename Char>
constexpr basic_static_string<Char, 1> static_string_concat() {
    return {static_cast<Char>('\0')};
}

template<typename Char, typename Arg, typename ... Args>
constexpr auto static_string_concat(Arg&& arg, Args&& ... args) {
    return __static_string_detail::concat(__static_string_detail::make(std::forward<Arg>(arg)),
        static_string_concat<Char>(std::forward<Args>(args) ...));
}

template<typename Char, size_t Size1, size_t Size2>
constexpr int static_string_compare(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return __static_string_detail::compare(str1, 0, str2, 0, 1, std::numeric_limits<size_t>::max());
}

template<typename Char, size_t Size1, size_t Size2>
constexpr int static_string_compare(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, __static_string_detail::make(str2));
}

template<typename Char, size_t Size1, size_t Size2>
constexpr int static_string_compare(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(__static_string_detail::make(str1), str2);
}

template<typename Char, size_t Size1, size_t Size2>
constexpr int static_string_compare(const Char (& str1)[Size1], const Char (& str2)[Size2]) {
    return static_string_compare(__static_string_detail::make(str1), __static_string_detail::make(str2));
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator==(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) == 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator==(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, str2) == 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator==(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) == 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator!=(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) != 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator!=(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, str2) != 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator!=(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) != 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator<(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) < 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator<(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, str2) < 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator<(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) < 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator<=(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) <= 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator<=(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, str2) <= 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator<=(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) <= 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator>(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) > 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator>(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, str2) > 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator>(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) > 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator>=(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) >= 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator>=(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return static_string_compare(str1, str2) >= 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr bool operator>=(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return static_string_compare(str1, str2) >= 0;
}

template<typename Char, size_t Size1, size_t Size2>
constexpr auto operator+(const basic_static_string<Char, Size1>& str1, const basic_static_string<Char, Size2>& str2) {
    return __static_string_detail::concat(str1, str2);
}

template<typename Char, size_t Size1, size_t Size2>
constexpr auto operator+(const basic_static_string<Char, Size1>& str1, const Char (& str2)[Size2]) {
    return __static_string_detail::concat(str1, __static_string_detail::make(str2));
}

template<typename Char, size_t Size1, size_t Size2>
constexpr auto operator+(const Char (& str1)[Size1], const basic_static_string<Char, Size2>& str2) {
    return __static_string_detail::concat(__static_string_detail::make(str1), str2);
}

template<typename Char, size_t Size>
constexpr auto static_string_reverse(const basic_static_string<Char, Size>& str) {
    return __static_string_detail::make(str, __static_string_detail::make_reverse_index_sequence<Size - 1>{});
}

template<typename Char, size_t Size>
constexpr auto static_string_reverse(const Char (& str)[Size]) {
    return __static_string_detail::make(str, __static_string_detail::make_reverse_index_sequence<Size - 1>{});
}

template<size_t Begin, size_t End, typename Char, size_t Size>
constexpr auto static_string_substring(const basic_static_string<Char, Size>& str) {
    static_assert(Begin <= End, "Begin is greater than End (Begin > End)");
    static_assert(End <= Size - 1, "End is greater than string length (End > Size - 1)");
    return __static_string_detail::make(str, __static_string_detail::make_index_subsequence<Begin, End>{});
}

template<size_t Begin, size_t End, typename Char, size_t Size>
constexpr auto static_string_substring(const Char (& str)[Size]) {
    static_assert(Begin <= End, "Begin is greater than End (Begin > End)");
    static_assert(End <= Size - 1, "End is greater than string length (End > Size - 1)");
    return __static_string_detail::make(str, __static_string_detail::make_index_subsequence<Begin, End>{});
}

template<size_t End, typename Char, size_t Size>
constexpr auto static_string_prefix(const basic_static_string<Char, Size>& str) {
    return static_string_substring<0, End>(str);
}

template<size_t End, typename Char, size_t Size>
constexpr auto static_string_prefix(const Char (& str)[Size]) {
    return static_string_substring<0, End>(str);
}

template<size_t Begin, typename Char, size_t Size>
constexpr auto static_string_suffix(const basic_static_string<Char, Size>& str) {
    return static_string_substring<Begin, Size - 1>(str);
}

template<size_t Begin, typename Char, size_t Size>
constexpr auto static_string_suffix(const Char (& str)[Size]) {
    return static_string_substring<Begin, Size - 1>(str);
}

template<typename Char, size_t Size>
constexpr size_t static_string_find(const basic_static_string<Char, Size>& str, Char ch, size_t from, size_t nth) {
    return Size < 2 || from >= Size - 1 ? static_string_npos :
        str.data[from] != ch ? static_string_find(str, ch, from + 1, nth) :
            nth > 0 ? static_string_find(str, ch, from + 1, nth - 1) : from;
}

template<typename Char, size_t Size>
constexpr size_t static_string_find(const Char (& str)[Size], Char ch, size_t from = 0, size_t nth = 0) {
    return static_string_find(__static_string_detail::make(str), ch, from, nth);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_find(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& substr, size_t from, size_t nth) {
    return Size < SubSize || from > Size - SubSize ? static_string_npos :
        __static_string_detail::compare(str, from, substr, 0, 1, SubSize - 1) != 0 ? static_string_find(str, substr, from + 1, nth) :
            nth > 0 ? static_string_find(str, substr, from + 1, nth - 1) : from;
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_find(const Char (& str)[Size], const basic_static_string<Char, SubSize>& substr, size_t from = 0, size_t nth = 0) {
    return static_string_find(__static_string_detail::make(str), substr, from, nth);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_find(const basic_static_string<Char, Size>& str, const Char (& substr)[SubSize], size_t from, size_t nth) {
    return static_string_find(str, __static_string_detail::make(substr), from, nth);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_find(const Char (& str)[Size], const Char (& substr)[SubSize], size_t from = 0, size_t nth = 0) {
    return static_string_find(__static_string_detail::make(str), __static_string_detail::make(substr), from, nth);
}

template<typename Char, size_t Size>
constexpr size_t static_string_rfind(const basic_static_string<Char, Size>& str, Char ch, size_t from, size_t nth) {
    return Size < 2 || from > Size - 2 ? static_string_npos :
        str.data[from] != ch ? static_string_rfind(str, ch, from - 1, nth) :
            nth > 0 ? static_string_rfind(str, ch, from - 1, nth - 1) : from;
}

template<typename Char, size_t Size>
constexpr size_t static_string_rfind(const Char (& str)[Size], Char ch, size_t from = Size - 2, size_t nth = 0) {
    return static_string_rfind(__static_string_detail::make(str), ch, from, nth);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_rfind(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& substr, size_t from, size_t nth) {
    return Size < SubSize || from > Size - SubSize ? static_string_npos :
        __static_string_detail::compare(str, from, substr, 0, 1, SubSize - 1) != 0 ? static_string_rfind(str, substr, from - 1, nth) :
            nth > 0 ? static_string_rfind(str, substr, from - 1, nth - 1) : from;
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_rfind(const Char (& str)[Size], const basic_static_string<Char, SubSize>& substr, size_t from = Size - SubSize, size_t nth = 0) {
    return static_string_rfind(__static_string_detail::make(str), substr, from, nth);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_rfind(const basic_static_string<Char, Size>& str, const Char (& substr)[SubSize], size_t from, size_t nth) {
    return static_string_rfind(str, __static_string_detail::make(substr), from, nth);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr size_t static_string_rfind(const Char (& str)[Size], const Char (& substr)[SubSize], size_t from = Size - SubSize, size_t nth = 0) {
    return static_string_rfind(__static_string_detail::make(str), __static_string_detail::make(substr), from, nth);
}

template<typename Char, size_t Size>
constexpr bool static_string_contains(const basic_static_string<Char, Size>& str, Char ch) {
    return static_string_find(str, ch) != static_string_npos;
}

template<typename Char, size_t Size>
constexpr bool static_string_contains(const Char (& str)[Size], Char ch) {
    return static_string_contains(__static_string_detail::make(str), ch);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr bool static_string_contains(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& substr) {
    return static_string_find(str, substr) != static_string_npos;
}

template<typename Char, size_t Size, size_t SubSize>
constexpr bool static_string_contains(const basic_static_string<Char, Size>& str, const Char (& substr)[SubSize]) {
    return static_string_contains(str, __static_string_detail::make(substr));
}

template<typename Char, size_t Size, size_t SubSize>
constexpr bool static_string_contains(const Char (& str)[Size], const basic_static_string<Char, SubSize>& substr) {
    return static_string_contains(__static_string_detail::make(str), substr);
}

template<typename Char, size_t Size, size_t SubSize>
constexpr bool static_string_contains(const Char (& str)[Size], const Char (& substr)[SubSize]) {
    return static_string_contains(__static_string_detail::make(str), __static_string_detail::make(substr));
}

template<typename Char, size_t Size>
constexpr size_t static_string_count(const basic_static_string<Char, Size>& str, Char ch) {
    return __static_string_detail::count(str, ch, 0);
}

template<typename Char, size_t Size>
constexpr size_t static_string_count(const Char (& str)[Size], Char ch) {
    return static_string_count(__static_string_detail::make(str), ch);
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_starts_with(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& prefix) {
    return SubSize > Size ? false :
        __static_string_detail::compare(str, 0, prefix, 0, 1, SubSize - 1) == 0;
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_starts_with(const Char (& str)[Size], const basic_static_string<Char, SubSize>& prefix) {
    return static_string_starts_with(__static_string_detail::make(str), prefix);
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_starts_with(const basic_static_string<Char, Size>& str, const Char (& prefix)[SubSize]) {
    return static_string_starts_with(str, __static_string_detail::make(prefix));
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_starts_with(const Char (& str)[Size], const Char (& prefix)[SubSize]) {
    return static_string_starts_with(__static_string_detail::make(str), __static_string_detail::make(prefix));
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_ends_with(const basic_static_string<Char, Size>& str, const basic_static_string<Char, SubSize>& suffix) {
    return SubSize > Size ? false :
        __static_string_detail::compare(str, Size - SubSize, suffix, 0, 1, SubSize - 1) == 0;
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_ends_with(const Char (& str)[Size], const basic_static_string<Char, SubSize>& suffix) {
    return static_string_ends_with(__static_string_detail::make(str), suffix);
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_ends_with(const basic_static_string<Char, Size>& str, const Char (& suffix)[SubSize]) {
    return static_string_ends_with(str, __static_string_detail::make(suffix));
}

template<typename Char, size_t SubSize, size_t Size>
constexpr bool static_string_ends_with(const Char (& str)[Size], const Char (& suffix)[SubSize]) {
    return static_string_ends_with(__static_string_detail::make(str), __static_string_detail::make(suffix));
}

template<size_t Index, typename Char, size_t Size>
constexpr auto static_string_split(const basic_static_string<Char, Size>& str) {
    return std::make_pair(static_string_prefix<Index>(str), static_string_suffix<Index + 1>(str));
}

template<size_t Index, typename Char, size_t Size>
constexpr auto static_string_split(const Char (& str)[Size]) {
    return static_string_split<Index>(__static_string_detail::make(str));
}

template<typename Char, size_t Size>
constexpr unsigned long long static_string_hash(const basic_static_string<Char, Size>& str) {
    return __static_string_detail::hash(str, 0);
}

template<typename Char, size_t Size>
constexpr unsigned long long static_string_hash(const Char (& str)[Size]) {
    return static_string_hash(__static_string_detail::make(str));
}

template<long long Value, typename Char>
constexpr auto int_to_static_string() {
    return __static_string_detail::make(__static_string_detail::make_int_char_sequence<(Value < 0), Value, Char>{});
}

template<unsigned long long Value, typename Char>
constexpr auto uint_to_static_string() {
    return __static_string_detail::make(__static_string_detail::make_uint_char_sequence<Value, Char>{});
}

template<typename Char, size_t Size>
constexpr long long static_string_to_int(const basic_static_string<Char, Size>& str) {
    return __static_string_detail::to_int(str);
}

template<typename Char, size_t Size>
constexpr long long static_string_to_int(const Char (& str)[Size]) {
    return static_string_to_int(__static_string_detail::make(str));
}

template<typename Char, size_t Size>
constexpr unsigned long long static_string_to_uint(const basic_static_string<Char, Size>& str) {
    return __static_string_detail::to_uint(str, Size - 2);
}

template<typename Char, size_t Size>
constexpr unsigned long long static_string_to_uint(const Char (& str)[Size]) {
    return static_string_to_uint(__static_string_detail::make(str));
}

} // namespace snw1

#endif // SNW1_STATIC_STRING_H
