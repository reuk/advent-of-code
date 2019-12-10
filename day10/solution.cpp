#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

struct Coord final {
  int64_t x{}, y{};
};

std::ostream &operator<<(std::ostream &os, const Coord &c) {
  return os << '(' << c.x << ',' << c.y << ')';
}

auto tie(const Coord &c) { return std::tie(c.x, c.y); }
auto operator<(const Coord &a, const Coord &b) { return tie(a) < tie(b); }
auto operator==(const Coord &a, const Coord &b) { return tie(a) == tie(b); }
auto operator!=(const Coord &a, const Coord &b) { return tie(a) != tie(b); }

auto operator+=(Coord &a, const Coord &b) -> auto & {
  a.x += b.x;
  a.y += b.y;
  return a;
}

auto operator-=(Coord &a, const Coord &b) -> auto & {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

auto operator+(const Coord &a, const Coord &b) {
  auto copy{a};
  copy += b;
  return copy;
}

auto operator-(const Coord &a, const Coord &b) {
  auto copy{a};
  copy -= b;
  return copy;
}

auto magSq(const Coord &a) { return a.x * a.x + a.y * a.y; }

template <typename Range> auto parseCoords(Range &&range) {
  int64_t x{}, y{};
  std::set<Coord> result;

  for (auto c : range) {
    switch (c) {
    case '\n':
      x = 0;
      y += 1;
      break;
    case '#':
      result.insert({x++, y});
      break;
    default:
      x += 1;
      break;
    }
  }

  return result;
}

template <typename T> auto gcd(T a, T b) {
  while (b != 0)
    a = std::exchange(b, a % b);
  return a;
}

auto computeIncrement(Coord c) -> Coord {
  const auto x0 = c.x == 0;
  const auto y0 = c.y == 0;

  if (x0 && y0)
    return {};
  if (x0)
    return {0, c.y < 0 ? -1 : 1};
  if (y0)
    return {c.x < 0 ? -1 : 1, 0};

  const auto common = gcd(std::abs(c.x), std::abs(c.y));
  return {c.x / common, c.y / common};
}

auto angle(Coord t) { return std::atan2(t.x, t.y); }

auto relativeMap(const std::set<Coord> &coords, Coord target) {
  std::map<double, std::vector<Coord>> map;

  for (auto c : coords) {
    if (c != target) {
      const auto d = c - target;
      map[angle(d)].emplace_back(d);
    }
  }

  for (auto &[angle, vec] : map) {
    std::sort(vec.begin(), vec.end(),
              [](auto a, auto b) { return magSq(a) < magSq(b); });
  }

  return map;
}

auto find200th(std::map<double, std::vector<Coord>> map, Coord target) {
  std::cout << map.size() << '\n';

  auto it = map.begin();
  for (auto i = 0; i != 200; ++i) {
    std::cout << target - it->second.front() << '\n';

    it->second.erase(it->second.begin());

    if (it->second.empty())
      it = map.erase(it);
    else
      it = std::next(it);

    if (it == map.end())
      it = map.begin();
  }

  std::cout << target + it->second.front() << '\n';
}

auto countVisible(const std::set<Coord> &coords, Coord target) {
  std::set<Coord> uniqueIncrements;
  for (auto other : coords)
    if (other != target)
      uniqueIncrements.emplace(computeIncrement(target - other));
  return uniqueIncrements.size();
}

constexpr char input[] = R"(.#..##.###...#######
##.############..##.
.#.######.########.#
.###.#######.####.#.
#####.##.#.##.###.##
..#####..#.#########
####################
#.####....###.#.#.##
##.#################
#####.##.###..####..
..######..##.#######
####.##.####...##..#
.#####..#.######.###
##...#.##########...
#.##########.#######
.####.#.###.###.#.##
....##.##.###..#####
.#.#.###########.###
#.#.#.#####.####.###
###.##.####.##.#..##)";

auto main() -> int {
  std::cout << std::atan2(0, 1) << '\n';
  const auto coords = parseCoords(input);
  const auto [mostVisible, pos] =
      std::accumulate(coords.cbegin(), coords.cend(),
                      std::tuple{size_t{0}, Coord{}}, [&](auto acc, auto c) {
                        const auto visible = countVisible(coords, c);
                        if (visible < std::get<0>(acc))
                          return acc;
                        return std::tuple{visible, c};
                      });
  std::cout << mostVisible << " (" << pos.x << ", " << pos.y << ")\n";
  find200th(relativeMap(coords, pos), pos);
}

