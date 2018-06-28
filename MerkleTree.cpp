#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <vector>

using namespace std;

// Hashing functions.                                                                                                                                                                                                                                                                                                                                                
int multiplyThem(int a, int b) {
  return a*b;
}

int addThem(int a, int b) {
  return a+b;
}

class Merkle {
private:
  vector<int> values;
  int (*hasher)(int, int);

public:
  Merkle(int (*f)(int,int)) {
    this->hasher = f;
  }

  int size() {
    return values.size();
  }

  void add(int value) {
    values.push_back(value);
  }

  int root() {
    vector<int> current;

    current = getHashedParents(this->values);
    while (current.size() != 1) {
      current = getHashedParents(current);
    }
    return current[0];
  }

private:
  vector<int> getHashedParents(const vector<int> &children) {
    vector<int> result;

    for (int i=0; i < children.size(); ) {
      int a = children[i], b = children[i];
      if (++i < children.size()) {
        b = children[i++];
      }
      int hash = this->hasher(a,b);
      printf("hash(%d, %d)=>%d ", a, b, hash);
      result.push_back(hash);
    }
    printf("\n");
    return result;
  }
};

int main(int argc, char** argv) {
    Merkle merkle(multiplyThem);
    merkle.add(1);
    merkle.add(2);
    merkle.add(3);
    merkle.add(4);
    merkle.add(5);

    printf("Merkle Root = %d\n\n", merkle.root());

    merkle = Merkle(addThem);
    merkle.add(1);
    merkle.add(2);
    merkle.add(3);
    merkle.add(4);
    merkle.add(5);

    printf("Merkle Root = %d\n\n", merkle.root());
    return 0;
}
