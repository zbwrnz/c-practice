#ifndef __INTERVAL_TREE_H__
#define __INTERVAL_TREE_H__

#include "ipa.h"
#include "interval.h"
#include "node.h"

typedef enum {lo, in, hi} Pos;
Pos point_overlap(unsigned int, Interval);
struct Node * build_tree(IPA*, unsigned int, unsigned int);

#endif
