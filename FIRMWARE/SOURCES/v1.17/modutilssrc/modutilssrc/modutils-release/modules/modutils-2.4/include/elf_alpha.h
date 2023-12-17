/* Machine-specific elf macros for the Alpha.  */
#ident "$Id: elf_alpha.h 1.1 Fri, 05 Jan 2001 12:45:19 +1100 kaos $"

#define ELFCLASSM	ELFCLASS64
#define ELFDATAM	ELFDATA2LSB

#define MATCH_MACHINE(x)  (x == EM_ALPHA)

#define SHT_RELM	SHT_RELA
#define Elf64_RelM	Elf64_Rela
