/* Minimal stdio.h stub for IntelliSense on Windows (cross-compiling for iOS) */
#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _IO_FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int fprintf(FILE *stream, const char *format, ...);
int printf(const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int vfprintf(FILE *stream, const char *format, __builtin_va_list arg);
int vprintf(const char *format, __builtin_va_list arg);
int vsprintf(char *str, const char *format, __builtin_va_list arg);
int vsnprintf(char *str, size_t size, const char *format, __builtin_va_list arg);

int fgetc(FILE *stream);
int getc(FILE *stream);
int getchar(void);
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
char *fgets(char *s, int n, FILE *stream);
int fputs(const char *s, FILE *stream);
int puts(const char *s);
int ungetc(int c, FILE *stream);

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
int fsetpos(FILE *stream, const fpos_t *pos);

int fclose(FILE *stream);
FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fflush(FILE *stream);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
void setbuf(FILE *stream, char *buf);

int remove(const char *filename);
int rename(const char *old, const char *new);
FILE *tmpfile(void);
char *tmpnam(char *s);

int feof(FILE *stream);
int ferror(FILE *stream);
void clearerr(FILE *stream);
int fileno(FILE *stream);

#ifdef __cplusplus
}
#endif

#endif /* _STDIO_H */