/**
 * @file resolver.h
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

#ifndef STATIC_APISET_RESOLVER_H__
#define STATIC_APISET_RESOLVER_H__

#include <optional>
#include <string>
#include <unordered_set>

namespace apiset {
enum class Version {
  V6,
};

template <Version V = Version::V6>
class Resolver {
 public:
  inline Resolver() noexcept {}

  Resolver(const Resolver&) noexcept = default;
  Resolver& operator=(const Resolver&) noexcept = default;

  Resolver(Resolver&&) noexcept = default;
  Resolver& operator=(Resolver&&) noexcept = default;

  inline Version version() const noexcept {
    return V;
  }

  std::optional<std::unordered_set<std::string_view>> resolve(std::string_view dllname) const noexcept;
  std::optional<std::unordered_set<std::wstring_view>> resolve(std::wstring_view dllname) const noexcept;
};

using ResolverV6 = Resolver<Version::V6>;
};  // namespace apiset

#ifdef SAPISRESOLVER_HEADER_ONLY
#include "apiset/impl/resolver.cpp"
#endif  // SAPISRESOLVER_HEADER_ONLY

#endif  // STATIC_APISET_RESOLVER_H__
