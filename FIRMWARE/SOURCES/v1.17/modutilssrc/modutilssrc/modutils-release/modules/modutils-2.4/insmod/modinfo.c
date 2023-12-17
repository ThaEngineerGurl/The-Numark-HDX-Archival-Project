/* Display information gleaned from a module's .modinfo section.
   Copyright 1996, 1997 Linux International.

   Contributed by Tom Dyas <tdyas@eden.rutgers.edu>

   This file is part of the Linux modutils.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
  */

/*
 * Fixes:
 *
 * optind++ in show_module_info, Salvador Ortiz Garcia <sog@msg.com.mx>
 * unified module path handling: Bjorn Ekwall <bj0rn@blox.se> February 1999
 * Rationalize common code for 32/64 bit architectures.
 *   Keith Owens <kaos@ocs.com.au> December 1999.
 * Add arch64().
 *   Keith Owens <kaos@ocs.com.au> December 1999.
 * Print " persistent" for persistent parameters.
 *   Keith Owens <kaos@ocs.com.au> November 2000.
 */

#ident "$Id: modinfo.c 1.6 Wed, 28 Mar 2001 20:49:06 +1000 kaos $"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "module.h"
#include "obj.h"
#include "util.h"
#include "version.h"
#include "config.h"

extern int show_module_info(const char *filename, const char *fmtstr, int do_parameters);
extern int show_module_info_32(const char *filename, const char *fmtstr, int do_parameters);
extern int show_module_info_64(const char *filename, const char *fmtstr, int do_parameters);

#if defined(COMMON_3264) && defined(ONLY_32)
#define SHOW_MODULE_INFO show_module_info_32		/* 32 bit version */
#elif defined(COMMON_3264) && defined(ONLY_64)
#define SHOW_MODULE_INFO show_module_info_64		/* 64 bit version */
#else
#define SHOW_MODULE_INFO show_module_info		/* Not common 3264 code */
#endif


static int convert_escape (int c)
{
  switch (c)
    {
    case 'a':  /* WARNING: the meaning of \a is changed is -traditional is used */
      c = '\a';
      break;
    case 'v':
      c = '\v';
      break;
    case 'b':
      c = '\b';
      break;
    case 'e': /* ESC -- non-ANSI */
    case 'E':
      c = '\033';
      break;
    case 'f':
      c = '\f';
      break;
    case 'n':
      c = '\n';
      break;
    case 'r':
      c = '\r';
      break;
    case 't':
      c = '\t';
      break;
    }

  return(c);
}


static const char *get_modinfo_value(struct obj_file *f, const char *key)
{
	struct obj_section *sec;
	char *p, *v, *n, *ep;
	size_t klen = strlen(key);

	if (strcmp(key, "filename") == 0) {
		return(f->filename);
	}

	sec = obj_find_section(f, ".modinfo");
	if (sec == NULL)
		return NULL;

	p = sec->contents;
	ep = p + sec->header.sh_size;
	while (p < ep) {
		v = strchr(p, '=');
		n = strchr(p, '\0');
		if (v) {
			if (v - p == klen && strncmp(p, key, klen) == 0)
				return v + 1;
		} else {
			if (n - p == klen && strcmp(p, key) == 0)
				return n;
		}
		p = n + 1;
	}

	return NULL;
}

/* Most fields are fixed format which makes it relatively easy for programs to
 * parse the output from modinfo.  But description and author fields can be
 * multi line and can contain any characters.  To make it possible to parse the
 * modinfo output, fields that might be multi line are printed enclosed in '"'
 * for human readability and embedded newlines are printed as "\n  " so the
 * second and subsequent lines are always indented.
 */

static int append_modinfo_tag(struct obj_file *f, char *tag, char *def,
			      char **out_str, int n, int multi_line)
{
	const char *p, *p1;
	int extra = 0, l;

	p = get_modinfo_value(f, tag);
	if (!p && !def)
		return -1;

	if (!p) {
		p = def;
		multi_line = 0;
	}

	if (multi_line) {
		p1 = p;
		extra = 2;
		while ((p1 = strchr(p1, '\n'))) {
			extra += 2;
			++p1;
		}
	}

	if (strlen(p)+extra < n)
		n = strlen(p)+extra;

	if (!multi_line || n <= 2) {
		strncpy(*out_str, p, n);
		*out_str += n;
	}
	else {
		*(*out_str)++ = '"';
		n -= 2;
		p1 = p;
		while (*p1) {
			p1 = strchr(p, '\n');
			if (p1)
				++p1;
			else
				p1 = p + strlen(p);
			l = p1 - p;
			if (l > n)
				l = n;
			strncpy(*out_str, p, l);
			n -= l;
			*out_str += l;
			if (*(p1-1) == '\n' && n >= 2) {
				strncpy(*out_str, "  ", 2);
				*out_str += 2;
				n -= 2;
			}
			p = p1;
		}
		*(*out_str)++ = '"';
	}

	return 0;
}

/* Format the "format" string based on the module contents. */
static char *format_query_string(struct obj_file *f, const char *format)
{
	static char buffer[1024];
	int c;
	const char *in_str = format;
	char *out_str = buffer;
	char *last_char = buffer + sizeof(buffer) - 1;

	while (*in_str && out_str < last_char) {
		/* Just copy normal characters into the output. */
		if (*in_str != '%')
		  {
		    if (*in_str == '\\')
		      c = convert_escape(*++in_str);
		    else
		      c = *in_str;

		    ++in_str;

		    *out_str++ = (char) c;
		    continue;
		  }
		/* Ensure that the replacement is there. */
		if (*++in_str == '\0') {
			error("missing replacement");
			return NULL;
		}
		/* Process the replacement as required. */
		switch (*in_str++) {

		case '%':
			*out_str++ = '%';
			break;

		case 'a':
			(void) append_modinfo_tag(f, "author", "<none>",
				&out_str, last_char - out_str, 1);
			break;

		case 'd':
			(void) append_modinfo_tag(f, "description", "<none>",
				&out_str, last_char - out_str, 1);
			break;

		case 'n':
			(void) append_modinfo_tag(f, "filename", "<none>",
				&out_str, last_char - out_str, 0);
			break;

		case '{':{
				char tag[128], *end = strchr(in_str, '}');
				int multi_line;

				/* Make sure the %{...} is formatted correctly. */
				if (!end) {
					error("unterminated %%{...} construct");
					return NULL;
				}
				if (end - in_str > sizeof(tag) - 1) {
					error("%%{...} construct is too large");
					return NULL;
				}
				/* Copy out the tag name. */
				memset(tag, 0, sizeof(tag));
				strncpy(tag, in_str, end - in_str);
				tag[end - in_str] = '\0';
				multi_line = strcmp(tag, "author") == 0 ||
					     strcmp(tag, "description") == 0;

				/* Append the tag's value if it exists. */
				append_modinfo_tag(f, tag, "<none>", &out_str, last_char - out_str, multi_line);

				/* Advance past the end of the replacement. */
				in_str = end + 1;
				break;
			}

		default:
			error("%c: unknown replacement", in_str[-1]);
			return NULL;
		}		/* switch */
	}			/* while */

	*out_str = '\0';
	return &buffer[0];
}

static void show_parameter(struct obj_file *f, char *key, char *value,
			   const char *desc)
{
	struct obj_symbol *sym;
	int min, max;
	char *p = value;

	sym = obj_find_symbol(f, key);
	if (sym == NULL)
		printf("warning: symbol for parameter %s not found\n", key);

	if (isdigit(*p)) {
		min = strtoul(p, &p, 10);
		if (*p == '-')
			max = strtoul(p + 1, &p, 10);
		else
			max = min;
	} else
		min = max = 1;

	if (max < min)
		printf("warning: parameter %s has max < min!\n", key);

	printf("parm:        %s ", key);

	switch (*p) {
	case 'c':
		printf("char");
		if (!isdigit(p[1]))
			printf(" *** missing string size ***");
		else while (isdigit(p[1]))
			++p;	/* swallow c array size */
		break;
	case 'b':
		printf("byte");
		break;
	case 'h':
		printf("short");
		break;
	case 'i':
		printf("int");
		break;
	case 'l':
		printf("long");
		break;
	case 's':
		printf("string");
		break;
	case '\0':
		printf("no format character!\n");
		return;
	default:
		printf("unknown format character '%c'", *p);
		return;
	}
	switch (*++p) {
	case 'p':
		printf(" persistent");
		if (*(p-1) == 's')
			printf(" (invalid)");
		break;
	case '\0':
		break;
	default:
		printf(" unknown format modifier '%c'", *p);
		break;
	}

	if (min > 1 || max > 1)
		printf(" array (min = %d, max = %d)", min, max);

	if (desc && *desc) {
		const char *p = desc, *p1;
		printf(", description \"");
		p1 = p;
		while (*p1) {
			p1 = strchr(p, '\n');
			if (p1)
				++p1;
			else
				p1 = p + strlen(p);
			printf("%.*s", p1 - p, p);
			if (*(p1-1) == '\n')
				printf("  ");
			p = p1;
		}
		printf("\"");
	}

	printf("\n");
}

static void show_module_parameters(struct obj_file *f)
{
	struct obj_section *sec;
	char *ptr, *value, *n, *endptr;
	int namelen;

	sec = obj_find_section(f, ".modinfo");
	if (sec == NULL) {
		error("module does not support typed parameters");
		return;
	}
	ptr = sec->contents;
	endptr = ptr + sec->header.sh_size;
	while (ptr < endptr) {
		value = strchr(ptr, '=');
		n = strchr(ptr, '\0');
		if (value) {
			namelen = value - ptr;
			if (namelen >= 5 && strncmp(ptr, "parm_", 5) == 0
			    && !(namelen > 10 && strncmp(ptr, "parm_desc_", 10) == 0)) {
				char *pname = xmalloc(namelen + 1);
				char *desckey = xmalloc(namelen + 5 + 1);

				strncpy(pname, ptr + 5, namelen - 5);
				pname[namelen - 5] = '\0';

				strcpy(desckey, "parm_desc_");	/* safe, xmalloc */
				strncat(desckey, ptr + 5, namelen - 5);
				desckey[namelen + 5] = '\0';

				show_parameter(f, pname, value + 1, get_modinfo_value(f, desckey));

				free(pname);
				free(desckey);
			}
		} else {
			if (n - ptr >= 5 && strncmp(ptr, "parm_", 5) == 0) {
				error("parameter %s found with no value", ptr);
			}
		}
		ptr = n + 1;
	}
}

int SHOW_MODULE_INFO(const char *filename, const char *fmtstr, int do_parameters)
{
	int fp;
	struct obj_file *f;
	char *p;

	/* Locate the file to be loaded.  */
	if (!strchr(filename, '/') && !strchr(filename, '.')) {
		char *tmp = search_module_path(filename);
		if (tmp == NULL) {
			error("%s: no module by that name found", filename);
			return 1;
		}
		filename = tmp;
	}
	error_file = filename;

	/* Attempt to open and parse the module file. */
	if ((fp = gzf_open(filename, O_RDONLY)) < 0) {
		error("%s: %m", filename);
		return -1;
	} else if ((f = obj_load(fp, ET_REL, filename)) == NULL)
		return -1;
	gzf_close(fp);

	if (fmtstr) {
		p = format_query_string(f, fmtstr);
		if (p)
			fputs(p, stdout);
	}
	if (do_parameters) {
		show_module_parameters(f);
	}
	return 0;
}

/* For common 3264 code, add an overall show_module_info, in the 64 bit version. */
#if defined(COMMON_3264) && defined(ONLY_64)
int show_module_info(const char *filename, const char *fmtstr, int do_parameters)
{
	if (arch64())
		return show_module_info_64(filename, fmtstr, do_parameters);
	else
		return show_module_info_32(filename, fmtstr, do_parameters);
}
#endif  /* defined(COMMON_3264) && defined(ONLY_64) */

/* For common 3264 code, only compile the usage message once, in the 64 bit version */
#if defined(COMMON_3264) && defined(ONLY_32)
extern void modinfo_usage(void);	/* Use the copy in the 64 bit version */
#else	/* Common 64 bit version or any non common code - compile usage routine */
void modinfo_usage(void)
{
	printf("usage: modinfo <options> <module>\n");
	printf("  -a, --author            display module author\n");
	printf("  -d, --description       display module description\n");
	printf("  -n, --filename          display module filename\n");
	printf("  -f <str>\n");
	printf("      --format <str>      print the <query-string>\n");
	printf("  -p, --parameters        display module parameters\n");
	printf("  -V, --version           show version\n");
	printf("  -h, --help              this usage screen\n");
}
#endif  /* defined(COMMON_3264) && defined(ONLY_32) */


/* For common 3264 code, only compile main in the 64 bit version. */
#if defined(COMMON_3264) && defined(ONLY_32)
/* Use the main in the 64 bit version */
#else
int main(int argc, char *argv[])
{
	static struct option long_opts[] =
	{
		{"author", 0, 0, 'a'},
		{"description", 0, 0, 'd'},
		{"filename", 0, 0, 'n'},
		{"format", required_argument, 0, 'f'},
		{"parameters", 0, 0, 'p'},
		{"version", 0, 0, 'V'},
		{"help", 0, 0, 'h'},
		{0, 0, 0, 0}
	};
	int do_parameters = 1, opt;
	char *fmtstr = "filename:    %n\ndescription: %d\nauthor:      %a\n";

	while ((opt = getopt_long(argc, argv, "adnq:pf:Vh", long_opts, NULL)) != EOF)
		switch (opt) {
		case 'a':
			fmtstr = "%a\n";
			do_parameters = 0;
			break;
		case 'd':
			fmtstr = "%d\n";
			do_parameters = 0;
			break;
		case 'n':
			fmtstr = "%n\n";
			do_parameters = 0;
			break;
		case 'f':
			fmtstr = xstrdup(optarg);
			do_parameters = 0;
			break;
		case 'p':
			do_parameters = 1;
			break;
		case 'V':
			fputs("modinfo (Linux modutils) " MODUTILS_VERSION "\n", stdout);
			exit(0);
		case 'h':

		default:
			modinfo_usage();
			exit(opt == 'h' ? 0 : 1);
		}

	if (optind >= argc) {
		modinfo_usage();
		exit(1);
	}
	error_file = "modinfo";
	while (optind < argc)
		show_module_info(argv[optind++], fmtstr, do_parameters);

	return 0;
}
#endif  /* defined(COMMON_3264) && defined(ONLY_32) */
