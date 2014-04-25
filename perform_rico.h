#ifndef __INCLUDED_PERFORM_RICO_H__
#define __INCLUDED_PERFORM_RICO_H__
/***
** @get_decision_partition.h
*/
#include "arff_attr.h"
#include "arff_data.h"
#include "arff_instance.h"
#include "arff_lexer.h"
#include "arff_parser.h"
#include "arff_scanner.h"
#include "arff_token.h"
#include "arff_utils.h"
#include "arff_value.h"
#include <vector>

using namespace std;

void get_decision_partition(vector<vector<int> > &decisionPartition);
void load_data();
void get_rules(vector<vector<int> > &decisionPartition);
#endif