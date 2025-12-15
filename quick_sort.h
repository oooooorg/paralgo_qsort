//
// Created by Tsvetkov Alexey on 12/14/25.
//

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>
#include <parlay/sequence.h>
#include <parlay/primitives.h>

void par_qsort(parlay::sequence<int>& arr);
void seq_qsort(std::vector<int> &arr);

#endif //QUICK_SORT_H

