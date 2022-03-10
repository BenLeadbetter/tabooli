#pragma once

namespace tabooli::detail {

// pinched from
// https://arne-mertz.de/2018/05/overload-build-a-variant-visitor-on-the-fly/

template <class ...Fs>
struct overload : Fs... {
  overload(Fs const&... fs) : Fs{fs}... {}
  using Fs::operator()...;
};

}