#include <stdio.h>

using TChar = unsigned long;

class TNode {
    TChar c;
    TNode *next;
    TNode (TChar a, TNode *oldTop): c{a}, next{oldTop} {}
    friend class TStack;
};

class TStack {
private:
    TNode *top;
public:
    TStack(): top{nullptr} {}
    ~TStack() {
        while ( top != nullptr ) {
            pop();
        }
    }
    void push( TChar a ) {
        top = new TNode( a, top );
    }
    TChar pop() {
        if ( !empty() ) {
            TNode *tmp = top;
            top = tmp->next;
            TChar res = tmp->c;
            delete tmp;
            return res;
        }
        return 0;
    }
    bool empty() {
        return top == nullptr;
    }
};

int min( int a, int b ) {
    if ( a < b ) {
        return a;
    }
    return b;
}


int boyerMoore( const int* L, const int* l, const int n, const int i ) {
    if ( i == n - 1 ) {
        return 1;
    } else if ( L[i+1] > 0 ) {
        return n - L[i+1];
    } else {
        return n - l[i+1];
    }
}


char saveChar = '\0';

bool nextText( int &lineNum, int &charNum, TChar &v ) {
    char c;
    bool hasNumber = false;
    v = 0;

    if ( saveChar == EOF ) {
        return false;
    }
    if ( saveChar == '\n' ) {
        lineNum++;
        charNum = 0;
    }

    while ( true ) {
        c = getchar();
        if ( c >= '0' && c <= '9' ) {
            if ( !hasNumber ) {
                hasNumber = true;
                charNum++;
            }
            v = v*10 + ( c - '0' );
        } else {
            if ( hasNumber ) {
                saveChar = c;
                return true;
            }
            if ( c == EOF ) {
                return false;
            }
            if ( c == '\n' ) {
                lineNum++;
                charNum = 0;
            }
        }
    }

}

int main() {
    TStack *patData = new TStack();
    int n;
    TChar *pat, *txt, v;
    char c;
    n = 0;
    while ( scanf( "%lu%c", &v, &c ) == 2 ) {
        patData->push( v );
        n++;
        if ( c == '\n' ) {
            break;
        }
    }

    pat = new TChar[n];
    for ( int i=n-1; i>=0; i-- ) {
        pat[i] = patData->pop();
    }
    delete patData;

    TChar *s = new TChar[n];
    for ( int i=0; i<n; i++ ) { s[i] = pat[n-1-i]; }
    int *z = new int[n];
    for ( int i=0; i<n; i++ ) { z[i] = 0; }
    for (int i=1, l=0, r=0; i<n; ++i) {
		if (i <= r) {
			z[i] = min (r-i+1, z[i-l]);
		}
		while (i+z[i] < n && s[z[i]] == s[i+z[i]]) {
			++z[i];
		}
		if (i+z[i]-1 > r) {
			l = i,  r = i+z[i]-1;
		}
	}
    delete[] s;


    int *N = new int[n];
    for ( int i=0; i<n; i++ ) { N[i] = z[n-1-i]; }
    delete[] z;

    int *L = new int[n];
    for ( int i = 0; i < n; i++ ) {
        L[i] = 0;
    }
    for ( int i = 0; i < n - 1; i++ ) {
        if ( N[i] > 0 ) {
            int j = n - N[i];
            L[j] = i + 1;
        }
    }
    int *l = new int[n];
    l[0] = 0;
    l[n-1] = N[0];
    for ( int i = n - 2; i > 0; i-- ) {
        l[i] = l[i+1];
        int j = n - 1 - i;
        if ( N[j] == j + 1 ) {
            l[i] = N[j];
        }
    }

    txt = new TChar[n];
    int *M = new int[n];
    int *lineNums = new int[n];
    int *charNums = new int[n];


    int lineNum = 1;
    int charNum = 0;
    int k = n, idx = 0;

    while ( nextText( lineNum, charNum, v ) ) {
        txt[idx] = v;
        lineNums[idx] = lineNum;
        charNums[idx] = charNum;
        M[idx] = 0;
        k--;
        if ( k == 0 ) {

            int h = idx, i = n - 1;
            while ( k == 0 ) {
                if ( M[h] == 0 ) {
                    if ( txt[h] == pat[i] ) {
                        if ( i == 0 ) {
                            int start = ( idx + 1 ) % n;
                            printf( "%d, %d\n", lineNums[start], charNums[start] );
                            M[idx] = n;
                            if ( n > 1 ) {
                                k = n - l[1];
                            } else {
                                k = 1;
                            }
                        } else {
                            i--;
                            h--;
                            if ( h < 0 ) {
                                h += n;
                            }
                        }
                    } else {
                        M[idx] = idx - h;
                        if ( M[idx] < 0 ) {
                            M[idx] += n;
                        }
                        k = boyerMoore( L, l, n, i );
                    }
                } else if ( M[h] >= N[i] && N[i] == i + 1 && N[i] > 0 ) {
                    int start = ( idx + 1 ) % n;
                    printf( "%d, %d\n", lineNums[start], charNums[start] );
                    M[idx] = idx - h;
                    if ( M[idx] < 0 ) {
                        M[idx] += n;
                    }
                    if ( n > 1 ) {
                        k = n - l[1];
                    } else {
                        k = 1;
                    }
                } else if ( M[h] > N[i] && N[i] < i + 1 ) {
                    M[idx] = idx - h;
                    if ( M[idx] < 0 ) {
                        M[idx] += n;
                    }
                    k = boyerMoore( L, l, n, i - N[i] );
                } else  {
                    i -= M[h];
                    h -= M[h];
                    if ( h < 0 ) {
                        h += n;
                    }
                }
            }
        }
        idx = ( idx + 1 ) % n;
    }

    delete [] pat;
    delete [] txt;
    delete [] L;
    delete [] l;
    delete [] N;
    delete [] M;
    delete [] lineNums;
    delete [] charNums;
    return 0;
}
