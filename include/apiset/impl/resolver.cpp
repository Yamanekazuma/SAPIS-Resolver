/**
 * @file resolver.cpp
 * @author Yamane Kazuma
 * @date 2025-09-29
 *
 * @copyright Copyright (c) 2025
 *
 * SPDX-FileCopyrightText: Copyright 2025 Yamane Kazuma
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is licensed under the GNU General Public License v3.0 or later.
 * See <https://www.gnu.org/licenses/> for details.
 */

#ifndef STATIC_APISET_RESOLVER_CPP__
#define STATIC_APISET_RESOLVER_CPP__

#ifdef SAPISRESOLVER_HEADER_ONLY
#define STATIC_APISET_RESOLVER_INLINE inline
#else  // SAPISRESOLVER_HEADER_ONLY
#define STATIC_APISET_RESOLVER_INLINE
#endif  // SAPISRESOLVER_HEADER_ONLY

#include "apiset/resolver.h"

#include <concepts>
#include <type_traits>
#include <utility>

#include <frozen/string.h>
#include <frozen/unordered_map.h>

namespace apiset {
namespace internal {

template <typename CharT>
inline std::basic_string_view<CharT> cvt_fstr_to_sv(const frozen::basic_string<CharT>& str) {
  return std::basic_string_view{str.data(), str.size()};
}

struct ApisetRealDlls_A {
  frozen::string dllname1;
  frozen::string dllname2{""};
};

struct ApisetRealDlls_W {
  frozen::wstring dllname1;
  frozen::wstring dllname2{L""};
};

constexpr std::pair<frozen::string, ApisetRealDlls_A> APISET_ENTRIES_V6_PAIRS_A[]{
#define SAPIS_T(str) str
#include "apiset/impl/apisetv6"
#undef SAPIS_T
};

constexpr std::pair<frozen::wstring, ApisetRealDlls_W> APISET_ENTRIES_V6_PAIRS_W[]{
#define SAPIS_T(str) L##str
#include "apiset/impl/apisetv6"
#undef SAPIS_T
};

constexpr auto APISET_ENTRIES_V6_A = frozen::make_unordered_map(APISET_ENTRIES_V6_PAIRS_A);
constexpr auto APISET_ENTRIES_V6_W = frozen::make_unordered_map(APISET_ENTRIES_V6_PAIRS_W);

template <class T, typename CharT>
concept HasDllNames = requires(T t) {
  { t.dllname1 } -> std::convertible_to<frozen::basic_string<CharT>>;
  { t.dllname2 } -> std::convertible_to<frozen::basic_string<CharT>>;
};

template <class Map, typename CharT>
  requires HasDllNames<typename Map::mapped_type, CharT>
inline std::optional<std::unordered_set<std::basic_string_view<CharT>>> find_and_make_set(const Map& map,
                                                                                          std::basic_string_view<CharT> key) noexcept {
  if (auto it = map.find(key); it != map.end()) {
    const auto& dlls = it->second;
    std::unordered_set<std::basic_string_view<CharT>> result{};
    result.insert(cvt_fstr_to_sv(dlls.dllname1));
    if (dlls.dllname2.length() > 0) {
      result.insert(cvt_fstr_to_sv(dlls.dllname2));
    }
    return result;
  }
  return std::nullopt;
}

};  // namespace internal
};  // namespace apiset

template <>
STATIC_APISET_RESOLVER_INLINE std::optional<std::unordered_set<std::string_view>> apiset::Resolver<apiset::Version::V6>::resolve(
    std::string_view dllname) const noexcept {
  if (dllname.ends_with(".dll")) {
    dllname = dllname.substr(0, dllname.size() - 4);
  }
  return find_and_make_set(apiset::internal::APISET_ENTRIES_V6_A, dllname);
}

template <>
STATIC_APISET_RESOLVER_INLINE std::optional<std::unordered_set<std::wstring_view>> apiset::Resolver<apiset::Version::V6>::resolve(
    std::wstring_view dllname) const noexcept {
  if (dllname.ends_with(L".dll")) {
    dllname = dllname.substr(0, dllname.size() - 4);
  }
  return find_and_make_set(apiset::internal::APISET_ENTRIES_V6_W, dllname);
}

#endif  // STATIC_APISET_RESOLVER_CPP__
