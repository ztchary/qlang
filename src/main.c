#include <stdio.h>
#include <stdlib.h>

struct ArrayList {
  void *ps;
  int cap;
  int len;
};

void appendArrayList(struct ArrayList *al, void *pointer) {
  if (al->len == al->cap) {
    al->cap *= 2;
    al->ps = realloc(al->ps, al->cap);
  }
  al->ps[as->len++] = pointer;
}

void *getArrayList(struct ArrayList *al, int i) {
  if (i < 0 || i > al->len) {
    return NULL;
  }
  return al->ps[i];
}

void freeArrayList(struct ArrayList *al) {
  for (int i = 0; i < al->len; i++) {
    void *pt = getArrayList(al, i);
    if (pt != NULL) {
      free(pt);
    }
  }
  free(al);
}

enum TokenType {
  KEYWORD,
  IDENT,
  TYPE,
  OPAREN,
  CPAREN,
  OBRACK,
  CBRACK,
  OBRACE,
  CBRACE,
  SEMI,
  COLON,
  OPERATOR,
  INTLIT,
  DOT
};

enum OperatorType {
  ASGN,    // =

  ADD,    // +
  SUB,     // -
  MUL,     // *
  DIV,     // /

  ADDEQ,  // +=
  SUBEQ,   // -=
  MULEQ,   // *=
  DIVEQ,   // /=

  BNOT,    // ~
  BOR,     // |
  BAND,    // &
  BXOR,    // ^

  BOREQ,   // |=
  BANDEQ,  // &=
  BXOREQ,  // ^=

  NOT,     // !
  OR,      // ||
  AND,     // &&
  XOR,     // ^^

  OREQ,    // ||=
  ANDEQ,   // &&=
  XOREQ,   // ^^=

  TERN,    // ? :

  IFNUL,   // ??

  IFNULEQ, // ??=

  EQUAL,   // ==
  NOTEQ,   // !=
  GT,      // >
  LT,      // <
  GTEQ,    // >=
  LTEQ,    // <=

  INC,     // ++
  DEC,     // --
  NOTNOT,  // !!

  DOTDOT   // ..
};

enum Keyword {
  FUNC,
  RETURN,
  ASM,
  IF,
  ELSE,
  NUL,
  FOR,
  WHILE,
  LOOP,
  BREAK,
  CONTINUE,
  SWITCH,
  STRUCT,
  MUT,

};

enum Type {
  VOID,
  U8,
  U16,
  U32,
  U64,
  I8,
  I16,
  I32,
  I64
};

struct Token {
  enum TokenType type;
  int intval;
  void *val;
  enum OperatorType op;
  enum Keyword kw;
  enum Type type;
};

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "No input files\n");
    return 1;
  }
  
  char *fname = argv[1];

  FILE *fp = fopen(fname, "r");

  if (fp == NULL) {
    fprintf(stderr, "Failed to open %s", fname);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  int len = ftell(fp);
  rewind(fp);

  char *src = malloc(len + 1);

  fread(src, len, 1, fp);

  printf("%s", src);
  fclose(fp);
  free(src);
}

struct ArrayList tokenize(char *src, int len) {
  struct ArrayList tokens = {malloc(5 * sizeof(void *)), 5, 0};
  int line = 1;
  for (int i = 0; i < len; i++) {
    struct Token *token = malloc(sizeof(struct Token));
    switch (src[i]) {
    case '\n':
      line++;
      break;
    case ';':
      token->type = SEMI;
      break;
    case ':':
      token->type = COLON;
      break;
    case '(':
      token->type = OPAREN;
      break;
    case ')':
      token->type = CPAREN;
      break;
    case '[':
      token->type = OBRACK;
      break;
    case ']':
      token->type = CBRACK;
      break;
    case '{':
      token->type = OBRACE;
      break;
    case '}':
      token->type = CBRACE;
      break;
    case '.':
      if (src[i+1] == '.') {
        token->type = OPERATOR;
        token->op = DOTDOT;
        i++;
      } else {
        token->type = DOT;
      }
      break;


    case '=':
      token->type = OPERATOR;
      if (src[i+1] == '=') {
        token->op = EQUAL;
        i++;
      } else {
        token->op = ASGN;
      }
      break;

    case '!':
      token->type = OPERATOR;
      switch (src[++i]) {
      case '=':
        token->op = NOTEQ;
        break;
      case '!':
        token->op = NOTNOT;
        break;
      default:
        token->op = ASGN;
        i--;
      }
      break;

    case '+':
      token->type = OPERATOR;
      if (src[++i] == '=') {
        token->op = ADDEQ;
      } else if (src[i] == '+') {
        token->op = INC;
      } else {
        token->op = ADD;
        i--;
      }
      break;

    case '-':
      token->type = OPERATOR;
      if (src[++i] == '=') {
        token->op = SUBEQ;
      } else if (src[i] == '+') {
        token->op = DEC;
      } else {
        token->op = SUB;
        i--;
      }
      break;

    case '*':
      token->type = OPERATOR;
      if (src[i+1] == '=') {
        token->op = MULEQ;
        i++;
      } else {
        token->op = MUL;
      }
      break;

    case '/':
      token->type = OPERATOR;
      if (src[i+1] == '=') {
        token->op = DIVEQ;
        i++;
      } else {
        token->op = DIV;
      }
      break;

    case '~':
      token->type = OPERATOR;
      token->op = BNOT;
      break;

    case '|':
      token->type = OPERATOR;
      switch (src[++i]) {
      case '|':
        if (src[++i] == '=') {
          token->op = OREQ;
        } else {
          i--;
          token->op = OR;
        }
        break;
      case '=':
        token->op = BOREQ;
      default:
        token->op = BOR;
        i--;
      }
      break;

    case '&':
      token->type = OPERATOR;
      switch (src[++i]) {
      case '&':
        if (src[++i] == '=') {
          token->op = ANDEQ;
        } else {
          i--;
          token->op = AND;
        }
        break;
      case '=':
        token->op = BANDEQ;
      default:
        token->op = BAND;
        i--;
      }
      break;

    case '^':
      token->type = OPERATOR;
      switch (src[++i]) {
      case '^':
        if (src[++i] == '=') {
          token->op = XOREQ;
        } else {
          i--;
          token->op = XOR;
        }
        break;
      case '=':
        token->op = BXOREQ;
      default:
        token->op = BXOR;
        i--;
      }
      break;

    case '?':
      token->type = OPERATOR;
      if (src[++i] == '?' && src[++i] == '=') {
        token->op = IFNULEQ;
      } else if (src[--i] == '?') {
        token->op = IFNUL;
      } else {
        token->op = TERN;
        i--;
      }
      break;

    

    case '0':
      token->type = INTLIT;
      token->intval = 0;
      switch (src[++i]) {
      case 'x':
        i++;
        while ((src[i] >= '0' && src[i] <= '9') || (src[i] >= 'a' && src[i] <= 'f')) {
          char c = src[i++];
          c -= '0';
          if (c > '9') {
            c = c - 'a' + 10;
          }
          token->intval <<= 4;
          token->intval += c;
        }
        break;
      case 'b':
        i++;
        while (src[i] == '0' || src[i] <= '1') {
          token->intval <<= 1;
          token->intval += src[i++] - '0';
        }
        break;
      case 'o':
        i++;
        while (src[i] >= '0' && src[i] <= '7') {
          token->intval <<= 3;
          token->intval += src[i++] - '0';
        }
        break;
      case 'd':
        i++;
        while (src[i] >= '0' && src[i] <= '9') {
          token->intval *= 10;
          token->intval += src[i++] - '0';
        }
        break;
      default:
        break;
      }
      i--;
    default:
      if (src[i] >= '0' && src[i] <= '9') {
        token->type = INTLIT;
        token->intval = 0;
        while (src[i] >= '0' && src[i] <= '9') {
          token->intval *= 10;
          token->intval += src[i++] - '0';
        }
      } else {
        printf("Unknown token %c", src[i]);
      }
      break;
    }
    appendArrayList(&tokens, &token);
  }
}
