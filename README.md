# project-euler-451

We solve problem [451](https://projecteuler.net/problem=451) from Project Euler here.

> Consider the number 15.
> There are eight positive numbers less than 15 which are coprime to 15: 1, 2, 4, 7, 8, 11, 13, 14.
> The modular inverses of these numbers modulo 15 are: 1, 8, 4, 13, 2, 11, 7, 14
> because
> 1 · 1 mod 15=1
> 2 · 8=16 mod 15=1
> 4 · 4=16 mod 15=1
> 7 · 13=91 mod 15=1
> 11 · 11=121 mod 15=1
> 14 · 14=196 mod 15=1
> 
> Let I(n) be the largest positive number m smaller than n-1 such that the modular inverse of m modulo n equals m itself.
> So I(15)=11.
> Also I(100)=51 and I(7)=1.
> 
> Find ∑ I(n) for 3≤n≤2×10^7

## Carmichael function and Euler's totient function

When the [Carmichael function](https://en.wikipedia.org/wiki/Carmichael_function) and the Euler's totient function agree, the modular inverse is either 1 or n-1. We build tables for these functions to save time on some cases. Though in hindsight this might not be necessary.

## Modular inverses for prime powers

For a prime power p^k, only a few values x satisfy x^2 = 1 mod p^k. When p is 2, there are four of them, namely 1, p^k/2-1, p^k/2+1, and p^k-1, though one should consider the case when p&k = 2 or 4 separately. For other prime powers, there are exactly two, namely 1 and p^k-1. This is explained in the wiki article of [Multiplicative group of integers modulo n](https://en.wikipedia.org/wiki/Multiplicative_group_of_integers_modulo_n#Structure).

## Chinese remainder theorem

The chinese remainder theorem says that if n = p^k * q^h, where p and q are primes, then there is a one-to-one correspondence between integers in Z/nZ and pairs (a, b) in Z/p^kZ x Z/q^hZ. The correspondence can be computed using the extended euclidean algorithm. 

## Our ideas

For each number n, we gather a list of vectors of the modular inverses of the prime power factors of n. Then we `merge' the vectors using the chinese remainder theorem to get a vector of modular inverses of n. At the end, we find the biggest entry (that is bounded by n-1) of such vector. This number is the l(n) desired. 
