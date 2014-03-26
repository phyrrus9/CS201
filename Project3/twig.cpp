#include <iostream>
#include <stack>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

template<class T>
class Stack : public stack<T>
{
public:
	T pop()
	{
		T tmp = stack<T>::top();
		stack<T>::pop();
		return tmp;
	}
};

class Cell
{
public:
	Cell(int i = 0, int j = 0) { x = i; y = j; }
	bool operator== (const Cell& c) const { return x == c.x && y == c.y; }
private:
	int x, y;
	friend class Maze;
};

class Maze
{
public:
	Maze(char *);
	void exitMaze();
private:
	Cell currentCell, exitCell, entryCell;
	const char exitMarker, entryMarker, visited, passage, wall;
	Stack<Cell> mazeStack;
	char **store;         // array of strings;
	void pushUnvisited(int,int);
	int rows, cols;
	friend ostream& operator<< (ostream& out, const Maze& maze)
	{
		sleep(1); //wait a second before doing anyprinting
		system("clear");
		for (int row = 0; row <= maze.rows+1; row++) out << maze.store[row] << endl; //less instructions this way
																	  //seems like
																	  //mov r1, #0 ;[=row]
																	  //ldr r2, [=maze,=rows]
																	  //ltop:
																	  //mov r3, r2 ; <----
																	  //add r3, #1 ; <----
																	  //cmp r2, r3
																	  //blt =lbottom
																	  //mov r3, =maze
																	  //mov r3, =store
																	  //add r3, =row
																	  //mov r3, [r3]
																	  //call _output
																	  //jmp =ltop
																	  //lbottom:
																	  //popq r1
																	  //B r1
										
		out << endl;
		return out;
	}
};

Maze::Maze(char *filename) : /*takes the input file as a parameter*/
exitMarker('e'), entryMarker('m'), visited('.'),
passage('0'), wall('1')
{
	Stack<char*> mazeRows;
	char str[80], *s;
	int col, row = 0;
	FILE *f = filename ? fopen(filename, "r") : stdin; //open if there is one, else read from stdin
	if (stdin == f)
		cout << "Enter a rectangular maze using the following " << "characters:\nm - entry\ne - exit\n1 - wall\n0 - passage\n" << "Enter one line at at time; end with Ctrl-D:\n";
	while (fscanf(f, "%s", str) != EOF)
	{
		row++;
		cols = strlen(str);
		s = new char[cols+3];    // two more cells for borderline columns;
		mazeRows.push(s);
		strcpy(s+1,str);
		s[0] = s[cols +1 ] = wall; // fill the borderline cells with 1s;
		s[cols+2] = '\0';
		if (strchr(s, exitMarker) != 0)
		{
			exitCell.x = row;
			exitCell.y = strchr(s,exitMarker) - s;
		}
		if (strchr(s ,entryMarker) != 0)
		{
			entryCell.x = row;
			entryCell.y = strchr(s,entryMarker) - s;
		}
	}
	rows = row;
	store = new char*[rows+2];        // create a 1D array of pointers;
	store[0] = new char[cols+3];      // a borderline row;
	while (!mazeRows.empty()) store[row--] = mazeRows.pop();
	store[rows+1] = new char[cols+3]; // another borderline row;
	store[0][cols+2] = store[rows+1][cols+2] = 0x00;
	for (col = 0; col <= cols+1; col++)
	{
		store[0][col] = wall;         // fill the borderline rows with 1s;
		store[rows+1][col] = wall;
	}
}

void Maze::pushUnvisited(int row, int col) { if (store[row][col] == passage || store[row][col] == exitMarker) mazeStack.push(Cell(row,col)); }

void Maze::exitMaze()
{
	int row, col;
	currentCell = entryCell;
	while (!(currentCell == exitCell))
	{
		row = currentCell.x;
		col = currentCell.y;
		cout << *this;         // print a snapshot;
		if (!(currentCell == entryCell))
			store[row][col] = visited;
		pushUnvisited(row-1,col);
		pushUnvisited(row+1,col);
		pushUnvisited(row,col-1);
		pushUnvisited(row,col+1);
		if (mazeStack.empty())
		{
			cout << *this << "Failure\n";
			return;
		}
		else currentCell = mazeStack.pop();
	}
	cout << *this << "Success\n";
}

int main(int argc, char * * argv)
{
	Maze(argv[1]).exitMaze();
	return 0;
}
