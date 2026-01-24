#define MAXSTRLEN 10
#define MAXCATS 500

struct cat {
    int cat_ID; // Cat ID
    char name[MAXSTRLEN+1]; // Cat name (single word)
    int score1;
    int score2;
    int score3;
    float average;
};

void readData(struct cat *cats, int *c);
int getCutestCat(struct cat *s, int n);