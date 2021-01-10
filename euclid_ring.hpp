#pragma once

#include <stdexcept>
#include <utility>
#include <vector>

namespace math {
	namespace euclid_ring {
		template <typename euclid_ring>
		struct bezout_result {
			euclid_ring x;
			euclid_ring y;
			euclid_ring d;
		};
		
		template <typename euclid_ring, typename er1, typename er2>
		euclid_ring extended_gcd(er1 &&a,
			er2 &&b,
			std::vector<euclid_ring> &qs)
		{
			if(b == euclid_ring{}) return std::forward<er1>(a);
			auto &&q = a / b;
			auto &&r = a % b;
			qs.push_back(std::move(q));
			return extended_gcd(std::forward<er2>(b), std::move(r), qs);
		}
		
		template <typename euclid_ring>
		bezout_result<euclid_ring> bezout(const euclid_ring &a, const euclid_ring &b) {
			std::vector<euclid_ring> qs;
			auto &&d = extended_gcd<euclid_ring>(a, b, qs);
			//		std::reverse(qs.begin(), qs.end());
			std::vector<euclid_ring> mat, tmp;
			mat.resize(4);
			mat[0] = 1;
			mat[3] = 1;
			tmp.resize(4);
			for(auto &q : qs) {
				tmp[0] = 0 * mat[0] + mat[2];
				tmp[1] = 0 * mat[1] + mat[3];
				tmp[2] = 1 * mat[0] + -q * mat[2];
				tmp[3] = 1 * mat[1] + -q * mat[3];
				std::swap(tmp, mat);
			}
			return { std::move(mat[0]), std::move(mat[1]), std::move(d) };
		}
		
		template <typename euclid_ring>
		euclid_ring inverse_on_mod(const euclid_ring &a, const euclid_ring &mod) {
			if(a == euclid_ring{}) throw std::overflow_error("divide by zero exception");
			auto &&br = bezout(a, mod);
			if(br.d != 1) throw std::logic_error("mod is not prime");
			return br.x;
		}
	}; // euclid_ring
}; // math
