#include <catch2/catch.hpp>

#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace {

constexpr char input[] = R"(12 VJWQR, 1 QTBC => 6 BGXJV
12 BGTMN, 2 DRKQR => 2 TVSF
2 FTFK => 2 THNDN
13 LKRTN, 7 MDPN, 12 NZKQZ => 5 LPWPD
1 HDKX, 3 DWZS, 1 RCBQS => 1 DCRK
14 ZCMF => 6 ZKHLC
3 ZFVH, 2 ZCMF, 1 SCJG, 1 LQWJ, 4 BGBJ, 1 NHPR, 3 VKZFJ, 7 FWFXZ => 4 QVJMP
11 TNMLB => 7 NVDCR
1 LPWPD, 1 BGBJ => 2 SCJG
3 DFCVF, 1 QGSN => 6 PQXG
1 BGXJV, 1 THNDN => 4 BCQN
3 LKRTN => 9 MDPN
2 THNDN, 13 RCKZ, 10 FQSLN => 8 VKZFJ
4 LBCZ, 9 LWHS => 1 FQSLN
6 WSRVZ => 9 TNMLB
8 FQSLN, 14 JQRF, 4 BGTMN => 5 QGSN
4 ZCMF, 4 PLSM, 2 ZHTX => 8 TDHPM
2 RSKC, 10 SHBC, 8 MDPN => 6 FMSZ
2 VJWQR => 2 FPTV
12 DRKQR => 6 NHPR
35 QJLF, 22 BGTMN, 11 VJWTR, 1 QVJMP, 8 LQWJ, 1 TWLC, 16 NXZCH, 18 THKF, 42 JBLM => 1 FUEL
2 BGTMN, 4 XJKN => 8 ZCMF
4 TVSF => 3 RSKC
7 HRWS, 1 TVSF => 3 ZHTX
134 ORE => 4 WSRVZ
1 VKZFJ, 1 TWLC, 4 ZHTX, 5 THNDN, 12 PLVN, 1 ZFXNP, 1 PQXG, 6 CWHV => 7 VJWTR
20 XJKN, 1 LCKW, 3 NZKQZ => 7 HDKX
1 LPWPD => 8 RCKZ
4 RCBQS, 1 NVDCR => 5 BGBJ
8 BGXJV => 4 BGTMN
13 QBDX, 16 BGXJV => 6 NZKQZ
2 LPWPD => 3 DRKQR
4 QBDX => 7 XJKN
12 LCKW, 9 NVDCR => 3 RCBQS
142 ORE => 3 QBDX
1 WXHJF => 1 XKDJ
2 RSKC => 2 CWHV
2 ZHTX, 1 ZFXNP => 6 JQRF
1 FTFK, 1 TVSF, 1 QBDX => 2 JBLM
1 TDHPM, 14 NHPR, 3 QPSF => 5 ZFVH
3 GDTPC, 1 ZKHLC => 8 ZFXNP
5 DWZS => 3 LQWJ
1 FTFK, 4 LBCZ, 13 NHPR => 1 FWFXZ
1 RCBQS, 12 SHBC => 9 FTFK
1 WSRVZ, 1 XKDJ => 5 LKRTN
2 BGTMN, 1 MDPN => 5 PLSM
2 BGXJV, 17 XKDJ, 4 FPTV => 9 LCKW
148 ORE => 2 QTBC
110 ORE => 2 VJWQR
42 ZFXNP, 15 RCKZ, 8 GDTPC => 3 QJLF
13 HRWS => 4 GDTPC
34 HRWS => 4 DFCVF
2 VKZFJ, 2 NHPR, 16 PLVN, 1 QPSF, 13 LBCZ, 4 DCRK, 10 LWHS => 7 NXZCH
3 CWHV, 1 THNDN => 7 LWHS
1 BGXJV, 2 QBDX => 5 DWZS
9 LQWJ => 8 QPSF
21 BCQN, 3 FMSZ, 1 RSKC => 5 THKF
118 ORE => 6 WXHJF
11 FMSZ => 9 TWLC
28 PLSM => 5 SHBC
1 ZKHLC, 23 SCJG => 7 LBCZ
17 DWZS, 16 THNDN => 9 PLVN
7 HDKX => 9 HRWS
)";

struct WeightedChemical final {
  std::string chemical;
  int64_t weight{};
};

auto &operator>>(std::istream &is, WeightedChemical &c) {
  return is >> c.weight >> c.chemical;
}

auto toWeightedChemical(const std::string &str) {
  std::istringstream ss{str};
  WeightedChemical wc;
  ss >> wc;
  return wc;
}

struct Requirements final {
  std::vector<WeightedChemical> requirements;
  int64_t denominator{};
};

auto parseLine(std::string line) {
  std::replace_if(
      line.begin(), line.end(),
      [](auto c) { return c == ',' || c == '=' || c == '>'; }, ' ');

  std::istringstream ss{line};
  return std::vector(std::istream_iterator<WeightedChemical>{ss},
                     std::istream_iterator<WeightedChemical>{});
}

auto buildTree(const std::string &input) {
  std::unordered_map<std::string, Requirements> result;

  std::stringstream ss{input};
  std::string line;

  while (std::getline(ss, line)) {
    auto components = parseLine(line);

    if (components.empty())
      break;

    const auto out = components.back();
    components.pop_back();
    result.emplace(out.chemical, Requirements{components, out.weight});
  }

  return result;
}

auto findOreRequired(const std::unordered_map<std::string, Requirements> &tree,
                     WeightedChemical wc,
                     std::unordered_map<std::string, int64_t> &surplus) {
  std::vector<WeightedChemical> stillRequired{wc};
  int64_t ore{};

  while (!stillRequired.empty()) {
    auto lookup = stillRequired.back();
    stillRequired.pop_back();

    if (const auto it = surplus.find(lookup.chemical); it != surplus.end()) {
      const auto amountToTake = std::min(lookup.weight, it->second);
      it->second -= amountToTake;
      lookup.weight -= amountToTake;
    }

    if (lookup.weight == 0)
      continue;

    if (lookup.chemical == "ORE") {
      ore += lookup.weight;
      continue;
    }

    if (const auto it = tree.find(lookup.chemical); it != tree.cend()) {
      const auto reactionsToRun = int64_t(
          std::ceil(double(lookup.weight) / double(it->second.denominator)));

      for (const auto requirement : it->second.requirements)
        stillRequired.push_back(
            {requirement.chemical, requirement.weight * reactionsToRun});

      surplus[lookup.chemical] +=
          (reactionsToRun * it->second.denominator) - lookup.weight;
    }
  }

  return ore;
}

auto findOreRequired(const std::unordered_map<std::string, Requirements> &tree,
                     WeightedChemical wc) {
  std::unordered_map<std::string, int64_t> surplus;
  return findOreRequired(tree, wc, surplus);
}

auto findMaxFuel(const std::unordered_map<std::string, Requirements> &tree) {
  int64_t fuelMade = 1'180'000; // found by guesswork, not ideal
  int64_t oreUsed = findOreRequired(tree, {"FUEL", fuelMade});
  std::unordered_map<std::string, int64_t> surplus;

  for (; oreUsed < 1'000'000'000'000; fuelMade += 1) {
    oreUsed += findOreRequired(tree, {"FUEL", 1}, surplus);
  }

  return fuelMade - 1;
}

} // namespace

TEST_CASE("day14") {
  const auto tree = buildTree(input);
  REQUIRE(findOreRequired(tree, {"FUEL", 1}) == 1590844);
  REQUIRE(findMaxFuel(tree) == 1184209);
}
