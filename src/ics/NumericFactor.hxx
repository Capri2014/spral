#pragma once

#include "AlignedCallocBlock.hxx"
#include "SymbolicFactor.hxx"

namespace spral {
namespace ics {

class NumericFactor {
   typedef double T;
public:
   /** Perform a factorization as part of construction */
   NumericFactor(const SymbolicFactor &sf, const T aval[]);

   /** Perform a solve with factors */
   void solve(int nrhs, T x[]);

private:
   const SymbolicFactor &sf_;
   AlignedCallocBlock<T> lmem;
};

} /* namespace ics */
} /* namespace spral */
