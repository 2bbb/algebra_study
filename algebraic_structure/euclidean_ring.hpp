#pragma once

#include "./ring.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

namespace bbb {
	namespace math {
		namespace euclidean_ring {
			// using namespace ring;
			
			// euclidean_ring R requires:
			// \forall a, b \in R, b != 0 -> \exists q, r \in R s.t. a = b * q + r /\ d(r) < d(b)
			// \forall a, b \in R, a != 0, b != 0 -> d(a) <= d(a * b)
			// 
			
			template <typename euclidean_ring>
			euclidean_ring modulo(const euclidean_ring &x, const euclidean_ring &y)
			{ return x % y; };
	
			template <typename euclidean_ring>
			struct bezout_result {
				euclidean_ring x;
				euclidean_ring y;
				euclidean_ring d;
			};
			
			template <typename euclidean_ring, typename er1, typename er2>
			euclidean_ring extended_gcd(er1 &&a,
				er2 &&b,
				std::vector<euclidean_ring> &qs)
			{
				if(b == euclidean_ring{}) return std::forward<er1>(a);
				auto &&q = a / b;
				auto &&r = a % b;
				qs.push_back(std::move(q));
				return extended_gcd(std::forward<er2>(b), std::move(r), qs);
			}
			
			template <typename euclidean_ring>
			bezout_result<euclidean_ring> bezout(const euclidean_ring &a, const euclidean_ring &b) {
				std::vector<euclidean_ring> qs;
				auto &&d = extended_gcd<euclidean_ring>(a, b, qs);
				//		std::reverse(qs.begin(), qs.end());
				std::vector<euclidean_ring> mat, tmp;
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
			
			template <typename euclidean_ring>
			euclidean_ring inverse_on_mod(const euclidean_ring &a, const euclidean_ring &mod) {
				if(a == euclidean_ring{}) throw std::overflow_error("divide by zero exception");
				auto &&br = bezout(a, mod);
				if(br.d != 1) throw std::logic_error("mod is not prime");
				return br.x;
			}
		}; // euclidean_ring
	}; // math
};
