CC=g++  


INCLUDE =. 


CFLAGS =-g -Wall

OO = main.o perform_rico.o  arff_attr.o arff_data.o arff_instance.o arff_lexer.o arff_parser.o arff_scanner.o arff_token.o arff_utils.o arff_value.o 

HH = arff_attr.h perform_rico.h  arff_data.h arff_instance.h arff_lexer.h arff_parser.h arff_scanner.h arff_token.h arff_utils.h arff_value.h 

rico: main.o $(OO)   

	$(CC) $(OO) -o rico



main.o: main.cpp $(HH)  

	$(CC) -c -g -Wall main.cpp 

perform_rico.o: perform_rico.cpp $(HH)

	$(CC) -c -g -Wall perform_rico.cpp

get_decision_partition.o: get_decision_partition.cpp $(HH)

	$(CC) -c -g -Wall get_decision_partition.cpp


arff_attr.o: arff_attr.cpp  $(HH)

	$(CC) -c $(CFLAGS)  arff_attr.cpp


arff_data.o: arff_data.cpp $(HH)

	$(CC) $(CFLAGS) -c arff_data.cpp

arff_instance.o: arff_instance.cpp $(HH)

	$(CC) $(CFLAGS) -c arff_instance.cpp
	
arff_lexer.o: arff_lexer.cpp $(HH)
	$(CC) $(CFLAGS) -c arff_lexer.cpp
	
arff_parser.o: arff_parser.cpp $(HH)
	$(CC) $(CFLAGS) -c arff_parser.cpp

arff_scanner.o: arff_scanner.cpp $(HH)
	$(CC) $(CFLAGS) -c arff_scanner.cpp

arff_token.o: arff_token.cpp $(HH)
	$(CC) $(CFLAGS) -c arff_token.cpp

arff_utils.o: arff_utils.cpp $(HH)
	$(CC) $(CFLAGS) -c arff_utils.cpp
	
arff_value.o: arff_value.cpp $(HH)
	$(CC) $(CFLAGS) -c arff_value.cpp
