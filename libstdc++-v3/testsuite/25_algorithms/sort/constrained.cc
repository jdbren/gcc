// Copyright (C) 2020-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-do run { target c++20 } }
// { dg-require-cstdint "" }

#include <algorithm>
#include <random>
#include <ranges>
#include <vector>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>

using __gnu_test::test_container;
using __gnu_test::test_range;
using __gnu_test::random_access_iterator_wrapper;

namespace ranges = std::ranges;

void
test01()
{
  for (unsigned size = 0; size < 50; ++size)
    {
      std::vector<int> vref(size);
      std::iota(vref.begin(), vref.end(), 0);
      std::vector<int> v1(vref), v2(vref);
      test_container<int, random_access_iterator_wrapper> c
	= {v1.data(), v1.data() + size};
      test_range<int, random_access_iterator_wrapper> r
	= {v2.data(), v2.data() + size};

      std::ranlux48_base g1(size), g2(size + 1);
      ranges::shuffle(c, g1);
      ranges::shuffle(ranges::begin(r), ranges::end(r), g2);

      VERIFY( ranges::sort(c) == c.end() );
      VERIFY( ranges::sort(r) == ranges::end(r) );

      VERIFY( ranges::equal(c, vref) );
      VERIFY( ranges::equal(r, vref) );
    }
}

struct X
{
  int i;
  constexpr X(int a) : i(a) { }
};

constexpr bool
test02()
{
  X x[] = {3,4,2,1,5};
  const X y[] = {4,3,2,1,5};

  auto res = ranges::sort(x, x+4, ranges::greater{}, &X::i);
  return (res == x+4
	  && ranges::equal(x, y, {}, &X::i, &X::i));
}

constexpr bool
test03()
{
  // PR libstdc++/100795 - ranges::sort should not use std::sort directly
#if __SIZEOF_INT128__
  auto v = std::views::iota(__int128(0), __int128(20));
#else
  auto v = std::views::iota(0ll, 20ll);
#endif

  int storage[20] = {2,5,4,3,1,6,7,9,10,8,11,14,12,13,15,16,18,0,19,17};
  auto w = v | std::views::transform([&](auto i) -> int& { return storage[i]; });
  using type = decltype(w);
  using cat = std::iterator_traits<std::ranges::iterator_t<type>>::iterator_category;
  static_assert( std::same_as<cat, std::output_iterator_tag> );
  static_assert( std::ranges::random_access_range<type> );

  ranges::sort(w);
  VERIFY( ranges::equal(w, v) );

  ranges::sort(w, std::ranges::greater{});
  VERIFY( ranges::equal(w, v | std::views::reverse) );

  ranges::sort(w, std::ranges::greater{}, std::negate{});
  VERIFY( ranges::equal(w, v) );

  return true;
}

int
main()
{
  test01();
  static_assert(test02());
  static_assert(test03());
}
