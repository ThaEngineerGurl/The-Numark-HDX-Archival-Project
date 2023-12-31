/* C code produced by gperf version 2.7 */
/* Command-line: gperf -a -C -E -g -H is_reserved_hash -k 1,3,$ -N is_reserved_word -p -t genksyms/keywords.gperf  */
#ident "$Id: keywords.c 1.2 Sun, 14 Jan 2001 23:06:24 +1100 kaos $"
struct resword { const char *name; int token; };
/* maximum key range = 101, duplicates = 0 */

#ifdef __GNUC__
__inline
#endif
static unsigned int
is_reserved_hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105,   5,
      105, 105, 105, 105, 105, 105,   0, 105, 105, 105,
        0, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105,   0, 105,   0, 105,  15,
       25,   0,  35,  30, 105,  20, 105, 105,  40,  30,
       30,   0,   0, 105,   0,   0,   0,  10,   5, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
      105, 105, 105, 105, 105, 105
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
const struct resword *
is_reserved_word (str, len)
     register const char *str;
     register unsigned int len;
{
  enum
    {
      TOTAL_KEYWORDS = 38,
      MIN_WORD_LENGTH = 3,
      MAX_WORD_LENGTH = 13,
      MIN_HASH_VALUE = 4,
      MAX_HASH_VALUE = 104
    };

  static const struct resword wordlist[] =
    {
      {""}, {""}, {""}, {""},
      {"auto", AUTO_KEYW},
      {"short", SHORT_KEYW},
      {"struct", STRUCT_KEYW},
      {"__asm__", ASM_KEYW},
      {"restrict", RESTRICT_KEYW},
      {"_restrict", RESTRICT_KEYW},
      {"__signed__", SIGNED_KEYW},
      {"__attribute", ATTRIBUTE_KEYW},
      {"__restrict__", RESTRICT_KEYW},
      {"__attribute__", ATTRIBUTE_KEYW},
      {""},
      {"__volatile", VOLATILE_KEYW},
      {""},
      {"__volatile__", VOLATILE_KEYW},
      {"EXPORT_SYMBOL", EXPORT_SYMBOL_KEYW},
      {"char", CHAR_KEYW},
      {""},
      {"static", STATIC_KEYW},
      {"__const", CONST_KEYW},
      {"int", INT_KEYW},
      {"__const__", CONST_KEYW},
      {""}, {""}, {""},
      {"__inline", INLINE_KEYW},
      {""},
      {"__inline__", INLINE_KEYW},
      {""}, {""},
      {"__signed", SIGNED_KEYW},
      {""},
      {"__asm", ASM_KEYW},
      {"extern", EXTERN_KEYW},
      {""},
      {"register", REGISTER_KEYW},
      {""},
      {"float", FLOAT_KEYW},
      {"double", DOUBLE_KEYW},
      {"typedef", TYPEDEF_KEYW},
      {"unsigned", UNSIGNED_KEYW},
      {"enum", ENUM_KEYW},
      {"_Bool", BOOL_KEYW},
      {""}, {""}, {""}, {""},
      {"const", CONST_KEYW},
      {""}, {""},
      {"volatile", VOLATILE_KEYW},
      {"void", VOID_KEYW},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"signed", SIGNED_KEYW},
      {""},
      {"asm", ASM_KEYW},
      {""},
      {"union", UNION_KEYW},
      {"inline", INLINE_KEYW},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"long", LONG_KEYW}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = is_reserved_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
