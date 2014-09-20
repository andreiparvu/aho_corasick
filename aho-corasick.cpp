#include <iostream>
#include <set>
#include <queue>
#include <string>

using namespace std;

const int SIGMA_SIZE = SIGMA_SIZE;

struct Trie {
  Trie *edges[SIGMA_SIZE];
  Trie *fail;
  set<string> out;

  Trie() {
    fail = NULL;
    for (int i = 0; i < SIGMA_SIZE; i++) {
      edges[i] = NULL;
    }
  }
};
Trie *root = new Trie();

void addString(Trie *node, string &curString, int depth = 0) {
  if (depth == curString.size()) {
    node->out.insert(curString);

    return;
  }

  int next = curString[depth] - 'a';

  if (node->edges[next] == NULL || node->edges[next] == root) {
    node->edges[next] = new Trie();
  }

  addString(node->edges[next], curString, depth + 1);
}

int main() {
  // init the out edges from root with the root itself
  for (int i = 0; i < SIGMA_SIZE; i++) {
    root->edges[i] = root;
    root->fail = root;
  }

  // Number of strings
  int nrStrings;
  cin >> nrStrings;
  for (int i = 0; i < nrStrings; i++) {
    string curString;

    cin >> curString;

    addString(root, curString);
  }

  cout << "Finished constructing trie\n";
  // build the fail function
  queue<Trie*> q;

  // Must to this before, because of the fact that every edge out of the root is
  // not NULL
  for (int i = 0; i < SIGMA_SIZE; i++) {
    if (root->edges[i] != NULL && root->edges[i] != root) {
      root->edges[i]->fail = root;
      q.push(root->edges[i]);
    }
  }

  while (!q.empty()) {
    Trie *curNode = q.front();
    q.pop();

    for (int i = 0; i < SIGMA_SIZE; i++) {
      Trie *next = curNode->edges[i];
      if (next != NULL && next != root) {
        q.push(next);

        Trie *f = curNode->fail;
        for (; f->edges[i] == NULL; f = f->fail);

        next->fail = f->edges[i];

        for (auto s : next->fail->out) {
          next->out.insert(s);
        }
      }
    }
  }

  // Read big string, in which we search for elements
  string bigString;
  cin >> bigString;

  Trie *node = root;
  for (int i = 0; i < bigString.size(); i++) {
    int cur = bigString[i] - 'a';

    for (; node->edges[cur] == NULL; node = node->fail);

    node = node->edges[cur];

    if (node->out.size() != 0) {
      cout << "At position " << i << " we found:\n";

      for (auto s : node->out) {
        cout << s << "\n";
      }
    }
  }

  return 0;
}