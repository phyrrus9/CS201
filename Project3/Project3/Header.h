//
//  Header.h
//  Project3
//
//  Created by Ethan Laur on 2/9/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef Project3_Header_h
#define Project3_Header_h



#endif
FILE *f = filename ? fopen(filename, "r") : stdin; //open if there is one, else read from stdin
if (stdin == f)
cout << "Enter a rectangular maze using the following " << "characters:\nm - entry\ne - exit\n1 - wall\n0 - passage\n" << "Enter one line at at time; end with Ctrl-D:\n";
while (fscanf(f, "%s", str) != EOF)