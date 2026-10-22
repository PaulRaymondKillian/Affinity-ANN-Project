/*------------------------------------------------------------------------
    Program :           Program01.h
    Written by:         Paul Killian
    Created:            2006-01-25
    Description:        Header File For Elementary Multiplication Program
 -------------------------------------------------------------------------*/

#ifndef POPULATIONBACKPROPAPPCPP_H
#define POPULATIONBACKPROPAPPCPP_H

#include <conio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void LoadPopulationAppDataFile( void );
string InputPopDataFile( void );
void AddFileToPreviousEntries( string file );

#endif