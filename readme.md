# strap-v2

Welcome to this, yet again, crazy project. This will be my very last project in the osdev area, thus it will be maintained over time and not abandoned like many others.

## Why C?

Based on my experience I have made the decision to write the kernel in C.

C++ and many alternatives offer many abstractions to the actual code, which do help in some ways, but distract in many others. An example are over-loaded operators, which do help to better generalize the code, but might get messy when trying to step things up.

C also follows a very pragmatic and functional syntax, which in the case of kernel development helps a lot.

## Goal

strap-v2 has the simple goal to base its roots into interpreted scripts. The general idea is having a very simple microkernel with ustar support, in order to load from a simple boot drive many interpreted modules that would extend kernel functionalities.

Functionalities I wish would be written as interpreted extensions:
  - TTY system
  - Keyboard driver
  - GFX driver
  - Window manager
  - Standard library (dynamic arrays, lists, dictionaries, ...)

To interpret any kind of language, though, there are some necessary things:
  - Simple memory manager
  - Output system
  - GDT and IDT descriptors
  - SMP (maybe?), otherwise any yield or pit based process switching
  - Authority check for scripts (kernel-level, driver-level or user-level)

I will try and keep the language parser and runtime as simple as possible such that I won't need anything like standard library lists in C.

### Memory Manager

As the interpreted language will only access managed data and structured objects it would be a great idea to base the memory manager off this concept, maybe completely ignoring the memory manager for the C side of the kernel, which must remain a simple foundation able to efficiently run scripts.

### Parser and runtime

The parser and runtime will be managed by the <a href="https://github.com/TheOSDevOrg/Tabos.VM">Tabos.VM</a> project.