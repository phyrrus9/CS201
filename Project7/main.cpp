#include "BST.h"
class BSTWordNode
{
	char str[30];
	int freq, length;
public:
	BSTWordNode()
	{
		memset(str, 0, 30);
		freq = 0;
	}
	BSTWordNode(const BSTWordNode &old)
	{
		setstr((char *)old.getstr());
		setfreq(old.getfreq());
	}
	BSTWordNode(char *_str, int _freq)
		{ setstr(_str); setfreq(_freq); }
	void setstr(char * _str)
		{ strcpy(str, _str); }
	void setfreq(int _freq)
		{ freq = _freq; }
	void setlength(int _length)
		{ length = _length; }
	const char *getstr() const
		{ return &str[0]; }
	int   getfreq() const
		{ return freq; }
	int getlength() const
		{ return length; }
	friend char operator==(const BSTWordNode &lhs, BSTWordNode &rhs);
	friend char operator< (const BSTWordNode &lhs, BSTWordNode &rhs);
	friend ostream& operator<< (ostream &lhs, BSTWordNode &rhs);
	BSTWordNode& operator=(const BSTWordNode &rhs)
	{
		setstr((char *)rhs.getstr());
		setfreq(rhs.getfreq());
		return *this;
	}
} _packed;
char operator==(const BSTWordNode &lhs, BSTWordNode &rhs)
	{ return (strcmp(lhs.getstr(), rhs.getstr())) == 0; }
char operator<(const BSTWordNode &lhs, BSTWordNode &rhs)
	{ return (strcmp(lhs.getstr(), rhs.getstr())) < 0; }
ostream& operator<< (ostream &lhs, BSTWordNode &rhs)
{
	lhs << rhs.getstr() << "(freq=" << rhs.getfreq() << ", depth=" << rhs.getlength() << ')' << endl;
	return lhs;
}
BSTWordNode *findNode(char *word, BST<BSTWordNode> *tree)
	{ return tree->search(*(new BSTWordNode(word, 0))); }
void addtotree(char *word, BST<BSTWordNode> *tree)
{
	if (findNode(word, tree) == NULL)
		tree->insert(*(new BSTWordNode(word, 1)));
	else
		findNode(word, tree)->setfreq(findNode(word, tree)->getfreq() + 1);
}

char * strcheck(char *str)
{
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (isalnum(str[i]))
			return &str[i];
	return NULL;
}

int main(int argc, char * * argv)
{
	FILE *f = fopen(argv[1], "r");
	BST<BSTWordNode> *tree = new BST<BSTWordNode>;
	char *tmpw = (char *)malloc(180), *sp = tmpw, *insw;
	int  *wc = (int *)malloc(sizeof(int));
	*wc = 0;
	memset(tmpw, 0, 120);
	while (fscanf(f, "%c", sp++) != EOF)
		if (!isalnum(*(sp - 1)))
		{
			*(sp - 1) = 0;
			if ((insw = strcheck(tmpw)) != NULL)
				addtotree(insw, tree);
			++*wc;
			memset(tmpw, 0, 120);
			sp = tmpw;
		}
	tree->inorder();
	printf("Total words: %d\nAVG hops: %lf\n", tree->nodeCount(),tree->avghops());
}

