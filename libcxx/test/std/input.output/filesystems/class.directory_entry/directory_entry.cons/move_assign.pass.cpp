//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: host-can-create-symlinks
// UNSUPPORTED: c++03, c++11, c++14

// <filesystem>

// class directory_entry

// directory_entry& operator=(directory_entry const&) = default;
// directory_entry& operator=(directory_entry&&) noexcept = default;
// void assign(path const&);
// void replace_filename(path const&);

#include <filesystem>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "filesystem_test_helper.h"
namespace fs = std::filesystem;

static void test_move_assign_operator() {
  using namespace fs;
  // Copy
  {
    static_assert(std::is_nothrow_move_assignable<directory_entry>::value,
                  "directory_entry is noexcept move assignable");
    const path p("foo/bar/baz");
    const path p2("abc");
    directory_entry e(p);
    directory_entry e2(p2);
    assert(e.path() == p && e2.path() == p2);
    e2 = std::move(e);
    assert(e2.path() == p);
    assert(e.path() != p); // testing moved from state
  }
}

static void move_assign_copies_cache() {
  using namespace fs;
  scoped_test_env env;
  const path dir = env.create_dir("dir");
  const path file = env.create_file("dir/file", 42);
  const path sym = env.create_symlink("dir/file", "sym");

  {
    directory_entry ent(sym);

    fs::remove(sym);

    directory_entry ent_cp;
    ent_cp = std::move(ent);
    assert(ent_cp.path() == sym);
    assert(ent_cp.is_symlink());
  }

  {
    directory_entry ent(file);

    fs::remove(file);

    directory_entry ent_cp;
    ent_cp = std::move(ent);
    assert(ent_cp.path() == file);
    assert(ent_cp.is_regular_file());
  }
}

int main(int, char**) {
  test_move_assign_operator();
  move_assign_copies_cache();

  return 0;
}
