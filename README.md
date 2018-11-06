## static-string-cpp

Compile-time string manipulation library for modern C++

### Integration

This library is header only, you need to include static_string.h

```cpp
#include <static_string.h>

using namespace snw1;
```

### Requirements

C++14 or later is required

### How to ...

Create static string from string literal

```cpp
constexpr auto str = "Hello"_ss; // str == "Hello"
constexpr size_t size = sizeof(str); // size == 6, str ends with '\0'
```

Output static string to standard output stream

```cpp
constexpr auto str = "Hello"_ss;
std::cout << str << std::endl;
```

Convert static string to std::string

```cpp
constexpr auto str = "Hello"_ss;
std::string str1 = to_string(str);
```

Get static string length and size

```cpp
constexpr auto str = "Hello"_ss;
constexpr size_t length = str.length(); // length == 5
constexpr size_t size = str.size(); // size == 6
```

Access static string symbols

```cpp
constexpr auto str = "Hello"_ss;
constexpr char ch1 = str[1]; // ch1 == 'e'
constexpr char ch4 = str[4]; // ch4 == 'o'
constexpr char ch5 = str[5]; // ch5 == '\0'
```

Iterate through static string

```cpp
constexpr auto str = "Hello"_ss;
for (size_t i = str.begin(); i != str.end(); ++i) // forward
    std::cout << str[i];
std::cout << std::endl; // Hello
for (size_t i = str.rbegin(); i != str.rend(); --i) // backward
    std::cout << str[i];
std::cout << std::endl; // olleH
```

Calculate static string or string literal hash

```cpp
constexpr auto str = "Hello"_ss;
constexpr unsigned long long hash1 = str.hash();
constexpr unsigned long long hash2 = static_string_hash("World");
// hash(s) = (s[0] + 1) + (s[1] + 1) * 33 + ... + (s[n - 1] + 1) * 33 ^ (n - 1) + 5381 * 33 ^ n mod 2 ^ 64
```

Compare two static strings or string literals

```cpp
constexpr auto str1 = "Hello"_ss;
constexpr auto str2 = "World"_ss;
static_assert(str1 < str2, "true");
static_assert(str1 >= str2, "false");
static_assert(str1 == "Hello", "true");
static_assert("World" != str2, "false");
static_assert(static_string_compare("Hello", "World") < 0, "true");
```

Convert number to static string

```cpp
constexpr int num = 12345;
constexpr auto str = ITOSS(num); // str == "12345";
```

Convert static string or string literal to number

```cpp
constexpr auto str1 = "12345"_ss;
constexpr char str2[] = "67890";
constexpr int num1 = SSTOI(str1); // num1 == 12345
constexpr int num2 = SSTOI(str2); // num2 == 67890
```

Concatenate static strings and string literals

```cpp
constexpr auto hello = "Hello"_ss;
constexpr auto world = "World"_ss;
constexpr auto greeting = hello + ", " + world + "!"; // greeting == "Hello, World!"
```

Concatenate static strings, string literals and numbers

```cpp
constexpr int apples = 5;
constexpr int oranges = 7;
constexpr auto message = static_string_concat("I have ", ITOSS(apples), 
    " apples and ", ITOSS(oranges), ", so I have ", ITOSS(apples + oranges), " fruits");
// message = "I have 5 apples and 7 oranges, so I have 12 fruits"    
```

```cpp
constexpr unsigned long long width = 123456789ULL;
constexpr unsigned long long height = 987654321ULL;
constexpr auto message = static_string_concat("A rectangle with width ", UTOSS(width), 
    " and height ", UTOSS(height), " has area ", UTOSS(width * height));
// message = "A rectangle with width 123456789 and height 987654321 has area 121932631112635269"    
```

```cpp
constexpr long long revenue = 1'000'000LL;
constexpr long long costs = 1'200'000LL;
constexpr long long profit = revenue - costs;
constexpr auto message = static_string_concat("The first quarter has ended with net ",
    (profit >= 0 ? "profit" : "loss  "), " of $", ITOSS(profit < 0 ? -profit : profit));
// message == "The first quarter has ended with net loss   of $200000"
```

Find character in static string or string literal

```cpp
constexpr auto str = "abracadabra"_ss;
constexpr auto p1 = str.find('d'); // p1 == 6
constexpr auto p2 = str.find('e'); // p2 == static_string_npos
constexpr auto p3 = str.find('a'); // p3 == 0
constexpr auto p4 = str.find('a', 4); // p4 == 5, find from position
constexpr auto p5 = str.find('a', str.begin(), 3); // p5 == 7, find 4th occurrence
constexpr auto p6 = str.rfind('a'); // p6 == 10
constexpr auto p7 = str.rfind('a', 8); // p7 == 7, find from position
constexpr auto p8 = str.rfind('a', str.rbegin(), 3); // p8 == 3, find 4th occurrence
constexpr auto p9 = static_string_find("abracadabra", 'b'); // p9 == 1
constexpr auto p10 = static_string_rfind("abracadabra", 'b'); // p10 == 8
```

Find substring in static string or string literal

```cpp
constexpr auto str = "abracadabra"_ss;
constexpr auto p1 = str.find("ada"); // p1 == 5
constexpr auto p2 = str.find("dbr"); // p2 == static_string_npos
constexpr auto p3 = str.find("ab"); // p3 == 0
constexpr auto p4 = str.find("ab", 4); // p4 == 7, find from position
constexpr auto p5 = str.find("ab", str.begin(), 1); // p5 == 7, find 2nd occurrence
constexpr auto p6 = str.rfind("ab"); // p6 == 7
constexpr auto p7 = str.rfind("ab", 5); // p7 == 0, find from position
constexpr auto p8 = str.rfind("ab", str.rbegin(), 1); // p8 == 0, find 2nd occurrence
constexpr auto p9 = static_string_find("abracadabra", "ab"); // p9 == 0
constexpr auto p10 = static_string_rfind("abracadabra", "ab"); // p10 == 7
```

Check if static string or string literal starts/ends with or contains specified substring

```cpp
constexpr auto str = "abracadabra"_ss;
static_assert(str.starts_with("abra"), "true");
static_assert(str.starts_with("brac"), "false");
static_assert(str.ends_with("abra"), "true");
static_assert(str.ends_with("dabr"), "false");
static_assert(str.contains("brac"), "true");
static_assert(str.contains("dabr"), "true");
static_assert(str.contains("brbc"), "false");
static_assert(str.contains("drbr"), "false");
static_assert(str.contains('b'), "true");
static_assert(str.contains('e'), "false");
static_assert(static_string_starts_with("abracadabra", "abra"), "true");
static_assert(static_string_ends_with("abracadabra", "abra"), "true");
static_assert(static_string_contains("abracadabra", "brac"), "true");
```

Get number of char occurrences in static string or string literal 

```cpp
constexpr auto str = "abracadabra"_ss;
constexpr size_t cnt1 = str.count('a'); // cnt1 == 5
constexpr size_t cnt2 = static_string_count("abracadabra", 'a'); // cnt2 == 5
```

Reverse static substring or string literal

```cpp
constexpr auto hello = "Hello"_ss;
constexpr auto str1 = hello.reverse(); // str1 == "olleH"
constexpr auto str2 = static_string_reverse("World"); // str2 == "dlroW"
```

Get substring, prefix or suffix of static string or string literal

```cpp
constexpr auto hello = "Hello"_ss;
constexpr auto str1 = hello.substring<1, 4>(); // str1 == "ell";
constexpr auto str2 = hello.prefix<4>(); // str2 == "Hell";
constexpr auto str3 = hello.suffix<1>(); // str3 == "ello";
constexpr auto str4 = static_string_substring<1, 4>("World"); // str4 == "orl";
constexpr auto str5 = static_string_prefix<4>("World"); // str5 == "Worl";
constexpr auto str6 = static_string_suffix<1>("World"); // str6 == "orld";
```

Split static string or string literal

```cpp
constexpr auto a = "abracadabra"_ss;
constexpr auto p1 = a.split<5>(); // p1 == {"abrac", "dabra"}
constexpr auto p2 = a.split<a.find('d')>(); // p2 == {"abraca", "abra"}
constexpr auto p3 = a.split<a.find('a', a.begin(), 1)>(); // p3 == {"abr", "cadabra"}
constexpr auto p4 = a.split<a.rfind('a', a.rbegin(), 1)>(); // p4 == {"abracad", "bra"}
constexpr auto p5 = static_string_split<5>("abracadabra"); // p5 == {"abrac", "dabra"}
```

Split static string into substrings and numbers

```cpp
constexpr auto url = "http://www.server.com:8080"_ss;
constexpr auto p = url.find("://");
constexpr auto protocol = url.prefix<p>(); // protocol == "http"
constexpr auto sockaddr = url.suffix<p + 3>();
constexpr auto hp = sockaddr.split<sockaddr.find(':')>();
constexpr auto host = hp.first; // host == "www.server.com"
constexpr int port = SSTOI(hp.second); // port == 8080
```

### License

The library is licensed under the [MIT License](http://opensource.org/licenses/MIT)

Copyright (c) 2017-2018 Andrew Sheetov <[andrew.sheetov@gmail.com](mailto:andrew.sheetov@gmail.com)>

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
