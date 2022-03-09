#include <stdio.h>

/*
Copyright (c) 2021 Devine Lu Linvega

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

#define TRIM 0x0100
#define LENGTH 0x10000

typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned short Uint16;

typedef struct {
	char name[64], items[64][64];
	Uint8 len;
} Macro;

typedef struct {
	char name[64];
	Uint16 addr, refs;
} Label;

typedef struct {
	char name[64], rune;
	Uint16 addr;
} Reference;

typedef struct {
	Uint8 data[LENGTH];
	Uint16 ptr, length, llen, mlen, rlen;
	Label labels[512];
	Macro macros[256];
	Reference refs[2048];
	char scope[64];
} Program;

Program p;
static int litlast = 0;

/* clang-format off */

static char ops[][4] = {
	"LIT", "INC", "POP", "DUP", "NIP", "SWP", "OVR", "ROT",
	"EQU", "NEQ", "GTH", "LTH", "JMP", "JCN", "JSR", "STH",
	"LDZ", "STZ", "LDR", "STR", "LDA", "STA", "DEI", "DEO",
	"ADD", "SUB", "MUL", "DIV", "AND", "ORA", "EOR", "SFT"
};

static int   scmp(char *a, char *b, int len) { int i = 0; while(a[i] == b[i]) if(!a[i] || ++i >= len) return 1; return 0; } /* string compare */
static int   sihx(char *s) { int i = 0; char c; while((c = s[i++])) if(!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f')) return 0; return i > 1; } /* string is hexadecimal */
static int   shex(char *s) { int n = 0, i = 0; char c; while((c = s[i++])) if(c >= '0' && c <= '9') n = n * 16 + (c - '0'); else if(c >= 'a' && c <= 'f') n = n * 16 + 10 + (c - 'a'); return n; } /* string to num */
static int   slen(char *s) { int i = 0; while(s[i]) ++i; return i; } /* string length */
static char *scpy(char *src, char *dst, int len) { int i = 0; while((dst[i] = src[i]) && i < len - 2) i++; dst[i + 1] = '\0'; return dst; } /* string copy */
static char *scat(char *dst, const char *src) { char *ptr = dst + slen(dst); while(*src) *ptr++ = *src++; *ptr = '\0'; return dst; } /* string cat */

/* clang-format on */

static int parse(char *w, FILE *f);

static int
error(const char *name, const char *msg)
{
	fprintf(stderr, "%s: %s\n", name, msg);
	return 0;
}

static char *
sublabel(char *src, char *scope, char *name)
{
	return scat(scat(scpy(scope, src, 64), "/"), name);
}

static Macro *
findmacro(char *name)
{
	int i;
	for(i = 0; i < p.mlen; ++i)
		if(scmp(p.macros[i].name, name, 64))
			return &p.macros[i];
	return NULL;
}

static Label *
findlabel(char *name)
{
	int i;
	for(i = 0; i < p.llen; ++i)
		if(scmp(p.labels[i].name, name, 64))
			return &p.labels[i];
	return NULL;
}

static Uint8
findopcode(char *s)
{
	int i;
	for(i = 0; i < 0x20; ++i) {
		int m = 0;
		if(!scmp(ops[i], s, 3))
			continue;
		if(!i) i |= (1 << 7); /* force keep for LIT */
		while(s[3 + m]) {
			if(s[3 + m] == '2')
				i |= (1 << 5); /* mode: short */
			else if(s[3 + m] == 'r')
				i |= (1 << 6); /* mode: return */
			else if(s[3 + m] == 'k')
				i |= (1 << 7); /* mode: keep */
			else
				return 0; /* failed to match */
			m++;
		}
		return i;
	}
	return 0;
}

static int
makemacro(char *name, FILE *f)
{
	Macro *m;
	char word[64];
	if(findmacro(name))
		return error("Macro duplicate", name);
	if(sihx(name) && slen(name) % 2 == 0)
		return error("Macro name is hex number", name);
	if(findopcode(name) || scmp(name, "BRK", 4) || !slen(name))
		return error("Macro name is invalid", name);
	if(p.mlen == 256)
		return error("Macros limit exceeded", name);
	m = &p.macros[p.mlen++];
	scpy(name, m->name, 64);
	while(fscanf(f, "%63s", word) == 1) {
		if(word[0] == '{') continue;
		if(word[0] == '}') break;
		if(word[0] == '%')
			return error("Macro error", name);
		if(m->len >= 64)
			return error("Macro size exceeded", name);
		scpy(word, m->items[m->len++], 64);
	}
	return 1;
}

static int
makelabel(char *name)
{
	Label *l;
	if(findlabel(name))
		return error("Label duplicate", name);
	if(sihx(name) && slen(name) % 2 == 0)
		return error("Label name is hex number", name);
	if(findopcode(name) || scmp(name, "BRK", 4) || !slen(name))
		return error("Label name is invalid", name);
	if(p.llen == 512)
		return error("Labels limit exceeded", name);
	l = &p.labels[p.llen++];
	l->addr = p.ptr;
	l->refs = 0;
	scpy(name, l->name, 64);

	return 1;
}

static int
makereference(char *scope, char *label, Uint16 addr)
{
	char subw[64];
	Reference *r;
	if(p.rlen == 2048)
		return error("References limit exceeded", label);
	r = &p.refs[p.rlen++];
	if(label[1] == '&')
		scpy(sublabel(subw, scope, label + 2), r->name, 64);
	else
		scpy(label + 1, r->name, 64);
	r->rune = label[0];
	r->addr = addr;
	return 1;
}

static void
writebyte(Uint8 b)
{
	if(p.ptr < TRIM)
		fprintf(stderr, "-- Writing in zero-page: %02x\n", b);
	p.data[p.ptr++] = b;
	p.length = p.ptr;
	litlast = 0;
}

static void
writeshort(Uint16 s, int lit)
{
	if(lit)
		writebyte(findopcode("LIT2"));
	writebyte(s >> 8);
	writebyte(s & 0xff);
}

static void
writelitbyte(Uint8 b)
{
	if(litlast) { /* combine literals */
		Uint8 hb = p.data[p.ptr - 1];
		p.ptr -= 2;
		writeshort((hb << 8) + b, 1);
		return;
	}
	writebyte(findopcode("LIT"));
	writebyte(b);
	litlast = 1;
}

static int
doinclude(const char *filename)
{
	FILE *f;
	char w[64];
	if(!(f = fopen(filename, "r")))
		return error("Include missing", filename);
	while(fscanf(f, "%63s", w) == 1)
		if(!parse(w, f))
			return error("Unknown token", w);
	fclose(f);
	return 1;
}

static int
parse(char *w, FILE *f)
{
	int i = 0;
	char word[64], subw[64], c;
	Macro *m;
	if(slen(w) >= 63)
		return error("Invalid token", w);
	switch(w[0]) {
	case '(': /* comment */
		while(fscanf(f, "%63s", word) == 1)
			if(word[0] == ')') break;
		break;
	case '~': /* include */
		if(!doinclude(w + 1))
			return error("Invalid include", w);
		break;
	case '%': /* macro */
		if(!makemacro(w + 1, f))
			return error("Invalid macro", w);
		break;
	case '|': /* pad-absolute */
		if(!sihx(w + 1))
			return error("Invalid padding", w);
		p.ptr = shex(w + 1);
		litlast = 0;
		break;
	case '$': /* pad-relative */
		if(!sihx(w + 1))
			return error("Invalid padding", w);
		p.ptr += shex(w + 1);
		litlast = 0;
		break;
	case '@': /* label */
		if(!makelabel(w + 1))
			return error("Invalid label", w);
		scpy(w + 1, p.scope, 64);
		litlast = 0;
		break;
	case '&': /* sublabel */
		if(!makelabel(sublabel(subw, p.scope, w + 1)))
			return error("Invalid sublabel", w);
		litlast = 0;
		break;
	case '#': /* literals hex */
		if(!sihx(w + 1) || (slen(w) != 3 && slen(w) != 5))
			return error("Invalid hex literal", w);
		if(slen(w) == 3)
			writelitbyte(shex(w + 1));
		else if(slen(w) == 5)
			writeshort(shex(w + 1), 1);
		break;
	case '.': /* literal byte zero-page */
		makereference(p.scope, w, p.ptr - litlast);
		writelitbyte(0xff);
		break;
	case ',': /* literal byte relative */
		makereference(p.scope, w, p.ptr - litlast);
		writelitbyte(0xff);
		break;
	case ';': /* literal short absolute */
		makereference(p.scope, w, p.ptr);
		writeshort(0xffff, 1);
		break;
	case ':': /* raw short absolute */
		makereference(p.scope, w, p.ptr);
		writeshort(0xffff, 0);
		break;
	case '\'': /* raw char */
		writebyte((Uint8)w[1]);
		break;
	case '"': /* raw string */
		i = 0;
		while((c = w[++i]))
			writebyte(c);
		break;
	case '[': break; /* ignored */
	case ']': break; /* ignored */
	default:
		/* opcode */
		if(findopcode(w) || scmp(w, "BRK", 4))
			writebyte(findopcode(w));
		/* raw byte */
		else if(sihx(w) && slen(w) == 2)
			writebyte(shex(w));
		/* raw short */
		else if(sihx(w) && slen(w) == 4)
			writeshort(shex(w), 0);
		/* macro */
		else if((m = findmacro(w))) {
			for(i = 0; i < m->len; ++i)
				if(!parse(m->items[i], f))
					return 0;
			return 1;
		} else
			return error("Unknown token", w);
	}
	return 1;
}

static int
resolve(void)
{
	Label *l;
	int i;
	for(i = 0; i < p.rlen; ++i) {
		Reference *r = &p.refs[i];
		switch(r->rune) {
		case '.':
			if(!(l = findlabel(r->name)))
				return error("Unknown zero-page reference", r->name);
			p.data[r->addr + 1] = l->addr & 0xff;
			l->refs++;
			break;
		case ',':
			if(!(l = findlabel(r->name)))
				return error("Unknown relative reference", r->name);
			p.data[r->addr + 1] = (Sint8)(l->addr - r->addr - 3);
			if((Sint8)p.data[r->addr + 1] != (l->addr - r->addr - 3))
				return error("Relative reference is too far", r->name);
			l->refs++;
			break;
		case ';':
			if(!(l = findlabel(r->name)))
				return error("Unknown absolute reference", r->name);
			p.data[r->addr + 1] = l->addr >> 0x8;
			p.data[r->addr + 2] = l->addr & 0xff;
			l->refs++;
			break;
		case ':':
			if(!(l = findlabel(r->name)))
				return error("Unknown absolute reference", r->name);
			p.data[r->addr + 0] = l->addr >> 0x8;
			p.data[r->addr + 1] = l->addr & 0xff;
			l->refs++;
			break;
		default:
			return error("Unknown reference", r->name);
		}
	}
	return 1;
}

static int
assemble(FILE *f)
{
	char w[64];
	scpy("on-reset", p.scope, 64);
	while(fscanf(f, "%63s", w) == 1)
		if(!parse(w, f))
			return error("Unknown token", w);
	return resolve();
}

static void
review(char *filename)
{
	int i;
	for(i = 0; i < p.llen; ++i)
		if(p.labels[i].name[0] >= 'A' && p.labels[i].name[0] <= 'Z')
			continue; /* Ignore capitalized labels(devices) */
		else if(!p.labels[i].refs)
			fprintf(stderr, "-- Unused label: %s\n", p.labels[i].name);
	fprintf(stderr,
		"Assembled %s in %d bytes(%.2f%% used), %d labels, %d macros.\n",
		filename,
		p.length - TRIM,
		p.length / 652.80,
		p.llen,
		p.mlen);
}

static void
write_symtab(FILE *fp)
{
    int i;
    unsigned short sz;
    size_t pos;
    unsigned char b[2];
    const char *sym = "SYM";

    sz = 0;

    fwrite(sym, 1, 3, fp);
    fwrite(&sz, 2, 1, fp);

    for (i = 0; i < p.llen; i++) {
        Label *l;
        unsigned char len;
        l = &p.labels[i];
        len = slen(l->name);
        fwrite(&len, 1, 1, fp);
        sz += 1; /* string length (max: 64) */
        fwrite(l->name, 1, len, fp);
        sz += len; /* string bytes */

        b[0] = l->addr & 0xFF;
        b[1] = (l->addr >> 8) & 0xFF;
        fwrite(b, 1, 2, fp);
        sz += 2; /* address (short) */
    }


    b[0] = sz & 0xFF;
    b[1] = (sz >> 8) & 0xFF;

    pos = ftell(fp);
    fseek(fp, 3L, SEEK_SET);
    fwrite(b, 1, 2, fp);
    fseek(fp, pos, SEEK_SET);
}

int
uxnasm_main(int argc, char *argv[])
{
	FILE *src, *dst;
    int symtab;

	if(argc < 3)
		return !error("usage", "input.tal output.rom");

    symtab = 0;

    if (argc > 3) {
        if (argv[1][0] == '-' && argv[1][1] == 'g') symtab = 1;
        argc--;
        argv++;
    }

	if(!(src = fopen(argv[1], "r")))
		return !error("Invalid input", argv[1]);
	if(!assemble(src))
		return !error("Assembly", "Failed to assemble rom.");
	if(!(dst = fopen(argv[2], "wb")))
		return !error("Invalid Output", argv[2]);

    if (symtab) {
        write_symtab(dst);
    }

	fwrite(p.data + TRIM, p.length - TRIM, 1, dst);
	review(argv[2]);
	return 0;
}

#ifndef NO_UXNASM_MAIN
int
main(int argc, char *argv[])
{
    return uxnasm_main(argc, argv);
}
#endif
