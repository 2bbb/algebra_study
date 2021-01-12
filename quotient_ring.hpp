#pragma once

#include "./algebraic_structure/euclidean_ring.hpp"

#include <type_traits>
#include <cstddef>
#include <cstdint>

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace bbb {
	namespace math {
		template <typename g>
		struct quotient_ring {
			using value_type = decltype(g::value());
			
			value_type inverse() const {
				if(v == value_type{}) throw std::overflow_error("divide by zero exception");
				auto &&br = euclidean_ring::bezout(v, g::value());
				//			if(br.d != 1) {
				//				std::ostringstream ss{""};
				//				ss << g::value() << " is not prime: " << v << ", " << br.d << ", " << br.x;
				//				throw std::logic_error(ss.str().c_str());
				//			}
				return br.x / br.d;
			}
			
			template <typename ... Xs>
			quotient_ring(Xs && ... vs)
			: v{std::forward<Xs>(vs) ...}
			{};
			
			quotient_ring operator+(const quotient_ring &rhs) const
			{ return {(v + rhs.v) % g::value()}; };
			template <typename lhs_type>
			friend quotient_ring operator+(const lhs_type &lhs, const quotient_ring &rhs)
			{ return {(lhs + rhs.v) % g::value()}; };
			
			quotient_ring operator-(const quotient_ring &rhs) const
			{ return {(v - rhs.v) % g::value()}; };
			template <typename lhs_type>
			friend quotient_ring operator-(const lhs_type &lhs, const quotient_ring &rhs)
			{ return {(lhs - rhs.v) % g::value()}; };
			
			quotient_ring operator*(const quotient_ring &rhs) const
			{ return {(v * rhs.v) % g::value()}; };
			template <typename lhs_type>
			friend quotient_ring operator*(const lhs_type &lhs, const quotient_ring &rhs)
			{ return {(lhs * rhs.v) % g::value()}; };
			
			//		quotient_ring operator/(const quotient_ring &rhs) const
			//		{ return {(v / rhs.v) % g::value()}; };
			//		template <typename lhs_type>
			//		friend quotient_ring operator/(const lhs_type &lhs, const quotient_ring &rhs)
			//		{ return {(lhs / rhs.v) % g::value()}; };
			
			quotient_ring operator/(const quotient_ring &rhs) const
			{ return {(v * rhs.inverse()) % g::value()}; };
			template <typename lhs_type>
			friend quotient_ring operator/(const lhs_type &lhs, const quotient_ring &rhs)
			{ return {(lhs * rhs.inverse()) % g::value()}; };
			
			bool operator==(const quotient_ring &rhs) const
			{ return (v - rhs.v) % g::value() == value_type{}; };
			template <typename lhs_type>
			friend bool operator==(const lhs_type &lhs, const quotient_ring &rhs)
			{ return {(lhs - rhs.v) % g::value()}; };
			
			value_type operator()() const
			{ return v; };
		private:
			value_type v;
		};
		template <typename g>
		std::ostream &operator<<(std::ostream &os, const quotient_ring<g> &v)
		{ return os << v() << " (mod " << g::value() << ")"; };
	};
};