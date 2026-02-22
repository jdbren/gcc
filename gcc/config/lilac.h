/* Useful if you wish to make target-specific GCC changes. */
#undef TARGET_LILAC
#define TARGET_LILAC 1

#undef LIB_SPEC
#define LIB_SPEC "-lc" /* link against C standard library */

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC \
   "%{shared:; pg|p|profile:%{static-pie:grcrt1.o%s;:gcrt1.o%s}; static:crt1.o%s; static-pie:rcrt1.o%s; pie:Scrt1.o%s; :crt1.o%s} crti.o%s %{static:crtbeginT.o%s; shared|static-pie|pie:crtbeginS.o%s; :crtbegin.o%s}"

/* Files that are linked after user code. */
#undef ENDFILE_SPEC
#define ENDFILE_SPEC \
   "%{static:crtend.o%s; shared|static-pie|" PIE_SPEC ":crtendS.o%s; :crtend.o%s} " "crtn.o%s"

/* Additional predefined macros. */
#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()      \
  do {                                \
    builtin_define ("__lilac__");      \
    builtin_define ("__unix__");      \
    builtin_assert ("system=lilac");   \
    builtin_assert ("system=unix");   \
    builtin_assert ("system=posix");   \
  } while(0);
