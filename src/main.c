#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define I unsigned
#define L double

I ATOM=0x7ff8, PRIM=0x7ff9, CONS=0x7ffa, CLOS=0x7ffb, NIL=0x7ffc;

#define T(x) *(uint64_t*)&x>>48

L nil,tru,err;

#define A (char*)cell
#define N 1024
L cell[N];
I hp = 0, sp = N;

void abort() {exit(1);}

L box(I t, I i) {
  L x;
  *(uint64_t*)&x = (uint64_t)t<<48|i;
  return x;
}

I ord(L x) {
  return *(uint64_t*)&x;
}

L num(L n) {
  return n;
}

I equ(L x, L y) {
  return *(uint64_t*)&x == *(uint64_t*)&y;
}

I not(L x) {
  return T(x) == NIL;
}

L cons(L x, L y) {
  cell[--sp] = x;
  cell[--sp] = y;
  
  if (hp > sp<<3) abort();
  
  return box(CONS, sp);
}

L car(L p) {
  return (T(p) & ~(CONS^CLOS)) == CONS ? cell[ord(p) + 1] : err;
}

L cdr(L p) {
  return (T(p) & ~(CONS^CLOS)) == CONS ? cell[ord(p)] : err;
}

L atom(const char *s) {
  I i = 0;
  
  while (i < hp && strcmp(A+i,s)) i += strlen(A+i)+1;
  
  if (i == hp && (hp += strlen(strcpy(A+i, s)) + 1 ) > sp<<3) abort();
  
  return box(ATOM, i);
}

L pair(L v, L x, L e) {
  return cons(cons(v, x), e);
}

L closure(L v, L x, L e) {
  return box(CLOS, ord(pair(v, x, equ(e, env) ? nil : e)));
}

L assoc(L v, L e) {
  while (T(e) == CONS && !equ(v, car(car(e)))) 
    e = cdr(e);
  
  return T(e) == CONS ? cdr(car(e)) : err;
}

int main() {
  nil = box(NIL, 0);
  tru = atom("#t");
  err = atom("ERR");
}