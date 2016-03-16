/* OmegaWalk: walks the full, infinite, omega-ary tree
 * Copyright Ben Wiederhake 2016
 *
 * This is free and unencumbered software released into the public domain.
 * For more information, please refer to the file 'LICENSE' file,
 * or visit <http://unlicense.org>
 *
 * Language: C++
 * Basic idea: triangles, but non-caching
 * How to run: From a shell, do:
 *   g++ -std=c++11 -Ofast -DNDEBUG -fomit-frame-pointer \
 *       -march=native -flto -fwhole-program -o walk_cpp walk.cpp
 *   ./walk_cpp
 * Hints:
 *   For debugging, use:
 *     g++ -std=c++11 -O2 -Wall -Wextra -pedantic -g -o walk_cpp walk.cpp
 *   To get timing infos, use:
 *     time ./walk_cpp > /dev/null
 */

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>

typedef unsigned int uint;

// Non-reusable; hidden assumptions! See comments.
// TODO: Try inline?
static uint detriang_uint(const uint in, uint* const head) {
    typedef std::vector<uint> triang_t;
    // Can't create the zero ad 1 on its own!
    static triang_t triangles = {0, 1};
    // static triang_t triangles = {0, 1, 3, 6, 10, 15, 21, 28, 36};

    // Find the triangle number that is *greater* than 'in'.
    // Then we know that the number before it must
    triang_t::const_iterator t_it =
        std::upper_bound(triangles.cbegin(), triangles.cend(), in);
    if (t_it == triangles.cend()) {
        // No such triangle number known, yet.
        // Hope that the next one is bigger than 'in' (= hidden assumption)
        // (Exploit a property of the calling algorithm.)
        assert(in == triangles.back() && "Missing entry -- skipped call?!");
        const uint head_p1 = triangles.size();
        triangles.emplace_back((head_p1 * (head_p1 + 1)) / 2);
        *head = head_p1 - 1;
        return 0;
    } else {
        assert(*t_it > in && "Not beyond 'in'?!");
        assert(triangles.begin() != t_it && "No previous element?!");
        --t_it;
        assert(*t_it <= in && "Wasn't exactly one beyond 'in'?!");
        *head = t_it - triangles.begin();
        return in - *t_it;
    }
}

int main() {
    // 1120 msecs on my machine
    // That's roughly 1780 per msec

    for (uint i = 0; i < 2000000; ++i) {
        printf("(");
        // prints the head-th sequence,
        // exploiting that the 0th sequence is empty.
        uint head = i;
        while (head != 0) {
            const uint rest = detriang_uint(head - 1, &head);
            printf("%d ", rest);
            assert(head >= rest);
            head -= rest;
        }
        printf(")\n");
    }
    return 0;
}
