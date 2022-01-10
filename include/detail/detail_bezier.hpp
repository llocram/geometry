#ifndef GEO_DETAIL_BEZIER_HPP
#define GEO_DETAIL_BEZIER_HPP

#include <cmath>

namespace geo {

namespace detail {

template <std::size_t... I>
[[nodiscard]] consteval std::size_t
factorial_impl(std::index_sequence<I...>) noexcept
{
  return (1 * ... * (I + 1));
}

template <std::size_t N>
[[nodiscard]] consteval std::size_t
factorial() noexcept
{
  return factorial_impl(std::make_index_sequence<N>{});
}

template <std::size_t N, std::size_t K>
[[nodiscard]] consteval std::size_t
binom_coeff() noexcept
{
  return factorial<N>() / (factorial<K>() * factorial<N - K>());
}

template <std::size_t N, std::size_t... I>
[[nodiscard]] consteval std::array<std::size_t, N>
init_binom_coeffs_impl(std::index_sequence<I...>) noexcept
{
  std::array<std::size_t, N> retval {};
  auto helper = [&](std::size_t i, std::size_t result) {
    retval[i] = result;
  };
  (..., helper(I, binom_coeff<N - 1, I>()));
  return retval;
}

template <std::size_t N>
[[nodiscard]] consteval std::array<std::size_t, N>
init_binom_coeffs() noexcept
{
  return init_binom_coeffs_impl<N>(std::make_index_sequence<N>{});
}

template <std::floating_point T, std::size_t... I>
[[nodiscard]] constexpr T
pow_impl(T base, std::index_sequence<I...>) noexcept
{
  T result = base;
  [[maybe_unused]] auto pow_helper = [&result, base](T) {
    result *= base;
  };
  (..., pow_helper(I));
  return result;
}

template <std::floating_point T, std::size_t N>
[[nodiscard]] constexpr T
pow(T base) noexcept
{
  if constexpr (N == 0) {
    return 1;
  } else {
    return detail::pow_impl(base, std::make_index_sequence<N - 1>());
  }
}

template <concepts::bezier Bezier>
struct bernstein
{
  template <std::floating_point T>
  [[nodiscard]] static constexpr typename std::iterator_traits<traits::const_iter_t<Bezier>>::value_type
  evaluate_at(Bezier const & bezier, T t) noexcept
  {
    if (std::is_constant_evaluated()) {
      return ce_evaluate_at_impl(
        bezier, t, std::make_index_sequence<traits::degree_v<Bezier> + 1>{});
    } else {
      return evaluate_at_impl(
        bezier, t, std::make_index_sequence<traits::degree_v<Bezier> + 1>{});
    }
  }

  template <std::floating_point T, std::size_t... I>
  [[nodiscard]] static constexpr typename std::iterator_traits<traits::const_iter_t<Bezier>>::value_type
  ce_evaluate_at_impl(Bezier const & bezier, T t, std::index_sequence<I...> seq) noexcept
  {
      return (... + (binom_coeffs[I]
                    * pow<T, seq.size() - I - 1>(T(1.0) - t)
                    * pow<T, I>(t)
                    * *std::next(cecbegin(bezier), I)));
  }

  template <std::floating_point T, std::size_t... I>
  [[nodiscard]] static typename std::iterator_traits<traits::const_iter_t<Bezier>>::value_type
  evaluate_at_impl(Bezier const & bezier, T t, std::index_sequence<I...> seq) noexcept
  {
      return (... + (binom_coeffs[I]
                    * pow<T, seq.size() - I - 1>(T(1.0) - t)
                    * pow<T, I>(t)
                    * *std::next(cbegin(bezier), I)));
  }

  inline static constexpr std::array<std::size_t, traits::degree_v<Bezier> + 1> binom_coeffs
    = init_binom_coeffs<traits::degree_v<Bezier> + 1>();
};

} // namespace detail

} // namespace geo

#endif
