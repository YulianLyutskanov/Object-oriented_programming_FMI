#include <iostream>
#include <fstream>
using namespace std;


// ex.1

void read3Nums() {
    int a, b, c;
    cin >> a >> b >> c;
    ofstream oFstr("result.txt");
    if (!oFstr.is_open()) {
        return;
    }
    oFstr << (a + b + c) << ' ' << a * b * c;
    oFstr.close();
}

void printDiff() {
    ifstream iFstr("result.txt");
    if (!iFstr.is_open()) {
        return;
    }
    int a, b;
    iFstr >> a >> b;
    cout << a - b;

    iFstr.close();
}



//ex.2

struct Pair {
    unsigned n1, n2;
};

constexpr unsigned  MAX_RELATION_LENGTH = 25;

struct Relation {
    unsigned curSize = 0;
    Pair pairs[MAX_RELATION_LENGTH] = {};
};



void writePairToFile(const Pair& p, ofstream& ofstr) {
    if (!ofstr.is_open())
        return;

    ofstr << '(' << p.n1 << ',' << p.n2 << ")\n";
}

void readPairFromFile(Pair& p, ifstream& ifstr) {
    if (!ifstr.is_open())
        return;

    
    ifstr.ignore();
    
    ifstr >>p.n1;
    ifstr.ignore();

    ifstr >>p.n2;
    ifstr.ignore(2);

    
}

void writeRelationToFile(const Relation& r, const char* str){
    if (!str)
        return;

    ofstream ofstr(str);

    if (!ofstr.is_open())
        return;

    for (size_t i = 0; i < r.curSize; i++)
    {
        writePairToFile(r.pairs[i], ofstr);

    }

    ofstr.close();

}


void readRelationFromFile(Relation& r, const char* str) {
    if (!str)
        return;

    ifstream ifstr(str);

    if (!ifstr.is_open())
        return;

    unsigned i = 0;
    while (i<=25) {
        Pair cur;
        readPairFromFile(cur, ifstr);
        if (ifstr.eof())
            break;
        r.pairs[i] = cur;
        i++;
    }

    r.curSize = i;
    ifstr.close();


}


//ex.3

struct Color {
    uint8_t r, g, b;
};



int main()
{
    //read3Nums();
    //printDiff();

    //Relation r = { 3,{{1,2},{4,5},{0,1}} };
    //Relation r;

    //writeRelationToFile(r,"test2.txt" );
    //readRelationFromFile(r, "test2.txt");


}
