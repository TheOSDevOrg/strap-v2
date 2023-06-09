#include <tests.h>
#include <stdio.h>
#include <TabosVM.h>

const char validation_test[] = 
    "TVM"
    "test.module\0" "1.0.0\0"
    
    "VAR"
    "varname\0" "number\0"
    "test\0"    "string|null\0"
    "\xff"
    
    "EXE"
    "\xde"
    "\x00\x00\x00\x00" "main\0" "void\0"
    "\x09\x00\x00\x00" "test\0" "void\0"

    
    "BYC"
//   callms     test
    "\x01\x00" "test\0"
//   debug
    "\x05\x00"
//   debug
    "\x05\x00"
//   end
    "\xff\xff"
    
    "EOE"
    "EOF";

void __STRAPV2_TEST_VM_flow() {
  TVM_module_t mod = TVM_modload(validation_test);
  TVM_code_t code = TVM_read(mod);

  TVM_engine_processor_t proc = TVM_build(mod, code);

  TVM_exec(&proc, false);
}