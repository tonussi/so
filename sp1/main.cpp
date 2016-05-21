#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>

#include <fstream>

using namespace std;

// STRUCTURES
typedef struct {
    unsigned int row; /*! row passed */
    unsigned int col; /*! column passed*/
} parameters;

// DEFINES
/*! table size */
#define TABLE_SIZE 9

// GERAL VARIABLES
/*! sudoku table */
int table[TABLE_SIZE][TABLE_SIZE];

/*! geral check values */
void * rv_col;
void * rv_row;
void * rv_square[TABLE_SIZE];

/*! mutex to controll print stream */
pthread_mutex_t print_mutex;


// Fuctions declaration
/*! simple print table function */
void print_table();

/*! read_table_from_file reads a tabel on a specific file.
 *
 *  Arguments:
 *  path: name of the file to read
 */
void read_table_from_file(const char *path);

// HELP ANALYZE FUNCTIONS
/*! all_false sets all the possibly values on v[] to false.
 *  This function helps on analyze functions.
 */
void all_false(bool v[]);

/*! verify is a function to verify ALL THE v[] ARRAY
 *  values and make an AND between them. If some of them
 *  are false, then the return is false, else, the return
 *  is true.
 */
bool verify(bool v[]);

// ANALYZE FUNCTIONS
/*! Analyze column function. Parameters must be a parameters
 *  struct with coordinates(row and column).
 */
void *col_analyze(void* p);

/*! row analyze function */
void *row_analyze(void* p);

/*! Analyze square function. Parameter 'p' must be a parameters
 *  struct with coordinates(row and column).
 *
 *  The logic is a verification by index mapping that all elements are distinct
 *  if not return (void *) false, or if its true than return (void *) true
 */
void *square_analyze(void* p);


//Fuctions implementation
/*! print table function */
void print_table() {
    pthread_mutex_lock(&print_mutex);
    cout << "  sudoku table " << endl;
    for(int i = 0; i < TABLE_SIZE; i++) {
        for(int j = 0; j < TABLE_SIZE; j++) {
            cout << table[i][j] << " ";
        }
        cout << "\n";
    }
    pthread_mutex_unlock(&print_mutex);
}

/*! read_table_from_file function */
void read_table_from_file(const char *path) {
  int i;
  FILE *file;
  if ((file = fopen(path, "r"))) {
    for (i = 0; i < TABLE_SIZE; i++) {
      int *ptr = table[i];
      fscanf(file, "%d %d %d %d %d %d %d %d %d",
             ptr, ptr + 1, ptr + 2, ptr + 3, ptr + 4,
             ptr + 5, ptr + 6, ptr + 7, ptr + 8);
    }
    fclose(file);
  }
}

// HELP ANALYZE FUNCTIONS
/*! all_false function */
void all_false(bool v[]) {
    for(int i = 0 ; i < TABLE_SIZE ; i++) v[i] = false;
}

/*! verify function */
bool verify(bool v[]) {
    bool ret = true;
    for(int i = 0 ; i < TABLE_SIZE ; i++) ret = ret && v[i];
    return ret;
}

// ANALYZE FUNCTIONS
/*! column analyze function */
void *col_analyze(void* p) {
    pthread_mutex_lock(&print_mutex);
    cout << "column thread created!" << endl;
    pthread_mutex_unlock(&print_mutex);

    /// Test if all values are mapable to a all-zero array from 1 to 9
    /// the array is from 0 to 9 but we only want it from 1 to 9
    /// the testing table values between 0 and 11 is the minimum security
    /// check we can do

    for(int i = 0 ; i < TABLE_SIZE ; i++) {
        int index = 0;
        int distinctIndexMap[11] = {0};
        for(int j = 0 ; j < TABLE_SIZE ; j++) {
            int index = table[j][i];
            if (distinctIndexMap[index] != 0 || !(table[j][i] > 0 and table[j][i] < 11) ) {
                return (void *) false;
            }
            else {
                distinctIndexMap[index] = 1;
            }
        }
    }

    pthread_mutex_lock(&print_mutex);
    cout << "column thread ended!" << endl;
    pthread_mutex_unlock(&print_mutex);
    return (void *) true;
}

/*! row analyze function */
void *row_analyze(void* p) {
    pthread_mutex_lock(&print_mutex);
    cout << "row thread created!" << endl;
    pthread_mutex_unlock(&print_mutex);

    /// Test if all values are mapable to a all-zero array from 1 to 9
    /// the array is from 0 to 9 but we only want it from 1 to 9
    /// the testing table values between 0 and 11 is the minimum security
    /// check we can do

    for(int i = 0 ; i < TABLE_SIZE ; i++) {
        int index = 0;
        int distinctIndexMap[11] = {0};
        for(int j = 0 ; j < TABLE_SIZE ; j++) {
            int index = table[i][j];
            if (distinctIndexMap[index] != 0 || !(table[i][j] > 0 and table[i][j] < 11)) {
                return (void *) false;
            }
            else {
                distinctIndexMap[index] = 1;
            }
        }
    }

    pthread_mutex_lock(&print_mutex);
    cout << "row thread ended!" << endl;
    pthread_mutex_unlock(&print_mutex);
    return (void *) true;
}

/*!  square analyze function */
void *square_analyze(void* p) {
    parameters *param = (parameters*) p;
    pthread_mutex_lock(&print_mutex);
    cout << "square thread created! row[" << param->row << "], col[" << param->col << "]" << endl;
    pthread_mutex_unlock(&print_mutex);

    int startRowPoint = param->row;
    int startColPoint = param->col;
    int endRowPoint   = param->row + 3;
    int endColPoint   = param->col + 3;

    /// Test if all values are mapable to a all-zero array from 1 to 9
    /// the array is from 0 to 9 but we only want it from 1 to 9
    /// the testing table values between 0 and 11 is the minimum security
    /// check we can do

    int distinctIndexMap[11] = {0};
    for (int i = startRowPoint; i < endRowPoint; i++) {
        for (int j = startColPoint; j < endColPoint; j++) {
            int index = table[j][i];
            if (distinctIndexMap[index] != 0 || !(table[j][i] > 0 and table[j][i] < 11)) {
                return (void *) false;
            }
            else {
                distinctIndexMap[index] = 1;
            }
        }
    }

    pthread_mutex_lock(&print_mutex);
    cout << "square thread ended!" << endl;
    pthread_mutex_unlock(&print_mutex);
    return (void *) true;
}

int main(int argc, char* argv[]) {

    /*! START CHECK FOR RIGHT USAGE:
     *      $ ./sp1.out <file_path>
     *
     *  The file_path must be passed as parameter.
     */
    if(argc != 2) {
        cout << "ERROR SP1 USAGE: ./sp1.out <file_path>" << endl;
        return 0;
    }

    // read the table from sended file_path
    read_table_from_file(argv[1]);
    print_table();

    /*! threads:
     *  row_t: row analyzer
     *  col_t: line analyzer
     *  square_t[]: threads to analyze the squares
     */
    pthread_t row_t;
    pthread_t col_t;
    pthread_t square_t[TABLE_SIZE];

    /*! MUTEX TO PRINT(usage) INIT */
    pthread_mutex_init(&print_mutex, NULL);

    /*! square_p: square parameters.
     *  Each square_p on array must be used by one square_t thread
     */
    parameters square_p[TABLE_SIZE];

    // THREAD CREATION
    /*! Column analyzer
     *  @param col_t col thread
     *  @param col_analyze col analyze function
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_create(&col_t, NULL, &col_analyze, NULL);

    /*! Row analyzer
     *  @param row_t row thread
     *  @param row_analyze row analyze function
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_create(&row_t, NULL, &row_analyze, NULL);

    // Square analyzer builder
    int thread_count = 0;
    for(int i = 0; i < TABLE_SIZE; i += 3) {
        for(int j = 0; j < TABLE_SIZE; j +=3) {

            /// set the square_p row and col
            /// this is a mapping scenario where
            /// the mapping is starting points for
            /// earch 3x3 grid in the sudoku, these
            /// ones are assigned to earch one of the
            /// 9 threads, so earch one of the threads
            /// can verify the grid independently

            square_p[thread_count].row = i;
            square_p[thread_count].col = j;
            /*! create the thread and send the params
             *
             *  @param square_t thread to run also
             *  @param NULL end point and the address to the runner
             *  @param square_analyze which eval a certain grid and the input to the
             *  @param runner is the row and col starting mapping points
             *  @return int error (0 if no erros, or a number pointing the error type)
             */
            pthread_create(&(square_t[thread_count]), NULL, &square_analyze, &(square_p[thread_count]));
            thread_count++;
        }
    }

    // THREAD JOIN
    /*! Column join function
     *  @param col_t column thread
     *  @param rv_col component to store the returning value of a col_analyze
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_join(col_t, &rv_col);

    /*! row join function
     *  @param row_t row thread
     *  @param rv_row component to store the returning value of the row_analyze
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_join(row_t, &rv_row);

    /*! Squares join function
     *  @param square_t's squares thread
     *  @param rv_square component to store the returning values of the
     *                   square_analyze
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    for(int i = 0; i < TABLE_SIZE; i++) {
        pthread_join(square_t[i], &rv_square[i]);
    }

    // VERIFY VALUES
    /*! lock mutex to print
     *
     *  @param pthread_mutex_t address to a mutex declaration
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_mutex_lock(&print_mutex);

    /*! check for some error and store on 'check' */
    bool check = false;
    cout << "Hypotesis begins as: " << check << ". Starting testing things..." << endl;

    /*! if rv_col have problems */
    if((bool) rv_col == true) {
        cout << "rv_col: " << rv_col << " --------> OK!" << endl;
        check = true;
    } else {
        /*! else, print false! */
        cout << "rv_col: " << rv_col << " --------> ERROR! " << endl;
        check = false;
    }

    /*! if rv_row have problems */
    if((bool) rv_row == true) {
        cout << "rv_row: " << rv_row << " --------> OK!" << endl;
        check = true;
    } else {
        /*! else, print false! */
        cout << "rv_row: " << rv_row << " --------> ERROR! " << endl;
        check = false;
    }

    /*! algorithm to check all rv_square */
    for(int i = 0; i < TABLE_SIZE; i++) {
        if((bool) rv_square[i] == true) {
            cout << "rv_square[" << i << "]: " << rv_square[i] << " --> OK!" << endl;
            check = true;
        } else /*! else, print ok! */ {
            cout << "rv_square[" << i << "]: " << rv_square[i] << " --> ERROR! " << endl;
            check = false;
        }
    }

    /*! unlock print mutex
     *
     *  @param pthread_mutex_t address to a mutex declaration
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_mutex_unlock(&print_mutex);

    /*! destroy print mutex
     *
     *  @param pthread_mutex_t address to a mutex declaration
     *  @return int error (0 if no erros, or a number pointing the error type)
     */
    pthread_mutex_destroy(&print_mutex);

    return 0;

}
