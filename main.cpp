#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

// Group 15 - Project 1 - 2/9/2025
// Bonus Task: Below 150 lines of code

bool applyOperator(const string &op, bool a, bool b) {
  // Test the operator and return the result of the operation
  if (op == "&") return a && b;
  if (op == "|") return a || b;
  if (op == "^") return a != b;
  if (op == "->") return !a || b;
  if (op == "<->") return a == b;

  // If the operator is not recognized, return false
  return false;
}

bool evaluatePostfix(const vector<string> &postfix, map<char, bool> &values) {
  // Initialize a stack to store the values
  stack<bool> s;

  // Iterate through the postfix expression
  for (const string &token : postfix) {
    // Check if the token is a variable, negation, or operator
    if (isalpha(token[0])) s.push(values[token[0]]);
    else if (token == "!") { bool a = s.top(); s.pop(); s.push(!a); }
    else { bool b = s.top(); s.pop(); bool a = s.top(); s.pop(); s.push(applyOperator(token, a, b)); }
  }

  // Return the result of the expression
  return s.top();
}

int precedence(const string &op) {
  // Return the precedence of the operator
  if (op == "!") return 4;
  if (op == "&") return 3;
  if (op == "^") return 2;
  if (op == "|") return 1;
  if (op == "->" || op == "<->") return 0;

  // If the operator is not recognized, return -1
  return -1;
}

vector<string> tokenize(string expr) {
  // Intialize a vector to store the tokens
  vector<string> tokens;

  // Iterate through the expression and tokenize it
  for (size_t i = 0; i < expr.length(); ++i) {
    // Check if the character is a variable, operator, or parenthesis
    if (isalpha(expr[i])) tokens.push_back(string(1, expr[i]));
    else if (expr[i] == '!' || expr[i] == '&' || expr[i] == '|' || expr[i] == '^') tokens.push_back(string(1, expr[i]));
    else if (expr[i] == '-' && expr[i + 1] == '>') { tokens.push_back("->"); i++; }
    else if (expr[i] == '<' && expr[i + 2] == '>') { tokens.push_back("<->"); i += 2; }
    else if (expr[i] == '(' || expr[i] == ')') tokens.push_back(string(1, expr[i]));
  }

  // Return the tokens
  return tokens;
}

vector<string> toPostfix(vector<string> &tokens) {
  // Initialize a stack to store the operators and a vector to store the postfix expression
  stack<string> ops;
  vector<string> postfix;

  // Iterate through the tokens and convert the infix expression to postfix
  for (const string &token : tokens) {
    // Check if the token is a variable, left parenthesis, right parenthesis, or operator
    if (isalpha(token[0])) postfix.push_back(token);
    else if (token == "(") ops.push(token);
    else if (token == ")") {
      while (ops.top() != "(") {
        postfix.push_back(ops.top());
        ops.pop();
      }
      ops.pop();
    }
    else {
      while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
        postfix.push_back(ops.top());
        ops.pop();
      }
      ops.push(token);
    }
  }

  // Pop the remaining operators from the stack
  while (!ops.empty()) {
    postfix.push_back(ops.top());
    ops.pop();
  }

  // Return the postfix expression
  return postfix;
}

void generateTruthTable(string expr) {
  // Initialize a map to store the values of the variables
  map<char, bool> values;

  // Remove whitespace from the expression and tokenize it
  expr.erase(remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

  // Get the variables and the postfix expression
  vector<string> tokens = tokenize(expr), postfix = toPostfix(tokens);

  // Generate the truth table
  string vars;
  for (const string &token : tokens) if (isalpha(token[0]) && vars.find(token[0]) == string::npos) vars += token;
  int n = vars.size(), trueCases = 0, numRows = 1 << n;

  for (char var : vars) cout << var << " ";
  cout << expr << "\n";

  for (int i = 0; i < numRows; ++i) {
    for (int j = 0; j < n; ++j) {
      values[vars[j]] = i & (1 << j);
      cout << values[vars[j]] << " ";
    }
    bool result = evaluatePostfix(postfix, values);
    cout << result << "\n";
    if (result) trueCases++;
  }
  cout << (trueCases == numRows ? "The expression is a tautology.\n" : trueCases == 0 ? "The expression is a contradiction.\n" : "The expression is a contingency.\n");
}

int main() {
  string expr;
  cout << "Enter a logical expression (use &, |, ^, !, ->, <->, and parentheses):\n";
  getline(cin, expr);
  generateTruthTable(expr);
  return 0;
}
