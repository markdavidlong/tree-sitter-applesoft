// This scanner excludes keywords from identifiers.
// This is supposed to emulate the behavior of the Apple II ROM (A2ROM).
// In particular, the A2ROM tokenizes these keywords first without regard to context.
// Therefore any variable name must terminate before any keyword begins.

#include <memory.h>
#include "tree_sitter/parser.h"

// scanner-src.c is for human editing.
// scanner.c is generated by token_processor.py

enum TokenType {
  NAME, // label is carried over from legacy docs, this is an identifier
  NONE
};

typedef struct
{
  char a2token[7]; // string with the excluded token
  int matches; // how many characters have been matched; don't confuse with token matches
  int len; // length of string
  int start; // offset to first matching character
} exclusion;
void create_exclusion(exclusion *x,const char *tok,int l)
{
  x->start = 0;
  x->matches = 0;
  x->len = l;
  for (int i = 0; i < x->len; i++)
    x->a2token[i] = tok[i];
}
void reset_exclusion(exclusion *x)
{
  x->start = 0;
  x->matches = 0;
}
void next_exclusion(exclusion *x,int32_t c,int pos)
{
  if (c==' ' || x->matches==x->len)
    return;
  if (x->a2token[x->matches]==c)
    x->matches++;
  else
    x->matches = (x->a2token[0]==c);
  if (x->matches==1)
    x->start = pos;
}
void finish_exclusion(exclusion *x,int *min_start)
{
  if (x->start<*min_start && x->matches==x->len)
    *min_start = x->start;
}

typedef struct
{
  // Special handling for the AT keyword is needed to emulate A2ROM behavior.
  // AT and TO are resolved as follows: /A *TO/ = (A)(TO) ; /A *T +O/ = (AT)(O)
  // To match this behavior, we only need to worry about matching AT, since if AT
  // matches, then the subsequent false match of TO is harmless to this scanner.
  int matches; // how many characters have been matched; don't confuse with token matches
  int start; // offset to first matching character
  int end; // offset to last matching character
} at_exclusion;
void create_at_exclusion(at_exclusion *x)
{
  x->start = 0;
  x->end = 0;
  x->matches = 0;
}
void reset_at_exclusion(at_exclusion *x)
{
  x->start = 0;
  x->end = 0;
  x->matches = 0;
}
void next_at_exclusion(at_exclusion *x,int32_t c,int pos)
{
  if (x->matches==2 && c=='O' && pos==x->end+1)
    reset_at_exclusion(x);
  if (c==' ' || x->matches==2)
    return;
  if (x->matches==1 && c=='T')
  {
    x->matches = 2;
    x->end = pos;
  }
  else
    x->matches = (c=='A');
  if (x->matches==1)
    x->start = pos;
}
void finish_at_exclusion(at_exclusion *x,int *min_start)
{
  if (x->start<*min_start && x->matches==2)
    *min_start = x->start;
}

typedef struct
{
  // Code to build exclusions is auto-generated by token-processor.py
  exclusion exclusions[128];
  unsigned num_exclusions;
  at_exclusion at_excl;
  int id_end;
  bool allow_lower_case;
} Scanner;

void create_scanner(Scanner *s)
{
  s->id_end = -1;
  s->allow_lower_case = true;
  create_at_exclusion(&s->at_excl);
  s->num_exclusions = 97;
  create_exclusion(&s->exclusions[0],"END",3);
  create_exclusion(&s->exclusions[1],"FOR",3);
  create_exclusion(&s->exclusions[2],"NEXT",4);
  create_exclusion(&s->exclusions[3],"DATA",4);
  create_exclusion(&s->exclusions[4],"INPUT",5);
  create_exclusion(&s->exclusions[5],"DEL",3);
  create_exclusion(&s->exclusions[6],"DIM",3);
  create_exclusion(&s->exclusions[7],"READ",4);
  create_exclusion(&s->exclusions[8],"GR",2);
  create_exclusion(&s->exclusions[9],"TEXT",4);
  create_exclusion(&s->exclusions[10],"PR#",3);
  create_exclusion(&s->exclusions[11],"IN#",3);
  create_exclusion(&s->exclusions[12],"CALL",4);
  create_exclusion(&s->exclusions[13],"PLOT",4);
  create_exclusion(&s->exclusions[14],"HLIN",4);
  create_exclusion(&s->exclusions[15],"VLIN",4);
  create_exclusion(&s->exclusions[16],"HGR2",4);
  create_exclusion(&s->exclusions[17],"HGR",3);
  create_exclusion(&s->exclusions[18],"HCOLOR=",7);
  create_exclusion(&s->exclusions[19],"HPLOT",5);
  create_exclusion(&s->exclusions[20],"DRAW",4);
  create_exclusion(&s->exclusions[21],"XDRAW",5);
  create_exclusion(&s->exclusions[22],"HTAB",4);
  create_exclusion(&s->exclusions[23],"HOME",4);
  create_exclusion(&s->exclusions[24],"ROT=",4);
  create_exclusion(&s->exclusions[25],"SCALE=",6);
  create_exclusion(&s->exclusions[26],"SHLOAD",6);
  create_exclusion(&s->exclusions[27],"TRACE",5);
  create_exclusion(&s->exclusions[28],"NOTRACE",7);
  create_exclusion(&s->exclusions[29],"NORMAL",6);
  create_exclusion(&s->exclusions[30],"INVERSE",7);
  create_exclusion(&s->exclusions[31],"FLASH",5);
  create_exclusion(&s->exclusions[32],"COLOR=",6);
  create_exclusion(&s->exclusions[33],"POP",3);
  create_exclusion(&s->exclusions[34],"VTAB",4);
  create_exclusion(&s->exclusions[35],"HIMEM:",6);
  create_exclusion(&s->exclusions[36],"LOMEM:",6);
  create_exclusion(&s->exclusions[37],"ONERR",5);
  create_exclusion(&s->exclusions[38],"RESUME",6);
  create_exclusion(&s->exclusions[39],"RECALL",6);
  create_exclusion(&s->exclusions[40],"STORE",5);
  create_exclusion(&s->exclusions[41],"SPEED=",6);
  create_exclusion(&s->exclusions[42],"LET",3);
  create_exclusion(&s->exclusions[43],"GOTO",4);
  create_exclusion(&s->exclusions[44],"RUN",3);
  create_exclusion(&s->exclusions[45],"IF",2);
  create_exclusion(&s->exclusions[46],"RESTORE",7);
  create_exclusion(&s->exclusions[47],"GOSUB",5);
  create_exclusion(&s->exclusions[48],"RETURN",6);
  create_exclusion(&s->exclusions[49],"REM",3);
  create_exclusion(&s->exclusions[50],"STOP",4);
  create_exclusion(&s->exclusions[51],"ON",2);
  create_exclusion(&s->exclusions[52],"WAIT",4);
  create_exclusion(&s->exclusions[53],"LOAD",4);
  create_exclusion(&s->exclusions[54],"SAVE",4);
  create_exclusion(&s->exclusions[55],"DEF",3);
  create_exclusion(&s->exclusions[56],"POKE",4);
  create_exclusion(&s->exclusions[57],"PRINT",5);
  create_exclusion(&s->exclusions[58],"CONT",4);
  create_exclusion(&s->exclusions[59],"LIST",4);
  create_exclusion(&s->exclusions[60],"CLEAR",5);
  create_exclusion(&s->exclusions[61],"GET",3);
  create_exclusion(&s->exclusions[62],"NEW",3);
  create_exclusion(&s->exclusions[63],"TAB(",4);
  create_exclusion(&s->exclusions[64],"TO",2);
  create_exclusion(&s->exclusions[65],"FN",2);
  create_exclusion(&s->exclusions[66],"SPC(",4);
  create_exclusion(&s->exclusions[67],"THEN",4);
  create_exclusion(&s->exclusions[68],"NOT",3);
  create_exclusion(&s->exclusions[69],"STEP",4);
  create_exclusion(&s->exclusions[70],"AND",3);
  create_exclusion(&s->exclusions[71],"OR",2);
  create_exclusion(&s->exclusions[72],"SGN",3);
  create_exclusion(&s->exclusions[73],"INT",3);
  create_exclusion(&s->exclusions[74],"ABS",3);
  create_exclusion(&s->exclusions[75],"USR",3);
  create_exclusion(&s->exclusions[76],"FRE",3);
  create_exclusion(&s->exclusions[77],"SCRN(",5);
  create_exclusion(&s->exclusions[78],"PDL",3);
  create_exclusion(&s->exclusions[79],"POS",3);
  create_exclusion(&s->exclusions[80],"SQR",3);
  create_exclusion(&s->exclusions[81],"RND",3);
  create_exclusion(&s->exclusions[82],"LOG",3);
  create_exclusion(&s->exclusions[83],"EXP",3);
  create_exclusion(&s->exclusions[84],"COS",3);
  create_exclusion(&s->exclusions[85],"SIN",3);
  create_exclusion(&s->exclusions[86],"TAN",3);
  create_exclusion(&s->exclusions[87],"ATN",3);
  create_exclusion(&s->exclusions[88],"PEEK",4);
  create_exclusion(&s->exclusions[89],"LEN",3);
  create_exclusion(&s->exclusions[90],"STR$",4);
  create_exclusion(&s->exclusions[91],"VAL",3);
  create_exclusion(&s->exclusions[92],"ASC",3);
  create_exclusion(&s->exclusions[93],"CHR$",4);
  create_exclusion(&s->exclusions[94],"LEFT$",5);
  create_exclusion(&s->exclusions[95],"RIGHT$",6);
  create_exclusion(&s->exclusions[96],"MID$",4);
}

bool is_alpha(int32_t c)
{
  return c >= 65 && c <= 90;
}

bool is_alphanumeric(int32_t c)
{
  return (c >= 65 && c <= 90) || (c >= 48 && c <= 57);
}

int32_t adj_case(Scanner *s,int32_t c)
{
  return (c >= 97 && c <= 122 && s->allow_lower_case) ? c - 32 : c;
}

bool first_pass(Scanner *s,TSLexer *lexer, const bool *valid_symbols)
{
  // First pass detects all A2ROM keyword tokens, and saves the position of the
  // identifier end, or the first keyword start, whichever is smaller.

  int32_t c;  // shorthand for the current lookahead character
  int id_pos=0; // not counting spaces
  int tot_pos=0; // counting spaces
  bool identifier_matching = false;
  uint8_t *map = (uint8_t *)calloc(32,1); // bitmap of space positions
  int max_pos = 32 * 8;

  // skip leading spaces
  while (lexer->lookahead==' ')
    lexer->advance(lexer,true);

  lexer->mark_end(lexer); // force another pass
  do
  {
    c = adj_case(s,lexer->lookahead);
    // loop to accumulate keyword matches
    for (unsigned i = 0; i < s->num_exclusions; i++)
      next_exclusion(&s->exclusions[i],c,tot_pos);
    next_at_exclusion(&s->at_excl,c,tot_pos);
    identifier_matching = ((id_pos==0 && is_alpha(c)) || (id_pos>0 && is_alphanumeric(c)) || c==' ');
    if (identifier_matching)
    {
      if (c==' ')
        map[tot_pos / 8] |= 1 << (tot_pos % 8);
      else
        id_pos++;
      tot_pos++;
      lexer->advance(lexer, false);
    }
  } while (identifier_matching && tot_pos<max_pos);
  s->id_end = tot_pos;
  // loop to rewind position to left-most keyword match
  for (unsigned i = 0; i < s->num_exclusions; i++)
    finish_exclusion(&s->exclusions[i],&s->id_end);
  finish_at_exclusion(&s->at_excl,&s->id_end);
  if (s->id_end>0 && id_pos>0)
  {
    // loop to rewind position to before trailing spaces
    while (s->id_end>1 && (map[(s->id_end-1)/8] & (1 << ((s->id_end-1)%8))))
      s->id_end--;
    lexer->result_symbol = NAME;
    return true;
  }
  else
  {
    // n.b. serialize will not be called
    lexer->result_symbol = NONE;
    return false;
  }
}

bool second_pass(Scanner *s,TSLexer *lexer, const bool *valid_symbols)
{
  // Lexing is already done. We just have to set the marker.
  for (int i=0;i<s->id_end;i++)
    lexer->advance(lexer,false);
  s->id_end = -1;
  lexer->result_symbol = NAME;
  return true;
}

void *tree_sitter_applesoft_external_scanner_create() {
  void *ans = malloc(sizeof(Scanner));
  create_scanner(ans);
  return ans;
}

bool tree_sitter_applesoft_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
  Scanner *scanner = (Scanner*)payload;
  if (!valid_symbols[NAME])
  {
    lexer->result_symbol = NONE;
    return false;
  }
  if (scanner->id_end<0)
    return first_pass(scanner,lexer,valid_symbols);
  else
    return second_pass(scanner,lexer,valid_symbols);
}

unsigned tree_sitter_applesoft_external_scanner_serialize(void *payload, char *buffer) {
  Scanner *s = (Scanner*)payload;
  char *src = (char*)&s->id_end;
  for (unsigned i=0;i<sizeof(int);i++)
    buffer[i] = src[i];
  return sizeof(int);
}

void tree_sitter_applesoft_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
  Scanner *s = (Scanner*)payload;
  for (unsigned i = 0; i < s->num_exclusions; i++)
    reset_exclusion(&s->exclusions[i]);
  reset_at_exclusion(&s->at_excl);
  s->id_end = -1;
  if (length==sizeof(int))
  {
    char *dst = (char*)&s->id_end;
    for (unsigned i=0;i<sizeof(int);i++)
      dst[i] = buffer[i];
  }
}

void tree_sitter_applesoft_external_scanner_destroy(void *payload) {
  free(payload);
}
