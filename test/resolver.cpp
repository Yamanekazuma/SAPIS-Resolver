#include <gtest/gtest.h>

#include <apiset/resolver.h>

#include <algorithm>

static constexpr char EMPTY_DLL_A[] = "";
static constexpr wchar_t EMPTY_DLL_W[] = L"";

static constexpr char NONEXISTENT_DLL_A[] = "NONEXISTENT.dll";
static constexpr wchar_t NONEXISTENT_DLL_W[] = L"NONEXISTENT.dll";

static constexpr char EXISTENT_DLL_A[] = "kernel32.dll";
static constexpr wchar_t EXISTENT_DLL_W[] = L"kernel32.dll";

TEST(test_resolver, initialize) {
  {
    auto resolver = apiset::Resolver{};
    EXPECT_EQ(resolver.version(), apiset::Version::V6);

    apiset::Resolver resolver_copied{resolver};
    EXPECT_EQ(resolver_copied.version(), apiset::Version::V6);
    resolver_copied = resolver;
    EXPECT_EQ(resolver_copied.version(), apiset::Version::V6);
  }

  {
    auto resolverV6 = apiset::ResolverV6{};
    EXPECT_EQ(resolverV6.version(), apiset::Version::V6);

    apiset::ResolverV6 resolverV6_copied{resolverV6};
    EXPECT_EQ(resolverV6_copied.version(), apiset::Version::V6);
    resolverV6_copied = resolverV6;
    EXPECT_EQ(resolverV6_copied.version(), apiset::Version::V6);
  }

  {
    auto resolverV6 = apiset::Resolver<apiset::Version::V6>{};
    EXPECT_EQ(resolverV6.version(), apiset::Version::V6);

    apiset::Resolver<apiset::Version::V6> resolverV6_copied{resolverV6};
    EXPECT_EQ(resolverV6_copied.version(), apiset::Version::V6);
    resolverV6_copied = resolverV6;
    EXPECT_EQ(resolverV6_copied.version(), apiset::Version::V6);
  }
}

TEST(test_resolver, resolve_v6) {
  apiset::ResolverV6 resolver{};

  {
    auto resolve_empty_c = resolver.resolve(EMPTY_DLL_A);
    auto resolve_empty_s = resolver.resolve(std::string{EMPTY_DLL_A});
    auto resolve_empty_wc = resolver.resolve(EMPTY_DLL_W);
    auto resolve_empty_ws = resolver.resolve(std::wstring{EMPTY_DLL_W});
    EXPECT_EQ(resolve_empty_c, std::nullopt);
    EXPECT_EQ(resolve_empty_s, std::nullopt);
    EXPECT_EQ(resolve_empty_wc, std::nullopt);
    EXPECT_EQ(resolve_empty_ws, std::nullopt);
  }

  {
    auto resolve_nonexistent_c = resolver.resolve(NONEXISTENT_DLL_A);
    auto resolve_nonexistent_s = resolver.resolve(std::string{NONEXISTENT_DLL_A});
    auto resolve_nonexistent_wc = resolver.resolve(NONEXISTENT_DLL_W);
    auto resolve_nonexistent_ws = resolver.resolve(std::wstring{NONEXISTENT_DLL_W});
    EXPECT_EQ(resolve_nonexistent_c, std::nullopt);
    EXPECT_EQ(resolve_nonexistent_s, std::nullopt);
    EXPECT_EQ(resolve_nonexistent_wc, std::nullopt);
    EXPECT_EQ(resolve_nonexistent_ws, std::nullopt);
  }

  {
    auto resolve_existent_c = resolver.resolve(EXISTENT_DLL_A);
    auto resolve_existent_s = resolver.resolve(std::string{EXISTENT_DLL_A});
    auto resolve_existent_wc = resolver.resolve(EXISTENT_DLL_W);
    auto resolve_existent_ws = resolver.resolve(std::wstring{EXISTENT_DLL_W});
    EXPECT_EQ(resolve_existent_c, std::nullopt);
    EXPECT_EQ(resolve_existent_s, std::nullopt);
    EXPECT_EQ(resolve_existent_wc, std::nullopt);
    EXPECT_EQ(resolve_existent_ws, std::nullopt);
  }

  {
    constexpr char ALIAS_WITH_EXT_A[] = "api-ms-win-core-util-l1-1-1.dll";
    constexpr wchar_t ALIAS_WITH_EXT_W[] = L"api-ms-win-core-util-l1-1-1.dll";
    auto resolve_alias_with_ext_c = resolver.resolve(ALIAS_WITH_EXT_A);
    auto resolve_alias_with_ext_s = resolver.resolve(std::string{ALIAS_WITH_EXT_A});
    auto resolve_alias_with_ext_wc = resolver.resolve(ALIAS_WITH_EXT_W);
    auto resolve_alias_with_ext_ws = resolver.resolve(std::wstring{ALIAS_WITH_EXT_W});
    EXPECT_TRUE(resolve_alias_with_ext_c);
    if (resolve_alias_with_ext_c) {
      EXPECT_EQ(resolve_alias_with_ext_c->size(), 2);
      EXPECT_TRUE(resolve_alias_with_ext_c->contains("kernel32.dll"));
      EXPECT_TRUE(resolve_alias_with_ext_c->contains("kernelbase.dll"));
    }
    EXPECT_TRUE(resolve_alias_with_ext_s);
    if (resolve_alias_with_ext_s) {
      EXPECT_EQ(resolve_alias_with_ext_s->size(), 2);
      EXPECT_TRUE(resolve_alias_with_ext_s->contains("kernel32.dll"));
      EXPECT_TRUE(resolve_alias_with_ext_s->contains("kernelbase.dll"));
    }
    EXPECT_TRUE(resolve_alias_with_ext_wc);
    if (resolve_alias_with_ext_wc) {
      EXPECT_EQ(resolve_alias_with_ext_wc->size(), 2);
      EXPECT_TRUE(resolve_alias_with_ext_wc->contains(L"kernel32.dll"));
      EXPECT_TRUE(resolve_alias_with_ext_wc->contains(L"kernelbase.dll"));
    }
    EXPECT_TRUE(resolve_alias_with_ext_ws);
    if (resolve_alias_with_ext_ws) {
      EXPECT_EQ(resolve_alias_with_ext_ws->size(), 2);
      EXPECT_TRUE(resolve_alias_with_ext_ws->contains(L"kernel32.dll"));
      EXPECT_TRUE(resolve_alias_with_ext_ws->contains(L"kernelbase.dll"));
    }
  }

  {
    constexpr char ALIAS_WITHOUT_EXT_A[] = "api-ms-win-core-util-l1-1-1";
    constexpr wchar_t ALIAS_WITHOUT_EXT_W[] = L"api-ms-win-core-util-l1-1-1";
    auto resolve_alias_without_ext_c = resolver.resolve(ALIAS_WITHOUT_EXT_A);
    auto resolve_alias_without_ext_s = resolver.resolve(std::string{ALIAS_WITHOUT_EXT_A});
    auto resolve_alias_without_ext_wc = resolver.resolve(ALIAS_WITHOUT_EXT_W);
    auto resolve_alias_without_ext_ws = resolver.resolve(std::wstring{ALIAS_WITHOUT_EXT_W});
    EXPECT_TRUE(resolve_alias_without_ext_c);
    if (resolve_alias_without_ext_c) {
      EXPECT_EQ(resolve_alias_without_ext_c->size(), 2);
      EXPECT_TRUE(resolve_alias_without_ext_c->contains("kernel32.dll"));
      EXPECT_TRUE(resolve_alias_without_ext_c->contains("kernelbase.dll"));
    }
    EXPECT_TRUE(resolve_alias_without_ext_s);
    if (resolve_alias_without_ext_s) {
      EXPECT_EQ(resolve_alias_without_ext_s->size(), 2);
      EXPECT_TRUE(resolve_alias_without_ext_s->contains("kernel32.dll"));
      EXPECT_TRUE(resolve_alias_without_ext_s->contains("kernelbase.dll"));
    }
    EXPECT_TRUE(resolve_alias_without_ext_wc);
    if (resolve_alias_without_ext_wc) {
      EXPECT_EQ(resolve_alias_without_ext_wc->size(), 2);
      EXPECT_TRUE(resolve_alias_without_ext_wc->contains(L"kernel32.dll"));
      EXPECT_TRUE(resolve_alias_without_ext_wc->contains(L"kernelbase.dll"));
    }
    EXPECT_TRUE(resolve_alias_without_ext_ws);
    if (resolve_alias_without_ext_ws) {
      EXPECT_EQ(resolve_alias_without_ext_ws->size(), 2);
      EXPECT_TRUE(resolve_alias_without_ext_ws->contains(L"kernel32.dll"));
      EXPECT_TRUE(resolve_alias_without_ext_ws->contains(L"kernelbase.dll"));
    }
  }
}

TEST(test_resolver, resolve_alter_versions_v6) {
  apiset::ResolverV6 resolver{};

  {
    constexpr char ALIAS_PATCH_DECREASED_A[] = "api-ms-win-rtcore-ntuser-private-l1-1-10";
    constexpr wchar_t ALIAS_PATCH_DECREASED_W[] = L"api-ms-win-rtcore-ntuser-private-l1-1-10";
    auto resolve_alias_p_d_c = resolver.resolve(ALIAS_PATCH_DECREASED_A);
    auto resolve_alias_p_d_s = resolver.resolve(std::string{ALIAS_PATCH_DECREASED_A});
    auto resolve_alias_p_d_wc = resolver.resolve(ALIAS_PATCH_DECREASED_W);
    auto resolve_alias_p_d_ws = resolver.resolve(std::wstring{ALIAS_PATCH_DECREASED_W});
    EXPECT_TRUE(resolve_alias_p_d_c);
    if (resolve_alias_p_d_c) {
      EXPECT_EQ(resolve_alias_p_d_c->size(), 1);
      EXPECT_TRUE(resolve_alias_p_d_c->contains("user32.dll"));
    }
    EXPECT_TRUE(resolve_alias_p_d_s);
    if (resolve_alias_p_d_s) {
      EXPECT_EQ(resolve_alias_p_d_s->size(), 1);
      EXPECT_TRUE(resolve_alias_p_d_s->contains("user32.dll"));
    }
    EXPECT_TRUE(resolve_alias_p_d_wc);
    if (resolve_alias_p_d_wc) {
      EXPECT_EQ(resolve_alias_p_d_wc->size(), 1);
      EXPECT_TRUE(resolve_alias_p_d_wc->contains(L"user32.dll"));
    }
    EXPECT_TRUE(resolve_alias_p_d_ws);
    if (resolve_alias_p_d_ws) {
      EXPECT_EQ(resolve_alias_p_d_ws->size(), 1);
      EXPECT_TRUE(resolve_alias_p_d_ws->contains(L"user32.dll"));
    }
  }

  {
    constexpr char ALIAS_PATCH_INCREASED_A[] = "api-ms-win-rtcore-ntuser-private-l1-1-12";
    constexpr wchar_t ALIAS_PATCH_INCREASED_W[] = L"api-ms-win-rtcore-ntuser-private-l1-1-12";
    auto resolve_alias_p_i_c = resolver.resolve(ALIAS_PATCH_INCREASED_A);
    auto resolve_alias_p_i_s = resolver.resolve(std::string{ALIAS_PATCH_INCREASED_A});
    auto resolve_alias_p_i_wc = resolver.resolve(ALIAS_PATCH_INCREASED_W);
    auto resolve_alias_p_i_ws = resolver.resolve(std::wstring{ALIAS_PATCH_INCREASED_W});
    EXPECT_EQ(resolve_alias_p_i_c, std::nullopt);
    EXPECT_EQ(resolve_alias_p_i_s, std::nullopt);
    EXPECT_EQ(resolve_alias_p_i_wc, std::nullopt);
    EXPECT_EQ(resolve_alias_p_i_ws, std::nullopt);
  }
}
