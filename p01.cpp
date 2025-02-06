#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <vector>

using namespace std;

bool evalOperator(const string &op, bool x, bool y) {
  if (op == "&") return x && y;
  if (op == "|") return x || y;
  if (op == "^") return x != y;
  if (op == "->") return !x || y;
  if (op == "<->") return x == y;
  return false;
}

bool computePostfix(const vector<string> &expr, map<char, bool> &values) {
  stack<bool> s;
  for (const string &tok : expr) {
    if (isalpha(tok[0])) s.push(values[tok[0]]);
    else if (tok == "!") { bool a = s.top(); s.pop(); s.push(!a); }
    else { bool b = s.top(); s.pop(); bool a = s.top(); s.pop(); s.push(evalOperator(tok, a, b)); }
  }
  return s.top();
}

int getPriority(const string &op) {
  if (op == "!") return 4;
  if (op == "&") return 3;
  if (op == "^") return 2;
  if (op == "|") return 1;
  if (op == "->" || op == "<->") return 0;
  return -1;
}

vector<string> tokenize(string input) {
  vector<string> tokens;
  for (int i = 0; i < input.length(); ++i) {
    if (isalpha(input[i])) tokens.push_back(string(1, input[i]));
    else if (input[i] == '!' || input[i] == '&' || input[i] == '|' || input[i] == '^') tokens.push_back(string(1, input[i]));
    else if (input[i] == '-' && input[i + 1] == '>') { tokens.push_back("->"); i++; }
    else if (input[i] == '<' && input[i + 2] == '>') { tokens.push_back("<->"); i += 2; }
    else if (input[i] == '(' || input[i] == ')') tokens.push_back(string(1, input[i]));
  }
  return tokens;
}

vector<string> convertToPostfix(vector<string> &tokens) {
  stack<string> ops;
  vector<string> postfix;
  for (const string &t : tokens) {
    if (isalpha(t[0])) postfix.push_back(t);
    else if (t == "(") ops.push(t);
    else if (t == ")") {
      while (ops.top() != "(") { postfix.push_back(ops.top()); ops.pop(); }
      ops.pop();
    } else {
      while (!ops.empty() && getPriority(ops.top()) >= getPriority(t)) {
        postfix.push_back(ops.top());
        ops.pop();
      }
      ops.push(t);
    }
  }
  while (!ops.empty()) { postfix.push_back(ops.top()); ops.pop(); }
  return postfix;
}

void truthTable(string input) {
  map<char, bool> values;
  vector<string> tokens = tokenize(input), postfix = convertToPostfix(tokens);
  string vars;
  for (const string &t : tokens) if (isalpha(t[0]) && vars.find(t[0]) == string::npos) vars += t;

  int n = vars.size(), rows = 1 << n, trueCases = 0;

  for (char v : vars) cout << v << " ";
  cout << input << endl;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < n; ++j) {
      values[vars[j]] = (i >> (n - j - 1)) & 1;
      cout << values[vars[j]] << " ";
    }
    bool res = computePostfix(postfix, values);
    cout << res << endl;
    trueCases += res;
  }

  cout << "Expression is a " << (trueCases == rows ? "tautology" : trueCases == 0 ? "contradiction" : "contingency") 
       << " with " << trueCases << " true cases out of " << rows << ".\n";
}

int main() {
  string input;
  cout << "Valid operators: ! (not), & (and), | (or), ^ (xor), -> (implication), <-> (biconditional)\n";
  cout << "Enter a logical expression: ";
  getline(cin, input);
  truthTable(input);
  return 0;
}
