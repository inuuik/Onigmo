/*
 * This program was generated by testconv.rb.
 */
#include "config.h"
#ifdef ONIG_ESCAPE_UCHAR_COLLISION
#undef ONIG_ESCAPE_UCHAR_COLLISION
#endif
#include <stdio.h>

#ifdef POSIX_TEST
#include "onigmoposix.h"
#else
#include "onigmo.h"
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#else
# include <strings.h>
#endif

#define SLEN(s)  strlen(s)

static int nsucc  = 0;
static int nfail  = 0;
static int nerror = 0;

static FILE* err_file;

#ifndef POSIX_TEST
static OnigRegion* region;
#endif

static void xx(char* pattern, char* str, int from, int to, int mem, int not)
{
  int r;

#ifdef POSIX_TEST
  regex_t reg;
  char buf[200];
  regmatch_t pmatch[25];

  r = regcomp(&reg, pattern, REG_EXTENDED | REG_NEWLINE);
  if (r) {
    regerror(r, &reg, buf, sizeof(buf));
    fprintf(err_file, "ERROR: %s\n", buf);
    nerror++;
    return ;
  }

  r = regexec(&reg, str, reg.re_nsub + 1, pmatch, 0);
  if (r != 0 && r != REG_NOMATCH) {
    regerror(r, &reg, buf, sizeof(buf));
    fprintf(err_file, "ERROR: %s\n", buf);
    nerror++;
    return ;
  }

  if (r == REG_NOMATCH) {
    if (not) {
      fprintf(stdout, "OK(N): /%s/ '%s'\n", pattern, str);
      nsucc++;
    }
    else {
      fprintf(stdout, "FAIL: /%s/ '%s'\n", pattern, str);
      nfail++;
    }
  }
  else {
    if (not) {
      fprintf(stdout, "FAIL(N): /%s/ '%s'\n", pattern, str);
      nfail++;
    }
    else {
      if (pmatch[mem].rm_so == from && pmatch[mem].rm_eo == to) {
        fprintf(stdout, "OK: /%s/ '%s'\n", pattern, str);
        nsucc++;
      }
      else {
        fprintf(stdout, "FAIL: /%s/ '%s' %d-%d : %d-%d\n", pattern, str,
	        from, to, pmatch[mem].rm_so, pmatch[mem].rm_eo);
        nfail++;
      }
    }
  }
  regfree(&reg);

#else
  regex_t* reg;
  OnigErrorInfo einfo;
  OnigSyntaxType syn = *ONIG_SYNTAX_DEFAULT;

  /* ONIG_OPTION_OFF(syn.options, ONIG_OPTION_ASCII_RANGE); */

  r = onig_new(&reg, (UChar* )pattern, (UChar* )(pattern + SLEN(pattern)),
	       ONIG_OPTION_DEFAULT, ONIG_ENCODING_SJIS, &syn, &einfo);
  if (r) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar* )s, r, &einfo);
    fprintf(err_file, "ERROR: %s\n", s);
    nerror++;
    return ;
  }

  r = onig_search(reg, (UChar* )str, (UChar* )(str + SLEN(str)),
		  (UChar* )str, (UChar* )(str + SLEN(str)),
		  region, ONIG_OPTION_NONE);
  if (r < ONIG_MISMATCH) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar* )s, r);
    fprintf(err_file, "ERROR: %s\n", s);
    nerror++;
    return ;
  }

  if (r == ONIG_MISMATCH) {
    if (not) {
      fprintf(stdout, "OK(N): /%s/ '%s'\n", pattern, str);
      nsucc++;
    }
    else {
      fprintf(stdout, "FAIL: /%s/ '%s'\n", pattern, str);
      nfail++;
    }
  }
  else {
    if (not) {
      fprintf(stdout, "FAIL(N): /%s/ '%s'\n", pattern, str);
      nfail++;
    }
    else {
      if (region->beg[mem] == from && region->end[mem] == to) {
        fprintf(stdout, "OK: /%s/ '%s'\n", pattern, str);
        nsucc++;
      }
      else {
        fprintf(stdout, "FAIL: /%s/ '%s' %d-%d : %d-%d\n", pattern, str,
	        from, to, region->beg[mem], region->end[mem]);
        nfail++;
      }
    }
  }
  onig_free(reg);
#endif
}

static void x2(char* pattern, char* str, int from, int to)
{
  xx(pattern, str, from, to, 0, 0);
}

static void x3(char* pattern, char* str, int from, int to, int mem)
{
  xx(pattern, str, from, to, mem, 0);
}

static void n(char* pattern, char* str)
{
  xx(pattern, str, 0, 0, 0, 1);
}

extern int main(int argc, char* argv[])
{
  err_file = stdout;

#ifdef POSIX_TEST
  reg_set_encoding(REG_POSIX_ENCODING_SJIS);
#else
  region = onig_region_new();
#endif

  x2("", "", 0, 0);
  x2("^", "", 0, 0);
  x2("$", "", 0, 0);
  x2("\\G", "", 0, 0);
  x2("\\A", "", 0, 0);
  x2("\\Z", "", 0, 0);
  x2("\\z", "", 0, 0);
  x2("^$", "", 0, 0);
  x2("\\ca", "\001", 0, 1);
  x2("\\C-b", "\002", 0, 1);
  x2("\\c\\\\", "\034", 0, 1);
  x2("q[\\c\\\\]", "q\034", 0, 2);
  x2("", "a", 0, 0);
  x2("a", "a", 0, 1);
  x2("\\x61", "a", 0, 1);
  x2("aa", "aa", 0, 2);
  x2("aaa", "aaa", 0, 3);
  x2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 0, 35);
  x2("ab", "ab", 0, 2);
  x2("b", "ab", 1, 2);
  x2("bc", "abc", 1, 3);
  x2("(?i:#RET#)", "#INS##RET#", 5, 10);
  x2("\\17", "\017", 0, 1);
  x2("\\x1f", "\x1f", 0, 1);
  x2("a(?#....\\\\JJJJ)b", "ab", 0, 2);
  x2("(?x)  G (o O(?-x)oO) g L", "GoOoOgLe", 0, 7);
  x2(".", "a", 0, 1);
  n(".", "");
  x2("..", "ab", 0, 2);
  x2("\\w", "e", 0, 1);
  n("\\W", "e");
  x2("\\s", " ", 0, 1);
  x2("\\S", "b", 0, 1);
  x2("\\d", "4", 0, 1);
  n("\\D", "4");
  x2("\\b", "z ", 0, 0);
  x2("\\b", " z", 1, 1);
  x2("\\B", "zz ", 1, 1);
  x2("\\B", "z ", 2, 2);
  x2("\\B", " z", 0, 0);
  x2("[ab]", "b", 0, 1);
  n("[ab]", "c");
  x2("[a-z]", "t", 0, 1);
  n("[^a]", "a");
  x2("[^a]", "\n", 0, 1);
  x2("[]]", "]", 0, 1);
  n("[^]]", "]");
  x2("[\\^]+", "0^^1", 1, 3);
  x2("[b-]", "b", 0, 1);
  x2("[b-]", "-", 0, 1);
  x2("[\\w]", "z", 0, 1);
  n("[\\w]", " ");
  x2("[\\W]", "b$", 1, 2);
  x2("[\\d]", "5", 0, 1);
  n("[\\d]", "e");
  x2("[\\D]", "t", 0, 1);
  n("[\\D]", "3");
  x2("[\\s]", " ", 0, 1);
  n("[\\s]", "a");
  x2("[\\S]", "b", 0, 1);
  n("[\\S]", " ");
  x2("[\\w\\d]", "2", 0, 1);
  n("[\\w\\d]", " ");
  x2("[[:upper:]]", "B", 0, 1);
  x2("[*[:xdigit:]+]", "+", 0, 1);
  x2("[*[:xdigit:]+]", "GHIKK-9+*", 6, 7);
  x2("[*[:xdigit:]+]", "-@^+", 3, 4);
  n("[[:upper]]", "A");
  x2("[[:upper]]", ":", 0, 1);
  x2("[\\044-\\047]", "\046", 0, 1);
  x2("[\\x5a-\\x5c]", "\x5b", 0, 1);
  x2("[\\x6A-\\x6D]", "\x6c", 0, 1);
  n("[\\x6A-\\x6D]", "\x6E");
  n("^[0-9A-F]+ 0+ UNDEF ", "75F 00000000 SECT14A notype ()    External    | _rb_apply");
  x2("[\\[]", "[", 0, 1);
  x2("[\\]]", "]", 0, 1);
  x2("[&]", "&", 0, 1);
  x2("[[ab]]", "b", 0, 1);
  x2("[[ab]c]", "c", 0, 1);
  n("[[^a]]", "a");
  n("[^[a]]", "a");
  x2("[[ab]&&bc]", "b", 0, 1);
  n("[[ab]&&bc]", "a");
  n("[[ab]&&bc]", "c");
  x2("[a-z&&b-y&&c-x]", "w", 0, 1);
  n("[^a-z&&b-y&&c-x]", "w");
  x2("[[^a&&a]&&a-z]", "b", 0, 1);
  n("[[^a&&a]&&a-z]", "a");
  x2("[[^a-z&&bcdef]&&[^c-g]]", "h", 0, 1);
  n("[[^a-z&&bcdef]&&[^c-g]]", "c");
  x2("[^[^abc]&&[^cde]]", "c", 0, 1);
  x2("[^[^abc]&&[^cde]]", "e", 0, 1);
  n("[^[^abc]&&[^cde]]", "f");
  x2("[a-&&-a]", "-", 0, 1);
  n("[a\\-&&\\-a]", "&");
  n("\\wabc", " abc");
  x2("a\\Wbc", "a bc", 0, 4);
  x2("a.b.c", "aabbc", 0, 5);
  x2(".\\wb\\W..c", "abb bcc", 0, 7);
  x2("\\s\\wzzz", " zzzz", 0, 5);
  x2("aa.b", "aabb", 0, 4);
  n(".a", "ab");
  x2(".a", "aa", 0, 2);
  x2("^a", "a", 0, 1);
  x2("^a$", "a", 0, 1);
  x2("^\\w$", "a", 0, 1);
  n("^\\w$", " ");
  x2("^\\wab$", "zab", 0, 3);
  x2("^\\wabcdef$", "zabcdef", 0, 7);
  x2("^\\w...def$", "zabcdef", 0, 7);
  x2("\\w\\w\\s\\Waaa\\d", "aa  aaa4", 0, 8);
  x2("\\A\\Z", "", 0, 0);
  x2("\\Axyz", "xyz", 0, 3);
  x2("xyz\\Z", "xyz", 0, 3);
  x2("xyz\\z", "xyz", 0, 3);
  x2("a\\Z", "a", 0, 1);
  x2("\\Gaz", "az", 0, 2);
  n("\\Gz", "bza");
  n("az\\G", "az");
  n("az\\A", "az");
  n("a\\Az", "az");
  x2("\\^\\$", "^$", 0, 2);
  x2("^x?y", "xy", 0, 2);
  x2("^(x?y)", "xy", 0, 2);
  x2("\\w", "_", 0, 1);
  n("\\W", "_");
  x2("(?=z)z", "z", 0, 1);
  n("(?=z).", "a");
  x2("(?!z)a", "a", 0, 1);
  n("(?!z)a", "z");
  x2("(?i:a)", "a", 0, 1);
  x2("(?i:a)", "A", 0, 1);
  x2("(?i:A)", "a", 0, 1);
  n("(?i:A)", "b");
  x2("(?i:[A-Z])", "a", 0, 1);
  x2("(?i:[f-m])", "H", 0, 1);
  x2("(?i:[f-m])", "h", 0, 1);
  n("(?i:[f-m])", "e");
  x2("(?i:[A-c])", "D", 0, 1);
  n("(?i:[^a-z])", "A");
  n("(?i:[^a-z])", "a");
  x2("(?i:[!-k])", "Z", 0, 1);
  x2("(?i:[!-k])", "7", 0, 1);
  x2("(?i:[T-}])", "b", 0, 1);
  x2("(?i:[T-}])", "{", 0, 1);
  x2("(?i:\\?a)", "?A", 0, 2);
  x2("(?i:\\*A)", "*a", 0, 2);
  n(".", "\n");
  x2("(?m:.)", "\n", 0, 1);
  x2("(?m:a.)", "a\n", 0, 2);
  x2("(?m:.b)", "a\nb", 1, 3);
  x2(".*abc", "dddabdd\nddabc", 8, 13);
  x2("(?m:.*abc)", "dddabddabc", 0, 10);
  n("(?i)(?-i)a", "A");
  n("(?i)(?-i:a)", "A");
  x2("a?", "", 0, 0);
  x2("a?", "b", 0, 0);
  x2("a?", "a", 0, 1);
  x2("a*", "", 0, 0);
  x2("a*", "a", 0, 1);
  x2("a*", "aaa", 0, 3);
  x2("a*", "baaaa", 0, 0);
  n("a+", "");
  x2("a+", "a", 0, 1);
  x2("a+", "aaaa", 0, 4);
  x2("a+", "aabbb", 0, 2);
  x2("a+", "baaaa", 1, 5);
  x2(".?", "", 0, 0);
  x2(".?", "f", 0, 1);
  x2(".?", "\n", 0, 0);
  x2(".*", "", 0, 0);
  x2(".*", "abcde", 0, 5);
  x2(".+", "z", 0, 1);
  x2(".+", "zdswer\n", 0, 6);
  x2("(.*)a\\1f", "babfbac", 0, 4);
  x2("(.*)a\\1f", "bacbabf", 3, 7);
  x2("((.*)a\\2f)", "bacbabf", 3, 7);
  x2("(.*)a\\1f", "baczzzzzz\nbazz\nzzzzbabf", 19, 23);
  x2("a|b", "a", 0, 1);
  x2("a|b", "b", 0, 1);
  x2("|a", "a", 0, 0);
  x2("(|a)", "a", 0, 0);
  x2("ab|bc", "ab", 0, 2);
  x2("ab|bc", "bc", 0, 2);
  x2("z(?:ab|bc)", "zbc", 0, 3);
  x2("a(?:ab|bc)c", "aabc", 0, 4);
  x2("ab|(?:ac|az)", "az", 0, 2);
  x2("a|b|c", "dc", 1, 2);
  x2("a|b|cd|efg|h|ijk|lmn|o|pq|rstuvwx|yz", "pqr", 0, 2);
  n("a|b|cd|efg|h|ijk|lmn|o|pq|rstuvwx|yz", "mn");
  x2("a|^z", "ba", 1, 2);
  x2("a|^z", "za", 0, 1);
  x2("a|\\Gz", "bza", 2, 3);
  x2("a|\\Gz", "za", 0, 1);
  x2("a|\\Az", "bza", 2, 3);
  x2("a|\\Az", "za", 0, 1);
  x2("a|b\\Z", "ba", 1, 2);
  x2("a|b\\Z", "b", 0, 1);
  x2("a|b\\z", "ba", 1, 2);
  x2("a|b\\z", "b", 0, 1);
  x2("\\w|\\s", " ", 0, 1);
  n("\\w|\\w", " ");
  x2("\\w|%", "%", 0, 1);
  x2("\\w|[&$]", "&", 0, 1);
  x2("[b-d]|[^e-z]", "a", 0, 1);
  x2("(?:a|[c-f])|bz", "dz", 0, 1);
  x2("(?:a|[c-f])|bz", "bz", 0, 2);
  x2("abc|(?=zz)..f", "zzf", 0, 3);
  x2("abc|(?!zz)..f", "abf", 0, 3);
  x2("(?=za)..a|(?=zz)..a", "zza", 0, 3);
  n("(?>a|abd)c", "abdc");
  x2("(?>abd|a)c", "abdc", 0, 4);
  x2("a?|b", "a", 0, 1);
  x2("a?|b", "b", 0, 0);
  x2("a?|b", "", 0, 0);
  x2("a*|b", "aa", 0, 2);
  x2("a*|b*", "ba", 0, 0);
  x2("a*|b*", "ab", 0, 1);
  x2("a+|b*", "", 0, 0);
  x2("a+|b*", "bbb", 0, 3);
  x2("a+|b*", "abbb", 0, 1);
  n("a+|b+", "");
  x2("(a|b)?", "b", 0, 1);
  x2("(a|b)*", "ba", 0, 2);
  x2("(a|b)+", "bab", 0, 3);
  x2("(ab|ca)+", "caabbc", 0, 4);
  x2("(ab|ca)+", "aabca", 1, 5);
  x2("(ab|ca)+", "abzca", 0, 2);
  x2("(a|bab)+", "ababa", 0, 5);
  x2("(a|bab)+", "ba", 1, 2);
  x2("(a|bab)+", "baaaba", 1, 4);
  x2("(?:a|b)(?:a|b)", "ab", 0, 2);
  x2("(?:a*|b*)(?:a*|b*)", "aaabbb", 0, 3);
  x2("(?:a*|b*)(?:a+|b+)", "aaabbb", 0, 6);
  x2("(?:a+|b+){2}", "aaabbb", 0, 6);
  x2("h{0,}", "hhhh", 0, 4);
  x2("(?:a+|b+){1,2}", "aaabbb", 0, 6);
  n("ax{2}*a", "0axxxa1");
  n("a.{0,2}a", "0aXXXa0");
  n("a.{0,2}?a", "0aXXXa0");
  n("a.{0,2}?a", "0aXXXXa0");
  x2("^a{2,}?a$", "aaa", 0, 3);
  x2("^[a-z]{2,}?$", "aaa", 0, 3);
  x2("(?:a+|\\Ab*)cc", "cc", 0, 2);
  n("(?:a+|\\Ab*)cc", "abcc");
  x2("(?:^a+|b+)*c", "aabbbabc", 6, 8);
  x2("(?:^a+|b+)*c", "aabbbbc", 0, 7);
  x2("a|(?i)c", "C", 0, 1);
  x2("(?i)c|a", "C", 0, 1);
  x2("(?i)c|a", "A", 0, 1);
  x2("(?i:c)|a", "C", 0, 1);
  n("(?i:c)|a", "A");
  x2("[abc]?", "abc", 0, 1);
  x2("[abc]*", "abc", 0, 3);
  x2("[^abc]*", "abc", 0, 0);
  n("[^abc]+", "abc");
  x2("a?\?", "aaa", 0, 0);
  x2("ba?\?b", "bab", 0, 3);
  x2("a*?", "aaa", 0, 0);
  x2("ba*?", "baa", 0, 1);
  x2("ba*?b", "baab", 0, 4);
  x2("a+?", "aaa", 0, 1);
  x2("ba+?", "baa", 0, 2);
  x2("ba+?b", "baab", 0, 4);
  x2("(?:a?)?\?", "a", 0, 0);
  x2("(?:a?\?)?", "a", 0, 0);
  x2("(?:a?)+?", "aaa", 0, 1);
  x2("(?:a+)?\?", "aaa", 0, 0);
  x2("(?:a+)?\?b", "aaab", 0, 4);
  x2("(?:ab)?{2}", "", 0, 0);
  x2("(?:ab)?{2}", "ababa", 0, 4);
  x2("(?:ab)*{0}", "ababa", 0, 0);
  x2("(?:ab){3,}", "abababab", 0, 8);
  n("(?:ab){3,}", "abab");
  x2("(?:ab){2,4}", "ababab", 0, 6);
  x2("(?:ab){2,4}", "ababababab", 0, 8);
  x2("(?:ab){2,4}?", "ababababab", 0, 4);
  x2("(?:ab){,}", "ab{,}", 0, 5);
  x2("(?:abc)+?{2}", "abcabcabc", 0, 6);
  x2("(?:X*)(?i:xa)", "XXXa", 0, 4);
  x2("(d+)([^abc]z)", "dddz", 0, 4);
  x2("([^abc]*)([^abc]z)", "dddz", 0, 4);
  x2("(\\w+)(\\wz)", "dddz", 0, 4);
  x3("(a)", "a", 0, 1, 1);
  x3("(ab)", "ab", 0, 2, 1);
  x2("((ab))", "ab", 0, 2);
  x3("((ab))", "ab", 0, 2, 1);
  x3("((ab))", "ab", 0, 2, 2);
  x3("((((((((((((((((((((ab))))))))))))))))))))", "ab", 0, 2, 20);
  x3("(ab)(cd)", "abcd", 0, 2, 1);
  x3("(ab)(cd)", "abcd", 2, 4, 2);
  x3("()(a)bc(def)ghijk", "abcdefghijk", 3, 6, 3);
  x3("(()(a)bc(def)ghijk)", "abcdefghijk", 3, 6, 4);
  x2("(^a)", "a", 0, 1);
  x3("(a)|(a)", "ba", 1, 2, 1);
  x3("(^a)|(a)", "ba", 1, 2, 2);
  x3("(a?)", "aaa", 0, 1, 1);
  x3("(a*)", "aaa", 0, 3, 1);
  x3("(a*)", "", 0, 0, 1);
  x3("(a+)", "aaaaaaa", 0, 7, 1);
  x3("(a+|b*)", "bbbaa", 0, 3, 1);
  x3("(a+|b?)", "bbbaa", 0, 1, 1);
  x3("(abc)?", "abc", 0, 3, 1);
  x3("(abc)*", "abc", 0, 3, 1);
  x3("(abc)+", "abc", 0, 3, 1);
  x3("(xyz|abc)+", "abc", 0, 3, 1);
  x3("([xyz][abc]|abc)+", "abc", 0, 3, 1);
  x3("((?i:abc))", "AbC", 0, 3, 1);
  x2("(abc)(?i:\\1)", "abcABC", 0, 6);
  x3("((?m:a.c))", "a\nc", 0, 3, 1);
  x3("((?=az)a)", "azb", 0, 1, 1);
  x3("abc|(.abd)", "zabd", 0, 4, 1);
  x2("(?:abc)|(ABC)", "abc", 0, 3);
  x3("(?i:(abc))|(zzz)", "ABC", 0, 3, 1);
  x3("a*(.)", "aaaaz", 4, 5, 1);
  x3("a*?(.)", "aaaaz", 0, 1, 1);
  x3("a*?(c)", "aaaac", 4, 5, 1);
  x3("[bcd]a*(.)", "caaaaz", 5, 6, 1);
  x3("(\\Abb)cc", "bbcc", 0, 2, 1);
  n("(\\Abb)cc", "zbbcc");
  x3("(^bb)cc", "bbcc", 0, 2, 1);
  n("(^bb)cc", "zbbcc");
  x3("cc(bb$)", "ccbb", 2, 4, 1);
  n("cc(bb$)", "ccbbb");
  n("(\\1)", "");
  n("\\1(a)", "aa");
  n("(a(b)\\1)\\2+", "ababb");
  n("(?:(?:\\1|z)(a))+$", "zaa");
  x2("(?:(?:\\1|z)(a))+$", "zaaa", 0, 4);
  x2("(a)(?=\\1)", "aa", 0, 1);
  n("(a)$|\\1", "az");
  x2("(a)\\1", "aa", 0, 2);
  n("(a)\\1", "ab");
  x2("(a?)\\1", "aa", 0, 2);
  x2("(a?\?)\\1", "aa", 0, 0);
  x2("(a*)\\1", "aaaaa", 0, 4);
  x3("(a*)\\1", "aaaaa", 0, 2, 1);
  x2("a(b*)\\1", "abbbb", 0, 5);
  x2("a(b*)\\1", "ab", 0, 1);
  x2("(a*)(b*)\\1\\2", "aaabbaaabb", 0, 10);
  x2("(a*)(b*)\\2", "aaabbbb", 0, 7);
  x2("(((((((a*)b))))))c\\7", "aaabcaaa", 0, 8);
  x3("(((((((a*)b))))))c\\7", "aaabcaaa", 0, 3, 7);
  x2("(a)(b)(c)\\2\\1\\3", "abcbac", 0, 6);
  x2("([a-d])\\1", "cc", 0, 2);
  x2("(\\w\\d\\s)\\1", "f5 f5 ", 0, 6);
  n("(\\w\\d\\s)\\1", "f5 f5");
  x2("(who|[a-c]{3})\\1", "whowho", 0, 6);
  x2("...(who|[a-c]{3})\\1", "abcwhowho", 0, 9);
  x2("(who|[a-c]{3})\\1", "cbccbc", 0, 6);
  x2("(^a)\\1", "aa", 0, 2);
  n("(^a)\\1", "baa");
  n("(a$)\\1", "aa");
  n("(ab\\Z)\\1", "ab");
  x2("(a*\\Z)\\1", "a", 1, 1);
  x2(".(a*\\Z)\\1", "ba", 1, 2);
  x3("(.(abc)\\2)", "zabcabc", 0, 7, 1);
  x3("(.(..\\d.)\\2)", "z12341234", 0, 9, 1);
  x2("((?i:az))\\1", "AzAz", 0, 4);
  n("((?i:az))\\1", "Azaz");
  x2("(?<=a)b", "ab", 1, 2);
  n("(?<=a)b", "bb");
  x2("(?<=a|b)b", "bb", 1, 2);
  x2("(?<=a|bc)b", "bcb", 2, 3);
  x2("(?<=a|bc)b", "ab", 1, 2);
  x2("(?<=a|bc||defghij|klmnopq|r)z", "rz", 1, 2);
  x2("(a)\\g<1>", "aa", 0, 2);
  x2("(?<!a)b", "cb", 1, 2);
  n("(?<!a)b", "ab");
  x2("(?<!a|bc)b", "bbb", 0, 1);
  n("(?<!a|bc)z", "bcz");
  x2("(?<name1>a)", "a", 0, 1);
  x2("(?<name_2>ab)\\g<name_2>", "abab", 0, 4);
  x2("(?<name_3>.zv.)\\k<name_3>", "azvbazvb", 0, 8);
  x2("(?<=\\g<ab>)|-\\zEND (?<ab>XyZ)", "XyZ", 3, 3);
  x2("(?<n>|a\\g<n>)+", "", 0, 0);
  x2("(?<n>|\\(\\g<n>\\))+$", "()(())", 0, 6);
  x3("\\g<n>(?<n>.){0}", "X", 0, 1, 1);
  x2("\\g<n>(abc|df(?<n>.YZ){2,8}){0}", "XYZ", 0, 3);
  x2("\\A(?<n>(a\\g<n>)|)\\z", "aaaa", 0, 4);
  x2("(?<n>|\\g<m>\\g<n>)\\z|\\zEND (?<m>a|(b)\\g<m>)", "bbbbabba", 0, 8);
  x2("(?<name1240>\\w+\\sx)a+\\k<name1240>", "  fg xaaaaaaaafg x", 2, 18);
  x3("(z)()()(?<_9>a)\\g<_9>", "zaa", 2, 3, 1);
  x2("(.)(((?<_>a)))\\k<_>", "zaa", 0, 3);
  x2("((?<name1>\\d)|(?<name2>\\w))(\\k<name1>|\\k<name2>)", "ff", 0, 2);
  x2("(?:(?<x>)|(?<x>efg))\\k<x>", "", 0, 0);
  x2("(?:(?<x>abc)|(?<x>efg))\\k<x>", "abcefgefg", 3, 9);
  n("(?:(?<x>abc)|(?<x>efg))\\k<x>", "abcefg");
  x2("(?:(?<n1>.)|(?<n1>..)|(?<n1>...)|(?<n1>....)|(?<n1>.....)|(?<n1>......)|(?<n1>.......)|(?<n1>........)|(?<n1>.........)|(?<n1>..........)|(?<n1>...........)|(?<n1>............)|(?<n1>.............)|(?<n1>..............))\\k<n1>$", "a-pyumpyum", 2, 10);
  x3("(?:(?<n1>.)|(?<n1>..)|(?<n1>...)|(?<n1>....)|(?<n1>.....)|(?<n1>......)|(?<n1>.......)|(?<n1>........)|(?<n1>.........)|(?<n1>..........)|(?<n1>...........)|(?<n1>............)|(?<n1>.............)|(?<n1>..............))\\k<n1>$", "xxxxabcdefghijklmnabcdefghijklmn", 4, 18, 14);
  x3("(?<name1>)(?<name2>)(?<name3>)(?<name4>)(?<name5>)(?<name6>)(?<name7>)(?<name8>)(?<name9>)(?<name10>)(?<name11>)(?<name12>)(?<name13>)(?<name14>)(?<name15>)(?<name16>aaa)(?<name17>)$", "aaa", 0, 3, 16);
  x2("(?<foo>a|\\(\\g<foo>\\))", "a", 0, 1);
  x2("(?<foo>a|\\(\\g<foo>\\))", "((((((a))))))", 0, 13);
  x3("(?<foo>a|\\(\\g<foo>\\))", "((((((((a))))))))", 0, 17, 1);
  x2("\\g<bar>|\\zEND(?<bar>.*abc$)", "abcxxxabc", 0, 9);
  x2("\\g<1>|\\zEND(.a.)", "bac", 0, 3);
  x3("\\g<_A>\\g<_A>|\\zEND(.a.)(?<_A>.b.)", "xbxyby", 3, 6, 1);
  x2("\\A(?:\\g<pon>|\\g<pan>|\\zEND  (?<pan>a|c\\g<pon>c)(?<pon>b|d\\g<pan>d))$", "cdcbcdc", 0, 7);
  x2("\\A(?<n>|a\\g<m>)\\z|\\zEND (?<m>\\g<n>)", "aaaa", 0, 4);
  x2("(?<n>(a|b\\g<n>c){3,5})", "baaaaca", 1, 5);
  x2("(?<n>(a|b\\g<n>c){3,5})", "baaaacaaaaa", 0, 10);
  x2("(?<pare>\\(([^\\(\\)]++|\\g<pare>)*+\\))", "((a))", 0, 5);
  x2("()*\\1", "", 0, 0);
  x2("(?:()|())*\\1\\2", "", 0, 0);
  x3("(?:\\1a|())*", "a", 0, 0, 1);
  x2("x((.)*)*x", "0x1x2x3", 1, 6);
  x2("x((.)*)*x(?i:\\1)\\Z", "0x1x2x1X2", 1, 9);
  x2("(?:()|()|()|()|()|())*\\2\\5", "", 0, 0);
  x2("(?:()|()|()|(x)|()|())*\\2b\\5", "b", 0, 1);
  x2("\\xED\\xF2", "\xed\xf2", 0, 2);
  x2("", "あ", 0, 0);
  x2("あ", "あ", 0, 2);
  n("い", "あ");
  x2("うう", "うう", 0, 4);
  x2("あいう", "あいう", 0, 6);
  x2("こここここここここここここここここここここここここここここここここここ", "こここここここここここここここここここここここここここここここここここ", 0, 70);
  x2("あ", "いあ", 2, 4);
  x2("いう", "あいう", 2, 6);
  x2("\\xca\\xb8", "\xca\xb8", 0, 2);
  x2(".", "あ", 0, 2);
  x2("..", "かき", 0, 4);
  x2("(?u)\\w", "お", 0, 2);
  n("(?u)\\W", "あ");
  x2("(?u)[\\W]", "う$", 2, 3);
  x2("\\S", "そ", 0, 2);
  x2("\\S", "漢", 0, 2);
  x2("\\b", "気 ", 0, 0);
  x2("\\b", " ほ", 1, 1);
  x2("\\B", "せそ ", 2, 2);
  x2("\\B", "う ", 3, 3);
  x2("\\B", " い", 0, 0);
  x2("[たち]", "ち", 0, 2);
  n("[なに]", "ぬ");
  x2("[う-お]", "え", 0, 2);
  n("[^け]", "け");
  x2("(?u)[\\w]", "ね", 0, 2);
  n("[\\d]", "ふ");
  x2("[\\D]", "は", 0, 2);
  n("[\\s]", "く");
  x2("[\\S]", "へ", 0, 2);
  x2("(?u)[\\w\\d]", "よ", 0, 2);
  x2("(?u)[\\w\\d]", "   よ", 3, 5);
  n("(?u)\\w鬼車", " 鬼車");
  x2("(?u)鬼\\W車", "鬼 車", 0, 5);
  x2("あ.い.う", "ああいいう", 0, 10);
  x2("(?u).\\wう\\W..ぞ", "えうう うぞぞ", 0, 13);
  x2("(?u)\\s\\wこここ", " ここここ", 0, 9);
  x2("ああ.け", "ああけけ", 0, 8);
  n(".い", "いえ");
  x2(".お", "おお", 0, 4);
  x2("^あ", "あ", 0, 2);
  x2("^む$", "む", 0, 2);
  x2("(?u)^\\w$", "に", 0, 2);
  x2("(?u)^\\wかきくけこ$", "zかきくけこ", 0, 11);
  x2("(?u)^\\w...うえお$", "zあいううえお", 0, 13);
  x2("(?u)\\w\\w\\s\\Wおおお\\d", "aお  おおお4", 0, 12);
  x2("\\Aたちつ", "たちつ", 0, 6);
  x2("むめも\\Z", "むめも", 0, 6);
  x2("かきく\\z", "かきく", 0, 6);
  x2("かきく\\Z", "かきく\n", 0, 6);
  x2("\\Gぽぴ", "ぽぴ", 0, 4);
  n("\\Gえ", "うえお");
  n("とて\\G", "とて");
  n("まみ\\A", "まみ");
  n("ま\\Aみ", "まみ");
  x2("(?=せ)せ", "せ", 0, 2);
  n("(?=う).", "い");
  x2("(?!う)か", "か", 0, 2);
  n("(?!と)あ", "と");
  x2("(?i:あ)", "あ", 0, 2);
  x2("(?i:ぶべ)", "ぶべ", 0, 4);
  n("(?i:い)", "う");
  x2("(?m:よ.)", "よ\n", 0, 3);
  x2("(?m:.め)", "ま\nめ", 2, 5);
  x2("あ?", "", 0, 0);
  x2("変?", "化", 0, 0);
  x2("変?", "変", 0, 2);
  x2("量*", "", 0, 0);
  x2("量*", "量", 0, 2);
  x2("子*", "子子子", 0, 6);
  x2("馬*", "鹿馬馬馬馬", 0, 0);
  n("山+", "");
  x2("河+", "河", 0, 2);
  x2("時+", "時時時時", 0, 8);
  x2("え+", "ええううう", 0, 4);
  x2("う+", "おうううう", 2, 10);
  x2(".?", "た", 0, 2);
  x2(".*", "ぱぴぷぺ", 0, 8);
  x2(".+", "ろ", 0, 2);
  x2(".+", "いうえか\n", 0, 8);
  x2("あ|い", "あ", 0, 2);
  x2("あ|い", "い", 0, 2);
  x2("あい|いう", "あい", 0, 4);
  x2("あい|いう", "いう", 0, 4);
  x2("を(?:かき|きく)", "をかき", 0, 6);
  x2("を(?:かき|きく)け", "をきくけ", 0, 8);
  x2("あい|(?:あう|あを)", "あを", 0, 4);
  x2("あ|い|う", "えう", 2, 4);
  x2("あ|い|うえ|おかき|く|けこさ|しすせ|そ|たち|つてとなに|ぬね", "しすせ", 0, 6);
  n("あ|い|うえ|おかき|く|けこさ|しすせ|そ|たち|つてとなに|ぬね", "すせ");
  x2("あ|^わ", "ぶあ", 2, 4);
  x2("あ|^を", "をあ", 0, 2);
  x2("鬼|\\G車", "け車鬼", 4, 6);
  x2("鬼|\\G車", "車鬼", 0, 2);
  x2("鬼|\\A車", "b車鬼", 3, 5);
  x2("鬼|\\A車", "車", 0, 2);
  x2("鬼|車\\Z", "車鬼", 2, 4);
  x2("鬼|車\\Z", "車", 0, 2);
  x2("鬼|車\\Z", "車\n", 0, 2);
  x2("鬼|車\\z", "車鬼", 2, 4);
  x2("鬼|車\\z", "車", 0, 2);
  x2("(?u)\\w|\\s", "お", 0, 2);
  x2("(?u)\\w|%", "%お", 0, 1);
  x2("(?u)\\w|[&$]", "う&", 0, 2);
  x2("[い-け]", "う", 0, 2);
  x2("[い-け]|[^か-こ]", "あ", 0, 2);
  x2("[い-け]|[^か-こ]", "か", 0, 2);
  x2("[^あ]", "\n", 0, 1);
  x2("(?:あ|[う-き])|いを", "うを", 0, 2);
  x2("(?:あ|[う-き])|いを", "いを", 0, 4);
  x2("あいう|(?=けけ)..ほ", "けけほ", 0, 6);
  x2("あいう|(?!けけ)..ほ", "あいほ", 0, 6);
  x2("(?=をあ)..あ|(?=をを)..あ", "ををあ", 0, 6);
  x2("(?<=あ|いう)い", "いうい", 4, 6);
  n("(?>あ|あいえ)う", "あいえう");
  x2("(?>あいえ|あ)う", "あいえう", 0, 8);
  x2("あ?|い", "あ", 0, 2);
  x2("あ?|い", "い", 0, 0);
  x2("あ?|い", "", 0, 0);
  x2("あ*|い", "ああ", 0, 4);
  x2("あ*|い*", "いあ", 0, 0);
  x2("あ*|い*", "あい", 0, 2);
  x2("[aあ]*|い*", "aあいいい", 0, 3);
  x2("あ+|い*", "", 0, 0);
  x2("あ+|い*", "いいい", 0, 6);
  x2("あ+|い*", "あいいい", 0, 2);
  x2("あ+|い*", "aあいいい", 0, 0);
  n("あ+|い+", "");
  x2("(あ|い)?", "い", 0, 2);
  x2("(あ|い)*", "いあ", 0, 4);
  x2("(あ|い)+", "いあい", 0, 6);
  x2("(あい|うあ)+", "うああいうえ", 0, 8);
  x2("(あい|うえ)+", "うああいうえ", 4, 12);
  x2("(あい|うあ)+", "ああいうあ", 2, 10);
  x2("(あい|うあ)+", "あいをうあ", 0, 4);
  x2("(あい|うあ)+", "$$zzzzあいをうあ", 6, 10);
  x2("(あ|いあい)+", "あいあいあ", 0, 10);
  x2("(あ|いあい)+", "いあ", 2, 4);
  x2("(あ|いあい)+", "いあああいあ", 2, 8);
  x2("(?:あ|い)(?:あ|い)", "あい", 0, 4);
  x2("(?:あ*|い*)(?:あ*|い*)", "あああいいい", 0, 6);
  x2("(?:あ*|い*)(?:あ+|い+)", "あああいいい", 0, 12);
  x2("(?:あ+|い+){2}", "あああいいい", 0, 12);
  x2("(?:あ+|い+){1,2}", "あああいいい", 0, 12);
  x2("(?:あ+|\\Aい*)うう", "うう", 0, 4);
  n("(?:あ+|\\Aい*)うう", "あいうう");
  x2("(?:^あ+|い+)*う", "ああいいいあいう", 12, 16);
  x2("(?:^あ+|い+)*う", "ああいいいいう", 0, 14);
  x2("う{0,}", "うううう", 0, 8);
  x2("あ|(?i)c", "C", 0, 1);
  x2("(?i)c|あ", "C", 0, 1);
  x2("(?i:あ)|a", "a", 0, 1);
  n("(?i:あ)|a", "A");
  x2("[あいう]?", "あいう", 0, 2);
  x2("[あいう]*", "あいう", 0, 6);
  x2("[^あいう]*", "あいう", 0, 0);
  n("[^あいう]+", "あいう");
  x2("あ?\?", "あああ", 0, 0);
  x2("いあ?\?い", "いあい", 0, 6);
  x2("あ*?", "あああ", 0, 0);
  x2("いあ*?", "いああ", 0, 2);
  x2("いあ*?い", "いああい", 0, 8);
  x2("あ+?", "あああ", 0, 2);
  x2("いあ+?", "いああ", 0, 4);
  x2("いあ+?い", "いああい", 0, 8);
  x2("(?:天?)?\?", "天", 0, 0);
  x2("(?:天?\?)?", "天", 0, 0);
  x2("(?:夢?)+?", "夢夢夢", 0, 2);
  x2("(?:風+)?\?", "風風風", 0, 0);
  x2("(?:雪+)?\?霜", "雪雪雪霜", 0, 8);
  x2("(?:あい)?{2}", "", 0, 0);
  x2("(?:鬼車)?{2}", "鬼車鬼車鬼", 0, 8);
  x2("(?:鬼車)*{0}", "鬼車鬼車鬼", 0, 0);
  x2("(?:鬼車){3,}", "鬼車鬼車鬼車鬼車", 0, 16);
  n("(?:鬼車){3,}", "鬼車鬼車");
  x2("(?:鬼車){2,4}", "鬼車鬼車鬼車", 0, 12);
  x2("(?:鬼車){2,4}", "鬼車鬼車鬼車鬼車鬼車", 0, 16);
  x2("(?:鬼車){2,4}?", "鬼車鬼車鬼車鬼車鬼車", 0, 8);
  x2("(?:鬼車){,}", "鬼車{,}", 0, 7);
  x2("(?:かきく)+?{2}", "かきくかきくかきく", 0, 12);
  x3("(火)", "火", 0, 2, 1);
  x3("(火水)", "火水", 0, 4, 1);
  x2("((時間))", "時間", 0, 4);
  x3("((風水))", "風水", 0, 4, 1);
  x3("((昨日))", "昨日", 0, 4, 2);
  x3("((((((((((((((((((((量子))))))))))))))))))))", "量子", 0, 4, 20);
  x3("(あい)(うえ)", "あいうえ", 0, 4, 1);
  x3("(あい)(うえ)", "あいうえ", 4, 8, 2);
  x3("()(あ)いう(えおか)きくけこ", "あいうえおかきくけこ", 6, 12, 3);
  x3("(()(あ)いう(えおか)きくけこ)", "あいうえおかきくけこ", 6, 12, 4);
  x3(".*(フォ)ン・マ(ン()シュタ)イン", "フォン・マンシュタイン", 10, 18, 2);
  x2("(^あ)", "あ", 0, 2);
  x3("(あ)|(あ)", "いあ", 2, 4, 1);
  x3("(^あ)|(あ)", "いあ", 2, 4, 2);
  x3("(あ?)", "あああ", 0, 2, 1);
  x3("(ま*)", "ままま", 0, 6, 1);
  x3("(と*)", "", 0, 0, 1);
  x3("(る+)", "るるるるるるる", 0, 14, 1);
  x3("(ふ+|へ*)", "ふふふへへ", 0, 6, 1);
  x3("(あ+|い?)", "いいいああ", 0, 2, 1);
  x3("(あいう)?", "あいう", 0, 6, 1);
  x3("(あいう)*", "あいう", 0, 6, 1);
  x3("(あいう)+", "あいう", 0, 6, 1);
  x3("(さしす|あいう)+", "あいう", 0, 6, 1);
  x3("([なにぬ][かきく]|かきく)+", "かきく", 0, 6, 1);
  x3("((?i:あいう))", "あいう", 0, 6, 1);
  x3("((?m:あ.う))", "あ\nう", 0, 5, 1);
  x3("((?=あん)あ)", "あんい", 0, 2, 1);
  x3("あいう|(.あいえ)", "んあいえ", 0, 8, 1);
  x3("あ*(.)", "ああああん", 8, 10, 1);
  x3("あ*?(.)", "ああああん", 0, 2, 1);
  x3("あ*?(ん)", "ああああん", 8, 10, 1);
  x3("[いうえ]あ*(.)", "えああああん", 10, 12, 1);
  x3("(\\Aいい)うう", "いいうう", 0, 4, 1);
  n("(\\Aいい)うう", "んいいうう");
  x3("(^いい)うう", "いいうう", 0, 4, 1);
  n("(^いい)うう", "んいいうう");
  x3("ろろ(るる$)", "ろろるる", 4, 8, 1);
  n("ろろ(るる$)", "ろろるるる");
  x2("(無)\\1", "無無", 0, 4);
  n("(無)\\1", "無武");
  x2("(空?)\\1", "空空", 0, 4);
  x2("(空?\?)\\1", "空空", 0, 0);
  x2("(空*)\\1", "空空空空空", 0, 8);
  x3("(空*)\\1", "空空空空空", 0, 4, 1);
  x2("あ(い*)\\1", "あいいいい", 0, 10);
  x2("あ(い*)\\1", "あい", 0, 2);
  x2("(あ*)(い*)\\1\\2", "あああいいあああいい", 0, 20);
  x2("(あ*)(い*)\\2", "あああいいいい", 0, 14);
  x3("(あ*)(い*)\\2", "あああいいいい", 6, 10, 2);
  x2("(((((((ぽ*)ぺ))))))ぴ\\7", "ぽぽぽぺぴぽぽぽ", 0, 16);
  x3("(((((((ぽ*)ぺ))))))ぴ\\7", "ぽぽぽぺぴぽぽぽ", 0, 6, 7);
  x2("(は)(ひ)(ふ)\\2\\1\\3", "はひふひはふ", 0, 12);
  x2("([き-け])\\1", "くく", 0, 4);
  x2("(?u)(\\w\\d\\s)\\1", "あ5 あ5 ", 0, 8);
  n("(?u)(\\w\\d\\s)\\1", "あ5 あ5");
  x2("(誰？|[あ-う]{3})\\1", "誰？誰？", 0, 8);
  x2("...(誰？|[あ-う]{3})\\1", "あaあ誰？誰？", 0, 13);
  x2("(誰？|[あ-う]{3})\\1", "ういうういう", 0, 12);
  x2("(^こ)\\1", "ここ", 0, 4);
  n("(^む)\\1", "めむむ");
  n("(あ$)\\1", "ああ");
  n("(あい\\Z)\\1", "あい");
  x2("(あ*\\Z)\\1", "あ", 2, 2);
  x2(".(あ*\\Z)\\1", "いあ", 2, 4);
  x3("(.(やいゆ)\\2)", "zやいゆやいゆ", 0, 13, 1);
  x3("(.(..\\d.)\\2)", "あ12341234", 0, 10, 1);
  x2("((?i:あvず))\\1", "あvずあvず", 0, 10);
  x2("(?<愚か>変|\\(\\g<愚か>\\))", "((((((変))))))", 0, 14);
  x2("\\A(?:\\g<阿_1>|\\g<云_2>|\\z終了  (?<阿_1>観|自\\g<云_2>自)(?<云_2>在|菩薩\\g<阿_1>菩薩))$", "菩薩自菩薩自在自菩薩自菩薩", 0, 26);
  x2("[[ひふ]]", "ふ", 0, 2);
  x2("[[いおう]か]", "か", 0, 2);
  n("[[^あ]]", "あ");
  n("[^[あ]]", "あ");
  x2("[^[^あ]]", "あ", 0, 2);
  x2("[[かきく]&&きく]", "く", 0, 2);
  n("[[かきく]&&きく]", "か");
  n("[[かきく]&&きく]", "け");
  x2("[あ-ん&&い-を&&う-ゑ]", "ゑ", 0, 2);
  n("[^あ-ん&&い-を&&う-ゑ]", "ゑ");
  x2("[[^あ&&あ]&&あ-ん]", "い", 0, 2);
  n("[[^あ&&あ]&&あ-ん]", "あ");
  x2("[[^あ-ん&&いうえお]&&[^う-か]]", "き", 0, 2);
  n("[[^あ-ん&&いうえお]&&[^う-か]]", "い");
  x2("[^[^あいう]&&[^うえお]]", "う", 0, 2);
  x2("[^[^あいう]&&[^うえお]]", "え", 0, 2);
  n("[^[^あいう]&&[^うえお]]", "か");
  x2("[あ-&&-あ]", "-", 0, 1);
  x2("[^[^a-zあいう]&&[^bcdefgうえお]q-w]", "え", 0, 2);
  x2("[^[^a-zあいう]&&[^bcdefgうえお]g-w]", "f", 0, 1);
  x2("[^[^a-zあいう]&&[^bcdefgうえお]g-w]", "g", 0, 1);
  n("[^[^a-zあいう]&&[^bcdefgうえお]g-w]", "2");
  x2("a<b>バージョンのダウンロード<\\/b>", "a<b>バージョンのダウンロード</b>", 0, 32);
  x2(".<b>バージョンのダウンロード<\\/b>", "a<b>バージョンのダウンロード</b>", 0, 32);
  fprintf(stdout,
       "\nRESULT   SUCC: %d,  FAIL: %d,  ERROR: %d      (by Onigmo %s)\n",
       nsucc, nfail, nerror, onig_version());

#ifndef POSIX_TEST
  onig_region_free(region, 1);
  onig_end();
#endif

  return ((nfail == 0 && nerror == 0) ? 0 : -1);
}
