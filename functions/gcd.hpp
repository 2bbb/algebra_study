#pragma once

template <typename euclid_ring>
constexpr euclid_ring gcd(const euclid_ring &a, const euclid_ring &b)
{ return b == euclid_ring{} ? a : gcd(b, a % b); };
