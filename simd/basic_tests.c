//-----------------------------------------------------------------------------
//
// Source code for MIPT course on informatics
// Page with slides: http://cs.mipt.ru/wp/?page_id=7775
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simple SIMD tests
//
// Compilation line:
// > gcc -O2 -march=tigerlake basic_tests.c
//
//-----------------------------------------------------------------------------

#include <stdio.h>

#include "simd.h"

void set_and_print() {
  ri128 x;
  ri256 y;
  ri512 z;

  printf("%s\n", "set and print test");
  x = set_xvalue(4);
  DUMP_XREG(x);
  y = set_yvalue(8);
  DUMP_YREG(y);
  z = set_zvalue(16);
  DUMP_ZREG(z);  
}

void store_and_load() {
  int vals[16] __attribute__((aligned(64)));
  ri512 z0, z1, z2, z3;
  m16 mask = 0xF0F0;

  printf("%s\n", "store and load test");
  z0 = set_zvalue(16);
  z1 = set_zvalue(20);
  store_zvalue(vals, z0);  
  z2 = mask_load_zvalue(z1, vals, mask); // load and blend with z1
  DUMP_ZREG(z2);
  z3 = blend_zvalue(mask, z1, z0);
  assert(equal_zvalue(z2, z3));
  z3 = blend_zvalue(mask, z0, z1);
  DUMP_ZREG(z3);
}

void permute() {
  ri512 z0, z1, z2, z3;
  printf("%s\n", "permute test");
  z0 = set_zvalues(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
  z1 = set_zvalues(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  z2 = permute_zvalues(z0, z1); // equal to z1, as z0 is identity
  z3 = permute_zvalues(z1, z0); // equal to z1, as z1 is reverse 
  DUMP_ZREG(z2);
  assert(equal_zvalue(z1, z2));
  assert(equal_zvalue(z2, z3));
}

void rotate_shift() {
  ri512 z0, z1, z2;
  printf("%s\n", "rotate test");
  z0 = set_zvalues(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
  DUMP_ZREG(z0);
  z1 = rotate_zvalue(z0, 3);
  DUMP_ZREG(z1); // 12, 11, 10, ..., 3, 2, 1, 0, 15, 14, 13
  z2 = rotate_zvalue(z0, -4);
  DUMP_ZREG(z2);
  z1 = shift_zvalue(z0, 3);
  DUMP_ZREG(z1); // 12, 11, 10, ..., 3, 2, 1, 0, 0, 0, 0
  z2 = shift_zvalue(z0, -4);
  DUMP_ZREG(z2);
}

int main() {
  set_and_print();
  store_and_load();
  permute();
  rotate_shift();
  printf("%s\n", "done");
}