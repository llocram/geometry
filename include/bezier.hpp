#ifndef GEO_BEZIER_HPP
#define GEO_BEZIER_HPP

#include <vector>

#include "detail/detail_bezier.hpp"
#include "point.hpp"
#include "traits.hpp"

namespace geo {

/***************************** model ********************************/

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont = std::vector<Point>
>
requires concepts::bezier_degree<Degree>
struct Bezier
{
  template <concepts::not_an_array U = Cont>
  Bezier()
      : ctrls({})
  {}

  template <concepts::array U = Cont>
  requires (sizeof(Cont) == (Degree + 1) * sizeof(Point))
  constexpr Bezier()
      : ctrls({})
  {}

  // TODO check if disable_if can be replaced with C++20 constrains
  template <
    typename Head,
    typename... Tail,
    concepts::not_an_array U = Cont,
    typename = traits::disable_if_t<
      sizeof...(Tail) == 0
      && std::is_same_v<std::decay_t<Head>, Bezier>
    >
  >
  Bezier(Head && head, Tail && ... tail)
      : ctrls(std::forward<Head>(head), std::forward<Tail>(tail)...)
  {}

  // TODO check if disable_if can be replaced with C++20 constrains
  template <
    typename Head,
    typename... Tail,
    concepts::array U = Cont,
    typename = traits::disable_if_t<
      sizeof...(Tail) == 0
      && std::is_same_v<std::decay_t<Head>, Bezier>
    >
  >
  requires ((sizeof(Cont) == (Degree + 1) * sizeof(Point)))
  constexpr Bezier(Head && head, Tail && ... tail)
      : ctrls{std::forward<Head>(head), std::forward<Tail>(tail)...}
  {}

  Cont ctrls{};
};

/***************************** adaptors ********************************/

namespace traits {

template <
  std::size_t Degree,
  concepts::point Point,
  typename Cont
>
struct tag<Bezier<Degree, Point, Cont>>
{
  using type = bezier_tag;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct degree<Bezier<Degree, Point, Cont>>
{
  static constexpr std::size_t value = Degree;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct const_iter<Bezier<Degree, Point, Cont>>
{
  using type = typename Cont::const_iterator;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct iter<Bezier<Degree, Point, Cont>>
{
  using type = typename Cont::iterator;
};

template <
  std::size_t Degree,
  concepts::point Point,
  concepts::container Cont
>
struct access_bezier<Bezier<Degree, Point, Cont>>
{
  [[nodiscard]] static const_iter_t<Bezier<Degree, Point, Cont>>
  cbegin(Bezier<Degree, Point, Cont> const & bezier) noexcept
  {
    return bezier.ctrls.cbegin();
  }

  [[nodiscard]] static constexpr const_iter_t<Bezier<Degree, Point, Cont>>
  cecbegin(Bezier<Degree, Point, Cont> const & bezier) noexcept
  {
    return bezier.ctrls.cbegin();
  }

  [[nodiscard]] static iter_t<Bezier<Degree, Point, Cont>>
  begin(Bezier<Degree, Point, Cont> & bezier) noexcept
  {
    return bezier.ctrls.begin();
  }
};

} // namespace traits

/***************************** algorithms ********************************/

template <concepts::bezier Bezier>
[[nodiscard]] constexpr typename std::iterator_traits<traits::const_iter_t<Bezier>>::value_type
evaluate_at(Bezier const & bezier, std::floating_point auto t) noexcept
{
  return detail::bernstein<Bezier>::evaluate_at(bezier, t);
}

} // namespace geo

#endif
