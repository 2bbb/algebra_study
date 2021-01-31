#pragma once

namespace bbb {
	namespace math {
        template <typename base_type_>
        struct multiplicative_magma {
            using base_type = base_type_;
            static base_type op(const multiplicative_magma &x,
                                const multiplicative_magma &y)
            { return x.v * y.v; };

            additive_magma operator*(const additive_magma &rhs) const
            { return {op(*this, rhs)}; };
            additive_magma &operator*=(const addtive_magma &rhs)
            { return v = op(*this, rhs), *this; };

            base_type &operator()()
            { return v; };
            const base_type &operator()() const
            { return v; };
        private:
            base_type v;
        };
	};
};
