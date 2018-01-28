#ifndef HELPERS_H
#define HELPERS_H

#include <QtGlobal>

// This file contains general helper functions for basic
// operations used through the application.

// Return random integer between given int values
// Pre: low < high
static int randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

#endif // HELPERS_H
