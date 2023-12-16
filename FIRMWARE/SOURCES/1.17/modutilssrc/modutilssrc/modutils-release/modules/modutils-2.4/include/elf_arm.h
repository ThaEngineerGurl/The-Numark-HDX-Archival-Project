/* Machine-specific elf macros for ARM.  */
#ident "$Id: elf_arm.h 1.1 Fri, 05 Jan 2001 12:45:19 +1100 kaos $"

#define ELFCLASSM	ELFCLASS32
#define ELFDATAM	ELFDATA2LSB

#define MATCH_MACHINE(x)  (x == EM_ARM)

#define SHT_RELM	SHT_REL
#define Elf32_RelM	Elf32_Rel
