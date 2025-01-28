all: test_utility rm_obj

clean:
	rm -rf *.o test_utility

stat_data.o: ./data_processing/stat_data/stat_data.c
			gcc -c -o stat_data.o ./data_processing/stat_data/stat_data.c

metrics.o: ./data_processing/metrics/metrics.c
			gcc -c -o metrics.o ./data_processing/metrics/metrics.c

data_processing.o: ./data_processing/data_processing.c
	gcc -c -o data_processing.o data_processing/data_processing.c

test.o:
	gcc -c -o test.o test.c

test_utility: test.o data_processing.o stat_data.o metrics.o
			gcc -o test_utility test.o data_processing.o stat_data.o metrics.o
rm_obj:
	rm -fr *.o
