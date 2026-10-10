#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// The token kinds, spelled exactly as they are printed.
static const char *KEYWORD = "KEYWORD", *IDENTIFIER = "IDENTIFIER", *NUMBER = "NUMBER";
static const char *STRING = "STRING", *OPERATOR = "OPERATOR", *SYMBOL = "SYMBOL";

struct Tok {
  string kind;
  string text;
};

static const set<string> KEYWORDS = {
    "SELECT", "FROM", "WHERE", "INSERT", "INTO", "VALUES", "CREATE", "TABLE", "UPDATE", "SET", "DELETE",
    "AND", "OR", "NOT", "IS", "NULL", "INTEGER", "TEXT", "BEGIN", "COMMIT", "ROLLBACK", "ORDER", "BY",
    "LIMIT", "OFFSET", "COUNT", "SUM", "AVG", "MIN", "MAX", "ASC", "DESC", "INDEX", "ON"};

static bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static string upper(string s) {
  for (size_t i = 0; i < s.size(); i++)
    if (s[i] >= 'a' && s[i] <= 'z') s[i] = (char)(s[i] - 32);
  return s;
}

// Split one line of SQL into tokens. Returns true on success. On failure returns false and puts
// the message that follows "ERR " into err.
static bool tokenize(const string &line, vector<Tok> &toks, string &err) {
  toks.clear();
  size_t n = line.size();
  size_t i = 0;
  while (i < n) {
    char c = line[i];
    if (c == ' ' || c == '\t' || c == '\r') {
      i++;
    } else if (string("(),;").find(c) != string::npos) {
      toks.push_back(Tok{SYMBOL, string(1, c)});
      i++;
    } else {
      // TODO: comments (-- to end of line), 'strings' and "quoted identifiers" with a doubled
      // quote as an escape, numbers (and the malformed ones), words (keyword or identifier),
      // and operators (two-character ones first). Anything else is an error.
      err = string("unexpected character: ") + c;
      return false;
    }
  }
  return true;
}

int main() {
  string line;
  while (getline(cin, line)) {
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
    if (line.find_first_not_of(" \t") == string::npos) continue;
    vector<Tok> toks;
    string err;
    if (!tokenize(line, toks, err)) {
      cout << "ERR " << err << "\n";
      continue;
    }
    for (size_t i = 0; i < toks.size(); i++) cout << toks[i].kind << " " << toks[i].text << "\n";
    cout << "END\n";
  }
  return 0;
}
