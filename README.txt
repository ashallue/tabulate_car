Andrew Shallue
coauthor: Jonathan Webster
late 2021, early 2022

Project to tabulate Carmichael numbers.
New algorithm:  "D/Delta method" which tabulates all Carmichael numbers of the form Pqr where P is a pre-product, and q, r are primes generated through divisors of (P-1)(P+D).  
This is an idea that has been discussed before (e.g. Pinch), our innovation is to push it as far as it can go.

Analysis suggests it should give improvements overall, but it is especially interesting that one can generate very large Carmichael numbers quickly.


// Things to do:

* If D is even, a divisor Delta of (P-1)(P+D) may not have 2 as a prime factor.
Generalization: Let p be a prime divisor of D.  For the construction to succeed, it is necessary that (P^2 + Delta)/D be an integer.  Since p divides the bottom, it must divide the top.

case a) if p divides P, we must have p | Delta.  So we can discard all divisors of (P-1)(P+D) where the exponent on p is 0.
case b) if p does not divid P, then p not | Delta.  So we can discard all divisors of (P-1)(P+D) where the exponent on p is > 0.

* Parallelization - DONE
From my notes: every processor has a factgen2 window tracking factors of P, P-1.Counts the number of admissable P seen.  When count is in the residue class corresponding to that particular processor, start a new segmented sieve.  By semented sieve, I think it just means running over divisors to find Carmichael numbers with that particular pre-product.

* What should I do about proving primality of r?
We have r - 1 = (P-1)(P+C)/Delta.  I could put the factors of P-1 into the text file.  Then apply trial division factorization on the co-factor.
With factors of r-1 in hand, I could do Pocklington.
Update: No, just cast into gmp multi-precision integer, invoke gmp implementation of elliptic curve primality proving.
https://sourceforge.net/projects/gmp-ecpp
or could try pari
https://pari.math.u-bordeaux.fr

* Implement Pinch
Summary.  First generate pre-products.  Though Pinch generates these through back-tracking search, to keep timing comparisions "fair" I will generate pre-products with the same incremental sieve as for the D-Delta algorithm.
"If P is mall enough, then proceed by using Proposition 2"
"If the value of P at level d-2 is large, then we loop overall values of p_{d-1} permitted by Proposition 1(1) and Proposition 3(1). The innermost loop runs over all primes p with Pp = 1 mod L for which p-1 | P-1 and satisfies bounds."

* Crossover - dynamic choice between CD and D-Delta
For a given D, how many C values vs how many divisors?
If fewer C, do Pinch's method.  If fewer divisors, do new method.

* I need to update preproduct_construction so that instead of passing in prime factors of P-1, P+D, I pass in the merged set of unique prime factors.
