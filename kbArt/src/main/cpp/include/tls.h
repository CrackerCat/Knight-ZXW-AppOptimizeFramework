#pragma once


#if defined(__aarch64__)
# define __get_tls() ({ void** __val; __asm__("mrs %0, tpidr_el0" : "=r"(__val)); __val; })
#elif defined(__arm__)
# define __get_tls() ({ void** __val; __asm__("mrc p15, 0, %0, c13, c0, 3" : "=r"(__val)); __val; })
#elif defined(__i386__)
# define __get_tls() ({ void** __val; __asm__("movl %%gs:0, %0" : "=r"(__val)); __val; })
#elif defined(__riscv)
# define __get_tls() ({ void** __val; __asm__("mv %0, tp" : "=r"(__val)); __val; })
#elif defined(__x86_64__)
# define __get_tls() ({ void** __val; __asm__("mov %%fs:0, %0" : "=r"(__val)); __val; })
#else
#error unsupported architecture
#endif

#if defined(__arm__) || defined(__aarch64__)
// The ARM ELF TLS ABI specifies[1] that the thread pointer points at a 2-word
// TCB followed by the executable's TLS segment. Both the TCB and the
// executable's segment are aligned according to the segment, so Bionic requires
// a minimum segment alignment, which effectively reserves an 8-word TCB. The
// ARM spec allocates the first TCB word to the DTV.
//
// [1] "Addenda to, and Errata in, the ABI for the ARM Architecture". Section 3.
// http://infocenter.arm.com/help/topic/com.arm.doc.ihi0045e/IHI0045E_ABI_addenda.pdf

#define MIN_TLS_SLOT (-2)  // update this value when reserving a slot
#define TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE (-2)
#define TLS_SLOT_BIONIC_TLS     (-1)
#define TLS_SLOT_DTV              0
#define TLS_SLOT_THREAD_ID        1
#define TLS_SLOT_APP              2 // was historically used for errno
#define TLS_SLOT_OPENGL           3
#define TLS_SLOT_OPENGL_API       4
#define TLS_SLOT_STACK_GUARD      5
#define TLS_SLOT_SANITIZER        6 // was historically used for dlerror
#define TLS_SLOT_ART_THREAD_SELF  7

// The maximum slot is fixed by the minimum TLS alignment in Bionic executables.
#define MAX_TLS_SLOT              7
#elif defined(__i386__) || defined(__x86_64__)

// x86 uses variant 2 ELF TLS layout, which places the executable's TLS segment
// immediately before the thread pointer. New slots are allocated at positive
// offsets from the thread pointer.

#define MIN_TLS_SLOT              0

#define TLS_SLOT_SELF             0
#define TLS_SLOT_THREAD_ID        1
#define TLS_SLOT_APP              2 // was historically used for errno
#define TLS_SLOT_OPENGL           3
#define TLS_SLOT_OPENGL_API       4
#define TLS_SLOT_STACK_GUARD      5
#define TLS_SLOT_SANITIZER        6 // was historically used for dlerror
#define TLS_SLOT_ART_THREAD_SELF  7
#define TLS_SLOT_DTV              8
#define TLS_SLOT_BIONIC_TLS       9
#define TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE 10
#define MAX_TLS_SLOT 10  // update this value when reserving a slot
#elif defined(__riscv)
// RISC-V ELF Specification[1] specifies that RISC-V uses Variant I as described
// by the ELF TLS specification, with tp containing the address one past the end
// of the TCB.
//
// [1]: RISC-V ELF Specification. Section: Thread Local Storage
// https://github.com/riscv-non-isa/riscv-elf-psabi-doc/blob/master/riscv-elf.adoc#thread-local-storage

#define MIN_TLS_SLOT (-10)  // update this value when reserving a slot

#define TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE (-10)
#define TLS_SLOT_BIONIC_TLS      (-9)
#define TLS_SLOT_DTV             (-8)
#define TLS_SLOT_THREAD_ID       (-7)
#define TLS_SLOT_APP             (-6)
#define TLS_SLOT_OPENGL          (-5)
#define TLS_SLOT_OPENGL_API      (-4)
#define TLS_SLOT_STACK_GUARD     (-3)
#define TLS_SLOT_SANITIZER       (-2)
#define TLS_SLOT_ART_THREAD_SELF (-1)
#define MAX_TLS_SLOT             (-1)
#endif
#define BIONIC_TLS_SLOTS (MAX_TLS_SLOT - MIN_TLS_SLOT + 1)
