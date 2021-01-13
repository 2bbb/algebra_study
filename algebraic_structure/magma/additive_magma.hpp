#pragma once

namespace bbb {
	namespace math {
        template <typename base_type_>
        struct additive_magma {
            using base_type = base_type_;
            static base_type op(const additive_magma &x,
                                const additive_magma &y)
            { return x.v + y.v; };

            additive_magma operator+(const addtive_magma &rhs) const
            { return {op(*this, rhs)}; };
            additive_magma operator+=(const addtive_magma &rhs) const
            { return v = op(*this, rhs), *this; };

            base_type &operator()()
            { return v; };
            const base_type &operator()() const
            { return v; };
            operator base_type()
            { return v; };
        private:
            base_type v;
        };
	};
};
