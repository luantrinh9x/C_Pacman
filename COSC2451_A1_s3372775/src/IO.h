#ifndef IO
#define IO

/** add function take left node, right node and pointer to the error string then return the double value */

/** write a file by using all elements in array and
  * file name from user input or previous file name from read file
  * @param finalArray is received from below writeMap() method
  * @param fileName is received from user input or read from .pac file
  * @param rows & cols are read from .pac file
  * @param createMap is check to write normal map or blank map
  */
void writeFile(char * finalArray, char * fileName, int rows, int cols, bool createMap);

/** create finalArray by allocate heap memory and
  * assign all elements in editArray to finalArray
  * @param editArray is received from main class pacman.c
  * @param fileName is received from user input or read from .pac file
  * @param rows & cols are read from .pac file
  * @param createMap is check to write normal map or blank map
  */
void writeMap(char * editArray,char * fileName,int rows, int cols, bool createMap);

/** read file with file name from user input, also create an array then put all
  * file name, rows, cols, map into that array then return to main class pacman.c
  * display authors, title, rows and cols to the screen
  * @param fileName is received from user input
  */
char * readFile(char * fileName);

/** draw map by read each character in the .pac file and also use
  * col and row to calculate then align the map in center of the screen
  * @param fullName is the file's path is passed from above readFile() method
  * @param col is passed from above readFile() method 
  * @param row is passed from above readFile() method 
  */
void drawMap(char * fullName, int col, int row);

#endif
