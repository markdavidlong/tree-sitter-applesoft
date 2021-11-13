// Overall structure copied from github.com/tree-sitter/tree-sitter-ruby
// This scanner excludes keywords from identifiers.
// This is supposed to approximate the behavior of the Apple II ROM.
// In particular, the A2ROM tokenizes these keywords first without regard to context.
// Therefore any variable name must terminate before any keyword begins.

#include <tree_sitter/parser.h>
#include <vector>
#include <string>

// scanner-src.cc is for human editing.
// scanner.cc is generated by token_processor.py

namespace
{

enum TokenType {
  NAME, // label is carried over from legacy docs, this is an identifier
  NONE
};

class exclusion
{
  char a2token[7]; // string with the excluded token
  int matches; // how many characters have been matched; don't confuse with token matches
  int len; // length of string
  int start; // offset to first matching character

public:

  exclusion(const char *tok,int l)
  {
    start = 0;
    matches = 0;
    len = l;
    for (int i=0;i<len;i++)
      a2token[i] = tok[i];
  }
  void reset()
  {
    start = 0;
    matches = 0;
  }
  void next(int32_t c,int pos)
  {
    if (c==' ' || matches==len)
      return;
    if (a2token[matches]==c)
      matches++;
    else
      matches = (a2token[0]==c);
    if (matches==1)
      start = pos;
  }
  void finish(int& min_start)
  {
    if (start<min_start && matches==len)
      min_start = start;
  }
};

struct Scanner
{
  std::vector<exclusion> exclusions;
  int id_end;

  Scanner()
  {
    id_end = -1;
    exclusions.push_back(exclusion("END",3));
    exclusions.push_back(exclusion("FOR",3));
    exclusions.push_back(exclusion("NEXT",4));
    exclusions.push_back(exclusion("DATA",4));
    exclusions.push_back(exclusion("INPUT",5));
    exclusions.push_back(exclusion("DEL",3));
    exclusions.push_back(exclusion("DIM",3));
    exclusions.push_back(exclusion("READ",4));
    exclusions.push_back(exclusion("GR",2));
    exclusions.push_back(exclusion("TEXT",4));
    exclusions.push_back(exclusion("PR#",3));
    exclusions.push_back(exclusion("IN#",3));
    exclusions.push_back(exclusion("CALL",4));
    exclusions.push_back(exclusion("PLOT",4));
    exclusions.push_back(exclusion("HLIN",4));
    exclusions.push_back(exclusion("VLIN",4));
    exclusions.push_back(exclusion("HGR2",4));
    exclusions.push_back(exclusion("HGR",3));
    exclusions.push_back(exclusion("HCOLOR=",7));
    exclusions.push_back(exclusion("HPLOT",5));
    exclusions.push_back(exclusion("DRAW",4));
    exclusions.push_back(exclusion("XDRAW",5));
    exclusions.push_back(exclusion("HTAB",4));
    exclusions.push_back(exclusion("HOME",4));
    exclusions.push_back(exclusion("ROT=",4));
    exclusions.push_back(exclusion("SCALE=",6));
    exclusions.push_back(exclusion("SHLOAD",6));
    exclusions.push_back(exclusion("TRACE",5));
    exclusions.push_back(exclusion("NOTRACE",7));
    exclusions.push_back(exclusion("NORMAL",6));
    exclusions.push_back(exclusion("INVERSE",7));
    exclusions.push_back(exclusion("FLASH",5));
    exclusions.push_back(exclusion("COLOR=",6));
    exclusions.push_back(exclusion("POP",3));
    exclusions.push_back(exclusion("VTAB",4));
    exclusions.push_back(exclusion("HIMEM:",6));
    exclusions.push_back(exclusion("LOMEM:",6));
    exclusions.push_back(exclusion("ONERR",5));
    exclusions.push_back(exclusion("RESUME",6));
    exclusions.push_back(exclusion("RECALL",6));
    exclusions.push_back(exclusion("STORE",5));
    exclusions.push_back(exclusion("SPEED=",6));
    exclusions.push_back(exclusion("LET",3));
    exclusions.push_back(exclusion("GOTO",4));
    exclusions.push_back(exclusion("RUN",3));
    exclusions.push_back(exclusion("IF",2));
    exclusions.push_back(exclusion("RESTORE",7));
    exclusions.push_back(exclusion("GOSUB",5));
    exclusions.push_back(exclusion("RETURN",6));
    exclusions.push_back(exclusion("REM",3));
    exclusions.push_back(exclusion("STOP",4));
    exclusions.push_back(exclusion("ON",2));
    exclusions.push_back(exclusion("WAIT",4));
    exclusions.push_back(exclusion("LOAD",4));
    exclusions.push_back(exclusion("SAVE",4));
    exclusions.push_back(exclusion("DEF",3));
    exclusions.push_back(exclusion("POKE",4));
    exclusions.push_back(exclusion("PRINT",5));
    exclusions.push_back(exclusion("CONT",4));
    exclusions.push_back(exclusion("LIST",4));
    exclusions.push_back(exclusion("CLEAR",5));
    exclusions.push_back(exclusion("GET",3));
    exclusions.push_back(exclusion("NEW",3));
    exclusions.push_back(exclusion("TAB(",4));
    exclusions.push_back(exclusion("TO",2));
    exclusions.push_back(exclusion("FN",2));
    exclusions.push_back(exclusion("SPC(",4));
    exclusions.push_back(exclusion("THEN",4));
    exclusions.push_back(exclusion("AT",2));
    exclusions.push_back(exclusion("NOT",3));
    exclusions.push_back(exclusion("STEP",4));
    exclusions.push_back(exclusion("AND",3));
    exclusions.push_back(exclusion("OR",2));
    exclusions.push_back(exclusion("SGN",3));
    exclusions.push_back(exclusion("INT",3));
    exclusions.push_back(exclusion("ABS",3));
    exclusions.push_back(exclusion("USR",3));
    exclusions.push_back(exclusion("FRE",3));
    exclusions.push_back(exclusion("SCRN(",5));
    exclusions.push_back(exclusion("PDL",3));
    exclusions.push_back(exclusion("POS",3));
    exclusions.push_back(exclusion("SQR",3));
    exclusions.push_back(exclusion("RND",3));
    exclusions.push_back(exclusion("LOG",3));
    exclusions.push_back(exclusion("EXP",3));
    exclusions.push_back(exclusion("COS",3));
    exclusions.push_back(exclusion("SIN",3));
    exclusions.push_back(exclusion("TAN",3));
    exclusions.push_back(exclusion("ATN",3));
    exclusions.push_back(exclusion("PEEK",4));
    exclusions.push_back(exclusion("LEN",3));
    exclusions.push_back(exclusion("STR$",4));
    exclusions.push_back(exclusion("VAL",3));
    exclusions.push_back(exclusion("ASC",3));
    exclusions.push_back(exclusion("CHR$",4));
    exclusions.push_back(exclusion("LEFT$",5));
    exclusions.push_back(exclusion("RIGHT$",6));
    exclusions.push_back(exclusion("MID$",4));

  }

  bool is_alpha(int32_t c)
  {
    return c >= 65 && c <= 90;
  }

  bool is_alphanumeric(int32_t c)
  {
    return c >= 65 && c <= 90 || c >= 48 && c <= 57;
  }

  unsigned serialize(char *buffer)
  {
    char *src = (char*)&id_end;
    for (int i=0;i<sizeof(id_end);i++)
      buffer[i] = src[i];
    return sizeof(id_end);
  }

  void deserialize(const char *buffer, unsigned length)
  {
    for (auto & excl : exclusions)
      excl.reset();
    id_end = -1;
    if (length==sizeof(id_end))
    {
      char *dst = (char*)&id_end;
      for (int i=0;i<sizeof(id_end);i++)
        dst[i] = buffer[i];
    }
  }

  bool first_pass(TSLexer *lexer, const bool *valid_symbols)
  {
    // First pass detects all A2ROM keyword tokens, and saves the position of the
    // identifier end, or the first keyword start, whichever is smaller.

    int32_t c;  // shorthand for the current lookahead character
    int id_pos=0; // not counting spaces
    int tot_pos=0; // counting spaces
    bool identifier_matching = false;

    // skip leading spaces
    while (lexer->lookahead==' ')
      lexer->advance(lexer,true);

    lexer->mark_end(lexer); // force another pass
    do
    {
      c = lexer->lookahead;
      // loop to accumulate keyword matches
      for (auto & excl : exclusions)
        excl.next(c,tot_pos);
      identifier_matching = (id_pos==0 && is_alpha(c) || id_pos>0 && is_alphanumeric(c) || c==' ');
      if (identifier_matching)
      {
        lexer->advance(lexer,false);
        tot_pos++;
        id_pos += (c!=' ');
      }
    } while(identifier_matching);
    id_end = tot_pos;
    // loop to rewind position to left-most keyword match
    for (auto & excl : exclusions)
      excl.finish(id_end);
    if (id_end>0 && id_pos>0)
    {
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

  bool second_pass(TSLexer *lexer, const bool *valid_symbols)
  {
    // Lexing is already done. We just have to set the marker.
    for (int i=0;i<id_end;i++)
      lexer->advance(lexer,false);
    id_end = -1;
    lexer->result_symbol = NAME;
    return true;
  }

  bool scan(TSLexer *lexer, const bool *valid_symbols)
  {
    if (!valid_symbols[NAME])
    {
      lexer->result_symbol = NONE;
      return false;
    }
    if (id_end<0)
      return first_pass(lexer,valid_symbols);
    else
      return second_pass(lexer,valid_symbols);
  }
};

}

extern "C" {

void *tree_sitter_applesoft_external_scanner_create() {
  return new Scanner();
}

bool tree_sitter_applesoft_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  return scanner->scan(lexer, valid_symbols);
}

unsigned tree_sitter_applesoft_external_scanner_serialize(void *payload, char *buffer) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  return scanner->serialize(buffer);
}

void tree_sitter_applesoft_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  scanner->deserialize(buffer, length);
}

void tree_sitter_applesoft_external_scanner_destroy(void *payload) {
  Scanner *scanner = static_cast<Scanner *>(payload);
  delete scanner;
}

}
