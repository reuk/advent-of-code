#include "aoc_coord.hpp"
#include "aoc_direction2d.hpp"

#include <catch2/catch.hpp>

#include <charconv>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <span>
#include <sstream>
#include <string>
#include <variant>

namespace {
auto constexpr test_input = R"(Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...

)";

auto constexpr input = R"(Tile 2411:
.#.##..#.#
.#.......#
.##...#.#.
......#...
#.#.......
##.....#.#
#..#...#..
##........
##....#...
##..#.##.#

Tile 1997:
#.#......#
.#......##
#.##..#..#
...#..#.#.
.#.#...#..
#.........
..#..#....
#.#..#.##.
#..##.....
#...#.#.#.

Tile 1427:
#...#..#..
..##.....#
..#...####
#..#.#...#
..#.#.#..#
..####....
#.#.###.#.
......#...
#.........
#.#.##...#

Tile 2161:
###.####.#
#....###.#
#.#......#
..#....#..
...#.....#
.#..#...##
#.#.....##
#..#......
.....#.#..
...###.#.#

Tile 1321:
###...#.#.
.#...#..##
.#.......#
..........
#.#.......
....#.....
######....
#.....#...
##.......#
#.###..###

Tile 1181:
.#..#..###
###...#.##
#.........
....###..#
#.#...#..#
#....###..
##..##...#
#....##.##
#.......#.
###.##...#

Tile 2749:
#####..##.
##........
......#.#.
#..#...#.#
.....##.#.
.....#...#
#.........
#........#
....#..#.#
.#..#...#.

Tile 3911:
##...#####
#...#..#.#
.##...#..#
#...#.....
#..#.#...#
..#......#
#.#.......
....##..#.
..#.#..##.
####.#..#.

Tile 3257:
#..#.##...
.#.....#.#
##.#.....#
##.#.#....
#..##..#..
........##
#.##...#.#
#.#..#...#
........##
.##.####.#

Tile 2237:
.###.#####
....#....#
.......###
#........#
..#.......
.#.#......
#...##...#
#.....#.#.
#...#.....
####.#####

Tile 2389:
#.#.###...
#.#.#...##
......#...
.####....#
..#..#..#.
#.......##
..###....#
#........#
#....##...
..#.#..##.

Tile 3209:
..##.#####
.........#
##..#.##..
...#.#...#
....#.#..#
#...#.....
#.#.#.....
#...#....#
#.#......#
...#..####

Tile 2579:
#...#.##..
....##.###
#......###
.......#..
##...#....
#.#......#
.##.#....#
.#.......#
#...#.....
##..#.##..

Tile 2087:
.###.##.#.
.##...##.#
..####.#.#
...#......
...#......
......#..#
#.##......
.#.#.#.#..
....##..##
...##.#..#

Tile 3517:
###.#...##
#.........
#.....#...
###..#.#..
##......#.
..#....#.#
#.....#...
..#...#.##
##...#.#..
##..##.###

Tile 3347:
##.#..#...
.........#
..........
.#.#.....#
..........
....#..#.#
#........#
##.#...#.#
....#..#..
##.#.#....

Tile 2711:
.##.#.#...
...#.#....
..........
#......###
..##......
#.........
#..#..##..
.##...##..
#......#..
#..#.##.#.

Tile 3877:
###.#..#.#
#...##...#
..#.#..#.#
##......##
.#........
##..#....#
..###..#.#
#.#.......
#.#......#
...###.###

Tile 1721:
###...##.#
......#..#
.##.##....
....##.#..
##..#..#..
###..#..#.
####..##.#
#.#.#....#
.......#.#
#..##....#

Tile 3457:
.#.#..##.#
..##.#.#..
##...#...#
...#......
..#....##.
#...#.#..#
..#.......
...#......
........#.
.#.#.##..#

Tile 1231:
.#.......#
#........#
#..#..#...
..#......#
.....#....
#.........
#...#..#.#
.....#..#.
.#.#....#.
###...##..

Tile 1901:
.###.#.#.#
...##.....
#......#.#
...##.....
....#.....
#.........
..#......#
##.....#..
#.........
#..##..###

Tile 3329:
#..#..#.#.
....#.....
###.....##
.........#
##..#.....
#.......##
#..#.....#
#..#.#..#.
...#......
...#...##.

Tile 1291:
..#....#.#
#.#...##.#
#..#....##
#..###...#
#...#.#...
####......
#.....##.#
#.#..#....
##....#..#
#.###.#...

Tile 1481:
#..#...##.
....#..#..
...##.....
#..#..##..
###.#..##.
##.#.##...
#..#.....#
#......#.#
#.#....#..
.#..#..#.#

Tile 2063:
.###..#..#
...#......
#..#.....#
#..#..#...
.....#.###
...#......
#.#.....#.
#........#
.#...#...#
##.#....##

Tile 2297:
.#..#..#.#
#..#......
#......#.#
#.#..###.#
..........
##..#.#.##
#......#..
#....#.#.#
#........#
.#..###...

Tile 2731:
..#.......
......#...
#..#...#..
..........
#..#...#..
#.#..#..##
##..#..##.
..........
#.##..#...
##....##..

Tile 1453:
....#.##.#
#..#..##.#
##.#.#....
#..##.#...
#......#..
#......###
...##...#.
#.##......
##........
#######.##

Tile 1117:
.###.#...#
.#.#.....#
..#.#...##
#.......#.
#.....#..#
#.###....#
##.#......
#.####..##
#..#..#.#.
##...#####

Tile 1823:
#.....#...
#......##.
.....#.#..
......###.
..#..###.#
....#.##.#
.##..#.#.#
#.#.......
##........
.#...##..#

Tile 3163:
..#####.##
...#......
#..###...#
..##..#...
.#.......#
..#....#.#
###.#.....
#..#..#...
..##.#....
.##.#.#..#

Tile 2083:
#..######.
....##.#.#
.....###.#
#...#..#..
#....#...#
...#...#.#
#..####...
#....#....
#..#....#.
#....#.#..

Tile 2659:
#.###.#...
...#......
#..#....##
..###.##..
##.......#
#.......#.
#....#...#
#..#.....#
..#.#.....
..#..##.#.

Tile 2113:
.##.######
.......###
#....#....
....#..#..
#.##......
####..##..
.##..####.
...#....##
.#.....#..
#.#.##..##

Tile 3943:
###..#....
.#.....#..
#.........
##....##..
#.#....#.#
#.#.#.....
#....#..#.
.##..##..#
....##....
.#.#.#####

Tile 3719:
#...#.#.#.
.........#
#.#.......
......#.#.
.#...#.#.#
.#....####
##........
.##..#.#.#
#.........
#.#.#....#

Tile 1409:
####.#####
#....#....
##.#....#.
#..#......
#....#....
#.#...#...
##.....#..
#.....#..#
......#...
..####...#

Tile 3547:
.#########
.#.##...#.
...#..#.#.
#.##...###
#......#..
##...#....
##.#.#.###
##........
.....#...#
###.##.##.

Tile 1123:
...##....#
.........#
#...#...##
..........
#.........
#......#.#
#..#.#.#.#
......#..#
#.#...#.##
####.#....

Tile 2203:
.####.#..#
##........
##.......#
#....#....
.#......##
.#..#..#.#
......#..#
##.###...#
#....#....
.##..##.##

Tile 3511:
#.#..#.#.#
#......###
#..#....#.
#.####....
##...#...#
#.##...##.
##.##..##.
.....#...#
#...##...#
.#.#..##.#

Tile 3499:
..#..#.###
#.........
..##..#...
...#.#.#.#
...#.#.#..
#.....##.#
...#......
..#...###.
##..###..#
#.#....###

Tile 1039:
#.#....##.
#..###.#.#
###.....#.
##..#....#
.#....##.#
#..#######
.##.#....#
#..#......
##.#...##.
#..#..##.#

Tile 3323:
#..#.#.##.
#......#..
.........#
.........#
.#....##.#
#.....#..#
#....##...
#...##...#
.###...#.#
#...###.##

Tile 1033:
###...##.#
#..#..#.##
.........#
.#.#.#..#.
#.........
#......##.
#...#.##.#
..........
###.......
.##.#.....

Tile 2269:
....#.##..
#........#
.#..#.#..#
#......#.#
#...#.###.
#..#.....#
....#....#
........##
#.#...#...
.#...#.#.#

Tile 1279:
...##.###.
....#...#.
#..#......
#......##.
.#.#....##
.#...##..#
..#..#...#
#......#..
#..#....#.
.#.#...#.#

Tile 2549:
..#..###..
..........
........##
....#....#
.........#
#...#..#.#
....##..#.
.#........
..##..#...
##.##.####

Tile 2377:
..#.######
..##..#..#
..#.....#.
#.#.#.#...
#....#....
..........
#..##....#
#...####..
.......#.#
#.#.#..##.

Tile 3169:
.######.##
#..#..####
#.....#..#
#.##..##.#
....#...##
..##......
#..#...###
.....##...
#......###
#.#....#..

Tile 1979:
###.#.##..
#..#...#..
#..#..#.#.
....##....
#.#.......
#..#.....#
....#.....
.#..#..#.#
#.#.###..#
#.####.#.#

Tile 2131:
..##..#.##
##...#....
#...#.#..#
.....#...#
.#.#.#..#.
##......#.
......#...
#..##.....
.###...###
#...######

Tile 2531:
.#.######.
....#..#.#
...###...#
.........#
#....#.#..
.....#...#
#...#....#
....#.....
..#.......
.....#.##.

Tile 3167:
####.###..
#..#.....#
#.......##
##..#..#.#
#.........
#.....#.#.
##.......#
...#.#....
.........#
######..##

Tile 3191:
#....#####
#...#.....
..##...#..
#.......#.
#..#.###.#
#####...##
#.#...###.
#.#..#.##.
#.........
#.##.##.#.

Tile 3767:
#..#..####
#......#..
#.#.##....
.....#..#.
.#...#.#..
..#.#..###
..##......
.###.##..#
.##....#.#
.##.##..##

Tile 2633:
##...##.##
#....#..##
...##.#...
#.#.#.....
.......###
.#...#....
#....#.###
.#.....#.#
##....#...
....#..##.

Tile 2677:
...#.##.#.
......#...
.....#....
#......#..
#...#.###.
..#....#.#
.........#
#..#...#.#
#.#....###
#.#..#....

Tile 3989:
.###.###.#
#.#..#..##
......#.#.
...#......
..####...#
....###.#.
#.###.#..#
....##....
#.........
.#.##.####

Tile 2897:
####...#.#
..........
#.#.......
#...#.....
......#...
##.###.#..
.##..##..#
...##.##..
...##...#.
.#.######.

Tile 3803:
#.#..#.##.
###....#..
#...#..#.#
.......###
#.....#..#
##.....#..
##..##..##
.#....##.#
.###......
#..#.#####

Tile 1187:
.#.#####..
.#.#......
#.#.####.#
#...#.....
..#.......
.#.....#..
#...#.#..#
#......###
#.##..#..#
#.##.##.##

Tile 1801:
#.##...#..
.##......#
#..####..#
#.##...#.#
#......#.#
.....#.#..
..###...#.
..#.#.....
#...#.#...
#....####.

Tile 2729:
.......#..
#.#...#..#
..#.#..#..
.....#.#..
#.........
#.#..#....
.#.......#
#.#....#.#
#.#...#.##
##.#.###.#

Tile 2969:
.......#.#
##..#....#
#.....#.##
##.....#.#
#..#......
.#.#.#....
#....#.#.#
##......##
#......##.
.##.##....

Tile 3079:
...##.####
.....#...#
..........
..#.#.....
.........#
.#...##.##
.###.....#
..#.#..##.
#...##...#
.##...##..

Tile 3407:
...#.#.#..
.#...##.##
##....#..#
.#....#.#.
.##.#....#
##......##
#...#...##
#..#.....#
#.....#...
##....##.#

Tile 3389:
####.#..#.
#..##.....
##...##.#.
#......#..
.....#..##
#...##..##
#...##...#
...##..#..
....##..##
#.#...#...

Tile 1663:
#...##.##.
..#..#.#.#
#.....#.#.
#.........
..##.##..#
.....#..#.
#....#....
#.##......
.##.#...##
.....#...#

Tile 1787:
#..##....#
......#.#.
#......##.
#.#......#
#....#...#
#.#.......
.....#...#
#...#..#..
.#......##
..###.#.##

Tile 2917:
##.#.#####
#..#..#.##
..##..#..#
#..#.##.##
#.#..#....
.#.#.##..#
.####.....
#..#...#.#
#...#....#
.#.#.##..#

Tile 3067:
##.#.#...#
.###..#..#
.##.#.#...
#.#.....##
..#.......
#...#...#.
##..#....#
...##..#.#
#.......#.
.###...#.#

Tile 2963:
.#..#.#...
#..#.##..#
##........
..#....###
###.#.#...
.#.#.#...#
..#.......
.#.#.#.#.#
#........#
#..#.#####

Tile 1753:
###.##.##.
#..#.#..#.
.....#...#
#.........
##.#..##.#
.#.#.#..#.
#......#.#
...#..#..#
#.#..#..#.
#.#..#.#.#

Tile 1609:
..#.####.#
#........#
..#.##.#.#
......#.#.
.....#....
#.......#.
..........
#.........
#..##.....
##...#.#.#

Tile 1237:
.#.##.##.#
...#.##...
..#......#
##...#..##
...#..#.##
.#.#####..
...#.##..#
#....#....
...#..#..#
#..#...##.

Tile 2267:
.#..#..###
...#.#...#
..#..#..##
##....###.
......#..#
..#.......
....#.#.#.
....##...#
###.....##
..##...#.#

Tile 1777:
...#...###
.......#..
##.#..#..#
.........#
#....##.#.
#.#..##.#.
.#..##.#.#
....#.#.##
.###..#.##
......##.#

Tile 1993:
..#.#..##.
#..#.#.#..
.#.#...##.
.......#.#
###.#.#...
#..#....#.
#...##...#
#...##...#
......##..
##.#.###.#

Tile 3571:
.###.#...#
...##.....
##..#.....
..#...#..#
......#.##
..#.##...#
##.#..#..#
#.###...##
..####....
..#.#..#..

Tile 3581:
#.#.#.#.#.
#.#.......
#.#....#..
#.###...#.
#.........
.........#
.#.......#
##........
#.##.....#
##.#..##.#

Tile 2383:
#..##..#..
..#...#..#
#...#.#.##
#.##.....#
....#..#..
...#.##..#
...#.....#
..###.#...
....##..#.
#...###..#

Tile 2591:
##...##.##
..#..#.#..
...#.....#
#.###.....
.....####.
...###.#..
..#....#..
#.........
....##...#
###.###.##

Tile 1153:
##.#...#.#
###......#
#....#.#..
....#...##
#...#...#.
..##....##
#..#...#..
#........#
.#..#.####
..##..#...

Tile 1489:
#.#.###..#
##.#.#..##
.#..##..#.
....#....#
#.#..#.#..
.#.......#
#......#..
##.....#.#
....##.##.
#....#####

Tile 1193:
##..###...
##.#......
......#..#
##......#.
#....#####
.#.#....##
...#..#..#
#..#......
##....##.#
########..

Tile 1163:
##.....###
.#.......#
#...#.##..
#..#...#..
...##.#...
.........#
....#...##
.####.#...
#.#.......
.#..##.###

Tile 2137:
##..#..#..
.#.#...#.#
#........#
#.........
..#.......
#...#....#
..##.....#
#....#.#..
..#......#
..##.##...

Tile 1559:
#..##..#.#
.........#
#..##.##..
#........#
#..#..#..#
....#...##
...#.....#
#......#..
.#........
###..####.

Tile 2521:
.###..#...
#..#......
....#.#.#.
#.....#.##
.#..#...#.
.........#
##...#...#
#...#.....
#..##.#.#.
##..######

Tile 1171:
..#####.##
..##.....#
.....#....
#.#.....##
#.#...#..#
#.##..#..#
#####....#
#..#.##...
##.......#
#.#.#.....

Tile 1361:
..##.###.#
.....##..#
###....#..
..........
#.......##
#..#.....#
#.........
.#.....#..
.......#.#
..##..#...

Tile 1259:
#.##..####
#...#..#..
..###....#
....#....#
...###....
...#..#.##
#.#.##....
##..##...#
##....#..#
#.###..##.

Tile 1987:
##.###..#.
.....#.#.#
#......#.#
#......#.#
..#.#.....
.#..#....#
.####.##..
..##.###.#
#........#
.....#....

Tile 2777:
#.#.##..#.
#...#.#..#
..#......#
...##.....
#.#......#
#........#
#....#....
.......##.
.....#..##
#..##.#..#

Tile 3673:
...###.###
###..#...#
..........
..#.#.##..
#.#..#....
#......#.#
..##......
.#...#.#..
#......#.#
##..#..##.

Tile 2819:
#.##.#####
#.....#.#.
####...#.#
.....#...#
#...#.#.#.
#..#...###
......#..#
....#.#...
.....#.#..
..#######.

Tile 1699:
###....#.#
......#..#
#.........
......#..#
#...#.##..
...#.....#
.........#
...##..#.#
#.........
.#.....##.

Tile 2351:
.#####.#.#
##....##..
###...#.#.
..#.#....#
.........#
.#.#.....#
..#...##..
#..#.#....
........##
###.#...##

Tile 1399:
.###...#..
#.#......#
.##....#.#
...#..#...
#.#.#..##.
#.......#.
#....#..##
....#...#.
#.....#..#
..#..#.#.#

Tile 3793:
##..####.#
.##...#..#
#.....#..#
#...#.....
.....#...#
..........
#....#...#
...##..#..
.#..##....
....#.#.##

Tile 3701:
.....##.#.
#..#.##...
........##
.........#
#..##...#.
#...#.#..#
..#..#.#..
#.....####
#...#..##.
...#..###.

Tile 2707:
#.###.#.#.
#......#..
####...#..
..........
#.#..##..#
#####..#.#
#..#..#.#.
###......#
#..#..#.#.
....#.#...

Tile 3559:
##.#.##.##
#....#...#
...##..#.#
..#.##....
.....#....
..##..#.##
....#.....
......##..
...###....
#.##...#..

Tile 2381:
..#...#.##
..#...#.#.
###...#...
##..##....
....#.##.#
...#......
.#..#....#
..........
##....#..#
###.#.##.#

Tile 1973:
.#...##...
#.#.#...##
#....#..##
#......###
......#..#
.........#
...##..###
.##..#..#.
#.....##.#
...##..###

Tile 3607:
#..#####..
.#.###.#.#
...##...#.
..#......#
.#.#......
##.#....##
###....#..
#...#...##
.#.....#..
#####..#..

Tile 2251:
##..#..#..
#........#
.........#
...#...#..
#.......##
.#.#..#..#
.##.##...#
###...##..
#........#
#.#..#####

Tile 3947:
..#.##..##
###.......
#.#..#....
#.#......#
#.#...#...
##.#.##..#
....##....
...##.#...
#.#.....#.
.##.#####.

Tile 3259:
.#.#####..
.....#.#..
##........
.....#....
#.#.......
.........#
......#...
......#..#
.##.......
#..#.##.##

Tile 3761:
.#....####
.##..#...#
........#.
###.#.....
#.....#..#
..##....##
........##
...#..#..#
...#.#....
###.######

Tile 2473:
#..###...#
##.....##.
#..###....
##....##..
.....#.#..
#.......#.
..#.....#.
......##..
....#....#
#...#.#...

Tile 2767:
##.##.#.#.
.#.####...
..##....##
#...#.....
..#..#...#
#.........
#........#
#...###...
#.#...##.#
.###..##..

Tile 2281:
#...#..#.#
...#......
.........#
#....#....
.......#..
.....##..#
#.#....#.#
##..##...#
........##
#....#...#

Tile 1693:
#.##.#####
......#...
#.#...##..
....#.#.#.
#..#...###
##.##.##.#
..#...####
#..##....#
#......#.#
#....###.#

Tile 2417:
.###.##.#.
##...#...#
#..#..##.#
#...#..#..
.........#
...#.....#
..##.#..#.
#.........
....##....
#.##.###.#

Tile 2879:
..#.#..#..
..##..#.#.
##......##
#..##...##
#.##..#...
#.......#.
.......#.#
##..#..#.#
#...#....#
#....#.##.

Tile 2851:
.......#.#
....#.....
##.###.##.
.......#..
.#..###..#
....###..#
#..#......
...#.#...#
.#.###...#
###.###.##

Tile 3931:
.....##.##
###....#..
#..#....##
#.#.....##
##...#....
#..##..#.#
.......#.#
...#..#.##
.#...#....
##....#.##

Tile 3181:
...##...#.
....#....#
#......#.#
#.###....#
##.......#
.#..#.#..#
###..#.#..
#....#.#..
#.......##
###..###..

Tile 2647:
#..#..#.##
#........#
#..#..##..
.....#...#
....##.#.#
#..###...#
..#......#
#.#..###..
#.......##
####...#..

Tile 2441:
..#..###..
##..###..#
##.##...#.
#.##.....#
.......###
...#...###
#...#.....
.#..####.#
.#.#.#..#.
#.#.##.#..

Tile 3733:
...#....##
..#..##..#
.....#..#.
##........
..#....#..
#....##.##
.#....#..#
.#.##.##..
#.##..#.#.
.#..####.#

Tile 1607:
#.#.#.#.##
#.........
.....#....
.#.#..#..#
..#......#
#.....#...
#.#..#....
......#.##
#.##...#..
..#.######

Tile 3301:
....##...#
#..##....#
....#.....
#.....###.
#....##.#.
#.#...#..#
......##.#
...##.....
.........#
..#######.

Tile 1109:
..#...###.
.#.......#
#.....#...
..........
##.#.#....
#...##....
#.....###.
.#....####
.#....#.##
###.#...#.

Tile 2789:
.#.#.####.
..........
#...#.#.#.
#..#....##
.....##...
.#..##.#.#
........##
#.#.#.#...
......#.##
#...#..#..

Tile 2593:
...#.#.#.#
#......###
..#.##....
#.......#.
...#.#...#
#..##....#
#..##.....
#.##.#...#
#..###..#.
.#....###.

Tile 2539:
...###..#.
..........
.........#
.....###..
.......#..
......#.#.
.#..#...##
#....#.#..
##...#....
.###.#.#.#

Tile 3307:
##.##.#...
.#..#.....
#..#..#..#
#.....##.#
..#.#.....
##....##..
.....#....
#..#.#....
######...#
####.#....

Tile 1613:
###..###.#
...#...#..
#.#..##...
......##.#
......#..#
..##......
#.....#..#
##.#..##.#
###...#..#
#.#..##.#.

Tile 3727:
..#...#.##
#.#......#
#.###...##
..####..#.
#.........
.....#...#
#..##....#
#.#.....##
#....###.#
..#..##.##

Tile 2017:
#...##..##
#..#..##.#
#........#
.....#..##
#..#.....#
....#....#
#....#....
.##.......
.......#..
#..##..#..

Tile 1933:
###.#.....
#.#.####..
..#..##...
##..###..#
###..#.###
..#.#....#
#.........
..#..#.###
#.#..###..
...#.#.#..

Tile 2239:
.#.#..#.##
...#.##..#
#...#....#
#..#......
#....#..#.
#..##..#.#
..#.#.#..#
#....#..#.
#...#.#...
.#.##.####

Tile 3449:
.##...###.
..#......#
.......#.#
..##......
#.###.##.#
....##.#..
........#.
##.#......
...#.#....
..###....#

Tile 2347:
..##....#.
#.........
..........
....#.#..#
.........#
##.##.#..#
......#..#
.#.##...#.
#.#####.##
.#.#....##

Tile 3023:
#...#.##..
.......#..
#.#..#.##.
#...#.#..#
..#.#...##
#.##...##.
......##..
.....#####
.#...##...
#..#.#....

Tile 3541:
###.######
..#.....#.
#....###..
......#...
..#..##..#
#........#
#..#......
###..#.##.
..#....#.#
#..###.#.#

Tile 3709:
##.#.##..#
#.........
.##.##...#
.#..#.#..#
#.......##
......#...
.#..#.....
.......#.#
#..##....#
.##.......

Tile 1657:
#.##.#...#
...#.#..#.
.....#..##
.....#.#..
######.###
##........
......#..#
#..#...#..
.......#..
...#....#.

Tile 1031:
####.#.##.
..........
.#...#..#.
...#.....#
.##.#....#
....#.##..
........##
#..#..#.##
#.##.....#
#.####...#

Tile 3691:
.#.#######
........##
#.#.......
#..##..#..
#.#.##.##.
....###.#.
.#..#..#..
..#.##.#..
.....#....
##.###...#

)";

using Contents = std::vector<std::string>;

struct Tile {
  int64_t number{};
  Contents contents;
};

std::istream &operator>>(std::istream &is, Tile &tile) {
  tile.contents.clear();

  std::string line;
  std::getline(is, line);

  if (std::smatch m; std::regex_match(line, m, std::regex{"Tile ([0-9]+):"})) {
    auto const match = m[1].str();
    std::from_chars(match.data(), match.data() + match.size(), tile.number);
  }

  while (std::getline(is, line)) {
    if (line.empty())
      break;

    tile.contents.push_back(line);
  }

  return is;
}

std::string get_edge(Contents const &t, aoc::Direction2d e) {
  if (e == aoc::Direction2d::up)
    return t.front();

  if (e == aoc::Direction2d::down)
    return t.back();

  std::string result;

  for (auto const &str : t)
    result.push_back(e == aoc::Direction2d::left ? str.front() : str.back());

  return result;
}

Contents get_edges(Contents const &t) {
  Contents result;

  for (auto const &e : aoc::directions2d) {
    auto str = get_edge(t, e);
    result.emplace_back(str.rbegin(), str.rend());
    result.emplace_back(std::move(str));
  }

  return result;
}

bool is_corner_tile(Tile const &tile,
                    std::map<std::string, std::vector<int64_t>> const &map) {
  auto const edges = get_edges(tile.contents);
  return std::count_if(edges.cbegin(), edges.cend(), [&](auto const &edge) {
           return map.find(edge)->second.size() == 1;
         }) > 2;
}

auto get_surrounding(aoc::Coord const &p) {
  return std::array{
      std::tuple{aoc::Coord{p.x - 1, p.y}, aoc::Direction2d::right},
      std::tuple{aoc::Coord{p.x + 1, p.y}, aoc::Direction2d::left},
      std::tuple{aoc::Coord{p.x, p.y - 1}, aoc::Direction2d::down},
      std::tuple{aoc::Coord{p.x, p.y + 1}, aoc::Direction2d::up}};
}

Contents flip(Contents const &t) { return {t.rbegin(), t.rend()}; }

Contents rotate(Contents const &t) {
  Contents rotated(t.front().size());

  for (auto const &line : t) {
    size_t index = rotated.size();

    for (auto const &c : line)
      rotated[--index].push_back(c);
  }

  return rotated;
}

std::vector<Contents> get_versions(Contents t) {
  std::vector<Contents> result;

  for (auto i = 0; i != 4; ++i, t = rotate(t)) {
    result.push_back(t);
    result.push_back(flip(t));
  }

  return result;
}

Contents
assemble_image(std::span<Tile const> tiles,
               std::map<std::string, std::vector<int64_t>> const &map) {
  auto const tiles_by_index = [&] {
    std::map<int64_t, Tile> result;

    for (auto const &tile : tiles)
      result.emplace(tile.number, tile);

    return result;
  }();

  std::map<aoc::Coord, Tile> layout;
  std::queue<aoc::Coord> positions_to_fill;

  auto const add_tile = [&](aoc::Coord pos, Tile const &t) {
    layout[pos] = t;

    for (auto const &[p, _] : get_surrounding(pos))
      positions_to_fill.push(p);
  };

  add_tile({0, 0}, tiles_by_index.begin()->second);

  while (!positions_to_fill.empty()) {
    auto const next_pos = positions_to_fill.front();
    positions_to_fill.pop();

    if (layout.find(next_pos) != layout.cend())
      continue;

    for (auto const &[adjacent, direction] : get_surrounding(next_pos)) {
      auto const it = layout.find(adjacent);

      if (it == layout.cend())
        continue;

      auto const edge = get_edge(it->second.contents, direction);
      auto const &all_matching_edges = map.find(edge)->second;

      for (auto const &matching : all_matching_edges) {
        if (matching == it->second.number)
          continue;

        auto const tile_to_use = tiles_by_index.find(matching)->second;
        auto const versions = get_versions(tile_to_use.contents);

        for (auto const &version : versions) {
          if (get_edge(version, opposite(direction)) != edge)
            continue;

          add_tile(next_pos, {tile_to_use.number, version});
          break;
        }

        break;
      }
    }
  }

  auto const min_x = layout.begin()->first.x;
  auto const min_y = layout.begin()->first.y;
  auto const max_y = std::prev(layout.end())->first.y + 1;

  auto const tile_dim = 8;
  auto const dim = tile_dim * (max_y - min_y);

  Contents image(dim);

  for (auto y = 0; y < dim; ++y) {
    for (auto x = 0; x < dim; ++x) {
      if (auto const tile =
              layout.find({x / tile_dim + min_x, y / tile_dim + min_y});
          tile != layout.cend())
        image[y].push_back(
            tile->second.contents[(y % tile_dim) + 1][(x % tile_dim) + 1]);
    }
  }

  return image;
}

Contents const sea_monster{"                  # ", "#    ##    ##    ###",
                           " #  #  #  #  #  #   "};

int64_t count_hashes(Contents const &c) {
  return std::accumulate(
      c.cbegin(), c.cend(), int64_t{}, [](auto acc, auto const &line) {
        return acc + std::count(line.cbegin(), line.cend(), '#');
      });
}

bool position_matches(Contents const &image, aoc::Coord const &base) {
  auto y = 0;

  for (auto &line : sea_monster) {
    auto x = 0;

    for (auto &c : line) {
      if (c == '#' && image[base.y + y][base.x + x] != '#')
        return false;

      x += 1;
    }

    y += 1;
  }

  return true;
}
} // namespace

TEST_CASE("day20") {
  auto const tiles = [&] {
    std::istringstream is{input};
    return std::vector(std::istream_iterator<Tile>{is},
                       std::istream_iterator<Tile>{});
  }();

  auto const edges = [&] {
    std::map<std::string, std::vector<int64_t>> result;

    for (auto const &tile : tiles)
      for (auto const &edge : get_edges(tile.contents))
        result[edge].push_back(tile.number);

    return result;
  }();

  auto const a = std::accumulate(tiles.cbegin(), tiles.cend(), int64_t{1},
                                 [&](auto acc, auto const &tile) {
                                   if (is_corner_tile(tile, edges))
                                     return acc * tile.number;

                                   return acc;
                                 });

  auto const b = [&] {
    auto const image = assemble_image(tiles, edges);
    auto const versions = get_versions(image);

    for (auto const &version : versions) {
      auto count = 0;

      for (auto y = 0; y < version.size() - sea_monster.size(); ++y) {
        for (auto x = 0; x < version[y].size() - sea_monster.front().size();
             ++x) {
          if (position_matches(version, {x, y}))
            count += 1;
        }
      }

      if (count != 0)
        return count_hashes(version) - count * count_hashes(sea_monster);
    }

    return int64_t{};
  }();
}
