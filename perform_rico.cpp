//////////////////////////////////////////////////////////////////////
/// @file load_data.cpp
/// 
/// @Load the arrf file
//////////////////////////////////////////////////////////////////////



/************************** base head files ******************************/



# include <stdio.h>

# include <malloc.h>

# include <math.h>

# include <ctype.h>

# include <string.h>

# include <stdlib.h>

# include <assert.h>

# include <ctype.h>

# include <errno.h>

# include <float.h>

# include <limits.h>

# include <locale.h>

# include <setjmp.h>

# include <stdarg.h>

# include <signal.h>

# include <string.h>

# include <stddef.h>

# include <time.h>

# include <algorithm>

using namespace std;



/************************* other head files ************************/
#include "arff_attr.h"
#include "arff_data.h"
#include "arff_instance.h"
#include "arff_lexer.h"
#include "arff_parser.h"
#include "arff_scanner.h"
#include "arff_token.h"
#include "arff_utils.h"
#include "arff_value.h"

/********************internal functions****************************/
bool isEqual(int i, int j);
void Combination(int arr[], int nLen, int m, int out[], int outLen, vector<vector<int> > &rulesIndexs, vector<vector<int> > &decisionPartition);
void build_partition(vector<int> attr_indexs, vector<vector<int> > &partition);
bool isAttrEqual(int index, int otherIndex, vector<int> attr_indexs);
bool isSubSet(vector<vector<int> > partition, vector<vector<int> > decisionPartition);
bool isMinimal(vector<int> attr_index, vector<vector<int> > rulesIndexs);

/************************ global variables ************************/
// the datas from arff files
ArffData *data = NULL;
int decisionAttr_num;
int maxAttrCov_num;
int minCov_num;
// this array contains the file that decide whether or not an attribut is a decision attributes
bool* decisionAttrBool;
// this array indicates the indexs of the decision attributes 
int* decisionAttrArray;
int num_instance;
char arff_file[100];
FILE *fpout;

/************************ external variables**********************/
extern char cfg_file[100];


/************************ load_data *****************************/
void load_data() {
	FILE *fpcfg;
	if ( fopen(cfg_file, "r") == NULL) {
		printf("\nError: cannot open data file %s for read!\n", cfg_file);
		exit(1);
	}

	/******* read the configurations ***************************/

	printf("\nLoading configure file ... \n");

	fpcfg = fopen(cfg_file, "r");
	char line[50];

	fgets(line, 50, fpcfg);
	
	

	sscanf(line, "%s", arff_file);
	// Begin loading the arrf file
	printf("\nLoading arrf file: %s\n", arff_file);

	ArffParser parser(arff_file);
	
	//get the datas
	data = parser.parse();
	
	int32 attr_num = data->num_attributes();
	printf("\nThere are %ld atrributes:\n", attr_num);
	
	decisionAttrBool = new bool[attr_num];
	for(int i = 0; i < attr_num; i++) {
		ArffAttr* attr = data->get_attr(i);
		std::string attr_name = attr->name();
		decisionAttrBool[i] = false;
		printf("%d. %s\n", i, attr_name.c_str());
	}
	
	
	printf("\nEnter the number of decision attributes:\n");
	scanf("%d", &decisionAttr_num);
	decisionAttrArray = new int[decisionAttr_num];
	
	for(int i = 0; i < decisionAttr_num; i++) {
		printf("\nType the digit before the %dth attributes that is to be considered the decision attributes:\n", i);
		int index;
		scanf("%d", &index);
		decisionAttrArray[i] = index;
		decisionAttrBool[index] = true;
	}
	
	
	printf("\nEnter the maximum number of attributes to consider for a covering:\n");
	scanf("%d", &maxAttrCov_num);
	
	printf("\nEnter the minimum coverage required for a rule to be reported:\n");
	scanf("%d", &minCov_num);
	
	num_instance = data->num_instances();
		
	fclose(fpcfg);
	printf("\nFinish loading data file!\n");
}

/*********************get_decision_partition**************************************/
void get_decision_partition( vector<vector<int> > &decisionPartition) {
	ArffParser parser(arff_file);
	
	//get the datas
	data = parser.parse();
	char out_file[100];
	char arff_file_cpy[100];
	strcpy(arff_file_cpy, arff_file);
	char *out_file_abs = strtok(arff_file_cpy, ".");
	strcat(out_file_abs, ".log");
	strcpy(out_file, out_file_abs);
	
	fpout =fopen(out_file, "w");
	
	for(int i = 0; i < num_instance; i++) {
		//printf("For instance %d\n", i);
		vector<vector<int> >::size_type sz = decisionPartition.size();
		//Find the partition for a specific instance 
		//if the partitions are empty
		if( sz == 0) {
			vector<int> new_vector;
			new_vector.push_back(i);
			decisionPartition.push_back(new_vector);
		} else {
			bool findPartition = false;
			for(unsigned j = 0; j < sz; j++) {
				//printf("decisionPartition[%d][0]:%d\n", j, decisionPartition[j][0]);
				if( isEqual(decisionPartition[j][0], i) ) {
					decisionPartition[j].push_back( i );
					findPartition = true;
					break;
				}
			}
			if(findPartition == false) {
				vector<int> new_vector;
				new_vector.push_back(i);
				decisionPartition.push_back(new_vector);
			}
		}
		
	}
	
	vector<vector<int> >::size_type sz = decisionPartition.size();
	fprintf(fpout, "\nDecision attributes:[");
	for(int i = 0; i < decisionAttr_num; i++) {
		ArffAttr* attr = data->get_attr(decisionAttrArray[i]);
		fprintf(fpout, "%s, ", attr->name().c_str());
	}
	fprintf(fpout, "]\n");
	
	fprintf(fpout, "\nDistribution of values for decision attributions:\n");
	for(unsigned int i = 0; i < sz; i++) {
		fprintf(fpout, "Value: ");
		for(int j = 0; j < decisionAttr_num; j++) {
			ArffValue* val = data->get_instance(decisionPartition[i][0])->get(decisionAttrArray[j]);
			ArffValueEnum val_type = val->m_type;
			switch(val_type) {
			case INTEGER:
				fprintf(fpout, "%d, ", val->m_int);
				break;
			case FLOAT:
				fprintf(fpout, "%f, ", val->m_float);
				break;
			case NOMINAL:
			case STRING:
				fprintf(fpout, "%s, ", val->m_str.c_str());
				break;
			case UNKNOWN_VAL:
				fprintf(fpout, "?, ");
				break;
			default:
				fprintf(fpout, "?, ");
			}
		}
		fprintf(fpout, "Occurences: %d\n", decisionPartition[i].size());
		sort(decisionPartition[i].begin(), decisionPartition[i].end());
	}
			
}

bool isEqual(int i, int j) {
	//printf("instancd %d, instance %d\n", i, j);
	ArffInstance* lhs = data->get_instance( i );
	ArffInstance* rhs = data->get_instance( j );
	
	//printf("get the instances!\n");
	for(int d = 0; d < decisionAttr_num; d++) {
		//printf("d= %d\n",d);
		//printf("decisionAttrArray[%d]: %d\n", d, decisionAttrArray[d]);
		ArffValue* lhs_value = lhs->get(decisionAttrArray[d]);
		ArffValue* rhs_value = rhs->get(decisionAttrArray[d]);
		//printf("get the value!\n");
		if(!(*lhs_value == *rhs_value)) {
			return false;
		}
	}
	
	return true;
}

/*********************get_rules**************************************/
void get_rules(vector<vector<int> > &decisionPartition) {
	// get all the rules for the covering
	ArffParser parser(arff_file);
	
	//get the datas
	data = parser.parse();
	int32 attr_num = data->num_attributes();
	int *non_dec_attr;
	non_dec_attr = new int[attr_num - decisionAttr_num];
	int attr_index = 0;
	
	//get the non decision attributes index
	for(int i = 0; i < attr_num; i++) {
		if(decisionAttrBool[i] == false) {
			non_dec_attr[attr_index] = i;
			attr_index++;
		}
	}
	
	//the vectors to save rules for coverings
	vector<vector<int> > rulesIndexs;
	//get the combinations
	for( int m = 1; m <= maxAttrCov_num; m++) {
		int* out = new int[m];
		//find all the possible combinations of rules
		Combination(non_dec_attr, attr_num - decisionAttr_num, m, out, m , rulesIndexs, decisionPartition);
		delete [] out;
	}
	
	if(rulesIndexs.empty()) {
		fprintf(fpout, "No rules for the covering!\n");
	}
	
	fclose(fpout);
}

void Combination(int arr[], int nLen, int m, int out[], int outLen, vector<vector<int> > &rulesIndexs, vector<vector<int> > &decisionPartition)  
{  
    if(m == 0)  
    {  
		vector<int> new_vector;
		for (int j = 0; j < outLen; j++) {
			new_vector.push_back(out[j]);
        } 
		
		vector<vector<int> > partition;
		build_partition(new_vector, partition);
		if(isSubSet(partition, decisionPartition) && isMinimal(new_vector, rulesIndexs)) {
			rulesIndexs.push_back(new_vector);
			//report the rules that is good
			vector<vector<int> >::size_type rSize = partition.size();
			fprintf(fpout, "\nRules for covering:[");
			for(int i = 0; i < outLen; i++) {
				ArffAttr* attr = data->get_attr(new_vector[i]);
				fprintf(fpout, "%s, ", attr->name().c_str());
			}
			fprintf(fpout, "]:\n");
	
			fprintf(fpout, "[");
			for(unsigned int i = 0; i < rSize; i++) {
				fprintf(fpout, "[ ");
				if(partition[i].size() >= minCov_num) {
					//print non_decision attributes values
					for(int j = 0; j < outLen; j++) {
						ArffValue* val = data->get_instance(partition[i][0])->get(new_vector[j]);
						ArffValueEnum val_type = val->m_type;
						switch(val_type) {
						case INTEGER:
							fprintf(fpout, "%d, ", val->m_int);
							break;
						case FLOAT:
							fprintf(fpout, "%f, ", val->m_float);
							break;
						case NOMINAL:
						case STRING:
							fprintf(fpout, "%s, ", val->m_str.c_str());
							break;
						case UNKNOWN_VAL:
							fprintf(fpout, "?, ");
							break;
						default:
							fprintf(fpout, "?, ");
						}
					}
			
					//print decision attributes values
					for(int j = 0; j < decisionAttr_num; j++) {
						ArffValue* val = data->get_instance(partition[i][0])->get(decisionAttrArray[j]);
						ArffValueEnum val_type = val->m_type;
						switch(val_type) {
							case INTEGER:
								fprintf(fpout, "%d, ", val->m_int);
								break;
							case FLOAT:
								fprintf(fpout, "%f, ", val->m_float);
								break;
							case NOMINAL:
							case STRING:
								fprintf(fpout, "%s, ", val->m_str.c_str());
								break;
							case UNKNOWN_VAL:
								fprintf(fpout, "?, ");
								break;
							default:
								fprintf(fpout, "?, ");
						}
					}
					fprintf(fpout, "],");
			
					fprintf(fpout, "%d],", partition[i].size());
		
				}
				}
				fprintf(fpout, "]\n");
			}
			return;  
    }  
  
    for (int i = nLen; i >= m; --i)  
    {  
        out[m-1] = arr[i-1];   
        Combination(arr,i-1,m-1,out,outLen, rulesIndexs, decisionPartition); 
    }  
}  

void build_partition(vector<int> attr_index, vector<vector<int> > &partition) {
	for(int i = 0; i < num_instance; i++) {
		vector<vector<int> >::size_type sz = partition.size();
		//Find the partition for a specific instance 
		//if the partitions are empty
		if( sz == 0) {
			vector<int> new_vector;
			new_vector.push_back(i);
			partition.push_back(new_vector);
		} else {
			bool findPartition = false;
			for(unsigned j = 0; j < sz; j++) {
				if( isAttrEqual(partition[j][0], i, attr_index) ) {
					partition[j].push_back( i );
					findPartition = true;
					break;
				}
			}
			if(findPartition == false) {
				vector<int> new_vector;
				new_vector.push_back(i);
				partition.push_back(new_vector);
			}
		}
		
	}
}

bool isAttrEqual(int index, int otherIndex, vector<int> attr_index) {
	ArffInstance* lhs = data->get_instance( index );
	ArffInstance* rhs = data->get_instance( otherIndex );
	
	vector<int>::size_type size = attr_index.size();
	for(unsigned int i = 0; i < size; i++) {
		ArffValue* lhs_value = lhs->get(attr_index[i]);
		ArffValue* rhs_value = rhs->get(attr_index[i]);
		if(!(*lhs_value == *rhs_value)) {
			return false;
		}
	}
	
	return true;
}

bool isSubSet(vector<vector<int> > partition, vector<vector<int> > decisionPartition) {
	vector<vector<int> >::size_type pSize = partition.size();
	vector<vector<int> >::size_type dSize = decisionPartition.size();
	
	for(unsigned int i = 0; i < pSize; i++) {
		sort(partition[i].begin(), partition[i].end());
		bool findIncludes = false;
		//find the set that includes partition[i]
		for(unsigned int j = 0; j < dSize; j++) {
			if(includes(decisionPartition[j].begin(), decisionPartition[j].end(), partition[i].begin(), partition[i].end())) {
				//find the set
				findIncludes = true;
				break;
			}
		}
		
		//can't find the set
		if(findIncludes == false) {
			return false;
		}
	}
	
	return true;
}

bool isMinimal(vector<int> attr_index, vector<vector<int> > rulesIndexs) {
	sort(attr_index.begin(), attr_index.end());
	vector<vector<int> >::size_type rSize = rulesIndexs.size();
	if(rSize == 0) return true;
	for(unsigned int i = 0; i < rSize; i++) {
		//find the rules that is a subset
		if(includes(attr_index.begin(), attr_index.end(), rulesIndexs[i].begin(), rulesIndexs[i].end())) {
			return false;
		}
	}
	return true;
}

/********************** end *********************************/