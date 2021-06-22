#ifndef GEO_BEZIER_H
#define GEO_BEZIER_H

#include <concepts>
#include <vector>

#include "detail/detail_bezier.h"
#include "point.h"
#include "traits.h"

namespace geo {

/***************************** model ********************************/

template <
    std::size_t Degree,
    concepts::point Point,
    concepts::container Cont = std::vector<Point>
>
struct Bezier {
  template <concepts::no_array U = Cont>
  Bezier()
      : ctrls({}) {}

  template <concepts::array U = Cont>
  constexpr Bezier()
      : ctrls({}) {
    static_assert(sizeof(Cont) == (Degree + 1) * sizeof(Point));
  }

  template <
    typename... Args,
    concepts::no_array U = Cont,
    typename = std::enable_if_t<(!std::is_same_v<std::decay_t<Args>, Bezier> && ... )>
  >
  Bezier(Args && ... args)
      : ctrls(std::forward<Args>(args)...) {}

  template <
    typename... Args,
    concepts::array U = Cont,
    typename = std::enable_if_t<(!std::is_same_v<std::decay_t<Args>, Bezier> && ... )>
  >
  constexpr Bezier(Args && ... args)
      : ctrls{std::forward<Args>(args)...} {
    static_assert(sizeof(Cont) == (Degree + 1) * sizeof(Point));
  }

  Cont ctrls{};
};

/***************************** adaptors ********************************/

namespace traits {

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct tag<Bezier<Degree, Point, Cont>> {
  using type = bezier_tag;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct const_iter<Bezier<Degree, Point, Cont>> {
  using type = Cont::const_iterator;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct iter<Bezier<Degree, Point, Cont>> {
  using type = Cont::iterator;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct access_bezier<Bezier<Degree, Point, Cont>> {
  static const_iter_t<Bezier<Degree, Point, Cont>>
  cbegin(Bezier<Degree, Point, Cont> const & bezier) {
    return bezier.ctrls.cbegin();
  }

  static constexpr const_iter_t<Bezier<Degree, Point, Cont>>
  cecbegin(Bezier<Degree, Point, Cont> const & bezier) {
    return bezier.ctrls.cbegin();
  }

  static iter_t<Bezier<Degree, Point, Cont>>
  begin(Bezier<Degree, Point, Cont> & bezier) {
    return bezier.ctrls.begin();
  }

  static constexpr iter_t<Bezier<Degree, Point, Cont>>
  cebegin(Bezier<Degree, Point, Cont> & bezier) {
    return bezier.ctrls.begin();
  }
};

} // namespace traits

/***************************** algorithms ********************************/

template <
  concepts::bezier Bezier,
  std::floating_point T
>
constexpr std::iterator_traits<traits::const_iter_t<Bezier>>::value_type
evaluateAt(Bezier const & bezier, T t) noexcept {
  if (std::is_constant_evaluated()) {
    return detail::bernstein<T, geo::traits::const_iter_t<Bezier>>::evaluateAt(
      geo::ce_const_iter(bezier), t, std::make_index_sequence<4>{});
  } else {
    return detail::bernstein<T, geo::traits::const_iter_t<Bezier>>::evaluateAt(
      geo::const_iter(bezier), t, std::make_index_sequence<4>{});
  }
}

} // namespace geo

#endif
