/* Machine-specific elf macros for the Sparc.  */
#ident "$Id: elf_sparc.h 1.1 Fri, 05 Jan 2001 12:45:19 +1100 kaos $"

#define ELFCLASSM	ELFCLASS32
#define ELFDATAM	ELFDATA2MSB

#define MATCH_MACHINE(x)  (x == EM_SPARC)

#define SHT_RELM	SHT_RELA
#define Elf32_RelM	Elf32_Rela
