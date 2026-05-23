// registers.h
#pragma once

#include <unordered_set>
#include <string>

    static const std::unordered_set<std::string> regs = {
        // ==================== x86-64 ====================
        // General-purpose (64-bit, 32-bit, 16-bit, 8-bit)
        "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RSP", "RBP",
        "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15",
        "EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "ESP", "EBP",
        "R8D", "R9D", "R10D", "R11D", "R12D", "R13D", "R14D", "R15D",
        "AX", "BX", "CX", "DX", "SI", "DI", "SP", "BP",
        "R8W", "R9W", "R10W", "R11W", "R12W", "R13W", "R14W", "R15W",
        "AL", "BL", "CL", "DL", "SIL", "DIL", "SPL", "BPL",
        "R8B", "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B",
        "AH", "BH", "CH", "DH",

        // Segment registers
        "CS", "DS", "SS", "ES", "FS", "GS",

        // Instruction pointer & flags
        "RIP", "EIP", "IP",
        "RFLAGS", "EFLAGS", "FLAGS",

        // x87 FPU
        "ST0", "ST1", "ST2", "ST3", "ST4", "ST5", "ST6", "ST7",

        // MMX
        "MM0", "MM1", "MM2", "MM3", "MM4", "MM5", "MM6", "MM7",

        // SSE / AVX
        "XMM0", "XMM1", "XMM2", "XMM3", "XMM4", "XMM5", "XMM6", "XMM7",
        "XMM8", "XMM9", "XMM10", "XMM11", "XMM12", "XMM13", "XMM14", "XMM15",
        "YMM0", "YMM1", "YMM2", "YMM3", "YMM4", "YMM5", "YMM6", "YMM7",
        "YMM8", "YMM9", "YMM10", "YMM11", "YMM12", "YMM13", "YMM14", "YMM15",
        "ZMM0", "ZMM1", "ZMM2", "ZMM3", "ZMM4", "ZMM5", "ZMM6", "ZMM7",
        "ZMM8", "ZMM9", "ZMM10", "ZMM11", "ZMM12", "ZMM13", "ZMM14", "ZMM15",
        "ZMM16", "ZMM17", "ZMM18", "ZMM19", "ZMM20", "ZMM21", "ZMM22", "ZMM23",
        "ZMM24", "ZMM25", "ZMM26", "ZMM27", "ZMM28", "ZMM29", "ZMM30", "ZMM31",
        "K0", "K1", "K2", "K3", "K4", "K5", "K6", "K7",
        "MXCSR",

        // Control registers
        "CR0", "CR1", "CR2", "CR3", "CR4", "CR5", "CR6", "CR7",
        "CR8", "CR9", "CR10", "CR11", "CR12", "CR13", "CR14", "CR15",

        // Debug registers
        "DR0", "DR1", "DR2", "DR3", "DR4", "DR5", "DR6", "DR7",

        // System address registers
        "GDTR", "LDTR", "IDTR", "TR",

        // Common Model-Specific Registers (MSRs)
        "IA32_EFER", "IA32_STAR", "IA32_LSTAR", "IA32_CSTAR", "IA32_FS_BASE",
        "IA32_GS_BASE", "IA32_SYSENTER_CS", "IA32_SYSENTER_ESP", "IA32_SYSENTER_EIP",
        "IA32_TSC", "IA32_APIC_BASE", "IA32_PAT", "IA32_MTRR_PHYSBASE0", "IA32_MTRR_PHYSMASK0",
        "IA32_PERF_GLOBAL_CTRL", "IA32_MCG_CAP", "IA32_RTIT_CTL",

        // Virtualization
        "VMCS", "VMCB",  // data structures, not true registers but often referenced

        // ==================== ARM64 (AArch64) ====================
        // General-purpose (64-bit and 32-bit)
        "X0", "X1", "X2", "X3", "X4", "X5", "X6", "X7",
        "X8", "X9", "X10", "X11", "X12", "X13", "X14", "X15",
        "X16", "X17", "X18", "X19", "X20", "X21", "X22", "X23",
        "X24", "X25", "X26", "X27", "X28", "X29", "X30",
        "W0", "W1", "W2", "W3", "W4", "W5", "W6", "W7",
        "W8", "W9", "W10", "W11", "W12", "W13", "W14", "W15",
        "W16", "W17", "W18", "W19", "W20", "W21", "W22", "W23",
        "W24", "W25", "W26", "W27", "W28", "W29", "W30",
        "XZR", "WZR",

        // Special-purpose
        "PC", "SP_EL0", "SP_EL1", "SP_EL2", "SP_EL3", "LR",

        // SIMD & FPU (V, Q, D, S, H, B banks)
        "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7",
        "V8", "V9", "V10", "V11", "V12", "V13", "V14", "V15",
        "V16", "V17", "V18", "V19", "V20", "V21", "V22", "V23",
        "V24", "V25", "V26", "V27", "V28", "V29", "V30", "V31",
        "Q0", "Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7",
        "Q8", "Q9", "Q10", "Q11", "Q12", "Q13", "Q14", "Q15",
        "Q16", "Q17", "Q18", "Q19", "Q20", "Q21", "Q22", "Q23",
        "Q24", "Q25", "Q26", "Q27", "Q28", "Q29", "Q30", "Q31",
        "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
        "D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15",
        "D16", "D17", "D18", "D19", "D20", "D21", "D22", "D23",
        "D24", "D25", "D26", "D27", "D28", "D29", "D30", "D31",
        "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7",
        "S8", "S9", "S10", "S11", "S12", "S13", "S14", "S15",
        "S16", "S17", "S18", "S19", "S20", "S21", "S22", "S23",
        "S24", "S25", "S26", "S27", "S28", "S29", "S30", "S31",
        "H0", "H1", "H2", "H3", "H4", "H5", "H6", "H7",
        "H8", "H9", "H10", "H11", "H12", "H13", "H14", "H15",
        "H16", "H17", "H18", "H19", "H20", "H21", "H22", "H23",
        "H24", "H25", "H26", "H27", "H28", "H29", "H30", "H31",
        "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7",
        "B8", "B9", "B10", "B11", "B12", "B13", "B14", "B15",
        "B16", "B17", "B18", "B19", "B20", "B21", "B22", "B23",
        "B24", "B25", "B26", "B27", "B28", "B29", "B30", "B31",

        // Status & control
        "PSTATE", "FPCR", "FPSR",

        // System Control Registers (EL1/EL2/EL3 examples)
        "SCTLR_EL1", "SCTLR_EL2", "SCTLR_EL3",
        "TTBR0_EL1", "TTBR1_EL1", "TTBR0_EL2", "TTBR1_EL2",
        "TCR_EL1", "TCR_EL2", "TCR_EL3",
        "MAIR_EL1", "MAIR_EL2", "MAIR_EL3",
        "VBAR_EL1", "VBAR_EL2", "VBAR_EL3",
        "ELR_EL1", "ELR_EL2", "ELR_EL3",
        "ESR_EL1", "ESR_EL2", "ESR_EL3",
        "FAR_EL1", "FAR_EL2", "FAR_EL3",
        "HCR_EL2", "VTCR_EL2", "VTTBR_EL2",

        // Debug registers
        "DBGBCR0_EL1", "DBGBCR1_EL1", "DBGWCR0_EL1", "DBGWCR1_EL1",

        // Performance monitors
        "PMCR_EL0", "PMCCNTR_EL0", "PMEVTYPER0_EL0", "PMEVCNTR0_EL0",

        // ==================== RISC-V (RV64GC) ====================
        // General-purpose (x0..x31) + ABI names
        "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
        "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
        "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23",
        "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31",
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
        "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",

        // Floating-point (F and D extensions)
        "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
        "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
        "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",
        "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
        "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",
        "fs0", "fs1", "fa0", "fa1", "fa2", "fa3", "fa4", "fa5",
        "fa6", "fa7", "fs2", "fs3", "fs4", "fs5", "fs6", "fs7",
        "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11",

        // Vector registers (V extension)
        "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
        "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
        "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
        "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31",

        // RISC-V CSRs (by privilege level)
        // Machine level
        "mstatus", "misa", "mtvec", "mscratch", "mepc", "mcause", "mtval",
        "mideleg", "medeleg", "mcounteren", "mcountinhibit", "mhartid",
        // Supervisor level
        "sstatus", "stvec", "sscratch", "sepc", "scause", "stval", "satp",
        // User level / floating-point
        "fflags", "frm", "fcsr",
        // Performance counters
        "cycle", "time", "instret", "hpmcounter3", "hpmcounter4",
        // Hypervisor extension (H)
        "hstatus", "hedeleg", "hideleg", "hgeip", "htimedelta", "hgatp"
    };
    enum class TokenLists{
        TOKEN_TYPE,
        TOKEN_IDENTIFIER,
        TOKEN_ASSIGN,
        TOKEN_SCOPE,
        TOKEN_RUNTIME,
        TOKEN_INDEXES
    };