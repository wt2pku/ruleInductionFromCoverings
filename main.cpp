#include "arff_attr.h"
#include "arff_data.h"
#include "arff_instance.h"
#include "arff_lexer.h"
#include "arff_parser.h"
#include "arff_scanner.h"
#include "arff_token.h"
#include "arff_utils.h"
#include "arff_value.h"
#include "perform_rico.h"
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
/************************ global variables ************************/
char cfg_file[100];

/*********************** main *************************************/
int main(int argc, char *argv[]) {
	char defaultFile[100] = "default.cfg";
    if (argc==1)  {
		strcpy(cfg_file, defaultFile);
	} else {
		strcpy(cfg_file, argv[1]);
	}
	load_data();
	vector<vector<int> > decisionPartition;
	get_decision_partition(decisionPartition);
	get_rules(decisionPartition);
	return 0;
}
	
	
	
	
