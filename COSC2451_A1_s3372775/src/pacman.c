#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include "interface.h"
#include "IO.h"

// define your own colors: the numbers correspond to the colors defined
// in the terminal configuration
#define COLOR_BACKGROUND 0
#define COLOR_WALL       1
#define COLOR_PACMAN     2

static void finish(int sig);
static void clearCMD(char * cmdArray);
static void exitCMD(char * cmdArray);
static void executeCMD(char * cmdArray);
static void processExecuteCMD(char * inputFileName, int mode);
static void drawBorder(int startX, int startY, int col, int row);
static void checkBlankMap(bool checkBlank);

//boolean to check command mode
bool commandMod = false;

//start index of cmd array
int countCMD = 0;

//define char array	of edit mode		   
char editArray[2300];

// array store rows 
char rows[3] = {0,0,'\0'};
// array store cols
char cols[3] = {0,0,'\0'};

// variable for create,draw map 
int col = 0;
int row = 0;
int curX = 0;
int curY = 0;
int mapIndex = 0;
int startX = 0;
int startY = 0;

//read filename, rows, cols and map from file
char * getFileInfo;

int main(int argc, char *argv[])
{
    //int num = 0;
    //char out[75];
    //struct timespec delay = {0, 500000000L}, rem;
    
    //delay.tv_sec = 0;
    //delay.tv_nsec = 500000000L;
    
    /* initialize your non-curses data structures here */
    
    (void) signal(SIGINT, finish);  /* arrange interrupts to terminate */
    (void) initscr();               /* initialize the curses library */
    keypad(stdscr, TRUE);           /* enable keyboard mapping */
    (void) nonl();                  /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();                /* take input chars one at a time, no wait for \n */
    (void) noecho();                /* echo input - in color */

    if (has_colors()) 
    {
        start_color();
    
        // initialise you colors pairs (foreground, background)
        init_pair(1, COLOR_WALL,    COLOR_BACKGROUND);
        init_pair(2, COLOR_PACMAN,  COLOR_BACKGROUND);
    }
    
    attrset(COLOR_PAIR(0));
    
    //coordinate of command mode  
    int x = 0;
    int y = 0;
    
	//define char array	of command mode		   
    char cmdArray[50] = "";
    
    //use to load level using argv 1
    if(argv[1] != NULL)
    {
        //run ./pacman.sh filename.pac
        getFileInfo = readFile(argv[1]);
        
        //copy map to edit array
	    processExecuteCMD("", 3);
       
		// calculate the col,row 
        col = atoi(cols);
        row = atoi(rows);
        
        //find the maximum x , y coordinate of the ncurses screen		
		getmaxyx(stdscr,y,x);
		
		//get the start index to draw the map
		startX = (x - col) / 2;
		startY = 6;
		
		// move the cursor to the position next to the map
		wmove(stdscr, 6, startX-1); 
    }
    
	//do the same job of some line above
	getmaxyx(stdscr,y,x);
	startX = (x - col) / 2;
	startY = 6;
	wmove(stdscr, 6, startX - 1); 
	
	//infinity loop
    for (;;) 
    {
        /* refresh, accept single keystroke of input */
        int c = getch();    
        
		//take the current x , y coordinate of the cursor    
		getmaxyx(stdscr,y,x);
		curX = getcurx(stdscr);
		curY = getcury(stdscr);
		
		//must make sure that we calculate the startX, startY in second time		
		startX = (x - col) / 2;
		startY = 6;
		
	    if (commandMod == false) 
	    {
	        switch(c)
            {
			    case ':':
					clearCMD(cmdArray);
			        wmove(stdscr, y-1, 0);
			        clrtobot();
			        commandMod = true;
					addch(':');	
					break;
                case KEY_DOWN: 
					curX = getcurx(stdscr);
					curY = getcury(stdscr)+1;
                    wmove(stdscr, curY,curX);     
                                  
					// check if the cursor still inside the map 
					if(curX >= startX && curX <= (startX + col-1) 
						&& curY >= startY && curY <= (startY + row-1))
					{
						//depend on the coordinate x and y of the cursor to define the
						//index of the map
						mapIndex = (curY - startY)*col + (curX -startX);	
					}
			
                    break;
                case KEY_UP:
					curX = getcurx(stdscr);
					curY = getcury(stdscr)-1;
                    wmove(stdscr, curY,curX);
                    
					// check if the cursor still inside the map 
					if(curX >= startX && curX <= (startX + col-1) 
						&& curY >= startY && curY <= (startY + row-1))
					{
						//depend on the coordinate x and y of the cursor to define the
						//index of the map
						mapIndex = (curY - startY)*col + (curX -startX);	
					}	
					
                    break;
                case KEY_LEFT:
					curX = getcurx(stdscr) - 1;
					curY = getcury(stdscr);
                    wmove(stdscr, curY,curX);
                    
					// check if the cursor still inside the map 
					if(curX >= startX && curX <= (startX + col-1) 
						&& curY >= startY && curY <= (startY + row-1))
					{
						//depend on the coordinate x and y of the cursor to define the
						//index of the map
						mapIndex = (curY - startY)*col + (curX - startX);
					}
					
                    break;
                case KEY_RIGHT:
					curX = getcurx(stdscr) + 1;
					curY = getcury(stdscr);                    
					wmove(stdscr, curY,curX);
					
					// check if the cursor still inside the map 
					if(curX >= startX && curX <= (startX + col-1) 
						&& curY >= startY && curY <= (startY + row-1))
					{
						//depend on the coordinate x and y of the cursor to define the
						//index of the map
						mapIndex = (curY - startY)*col + (curX - startX);	
					}
					
					break;
                default:
                    if(curX >= startX && curX <= (startX + col - 1)
						&& curY >= startY && curY <= (startY + row - 1))
					{
						//depend on the coordinate x and y of the cursor to define the index of the map
						// when edit some char, using mapIndex to define where the edit char will placed
						editArray[mapIndex] = c;
						mapIndex++;	
					}
                    refresh();
                    
                    break;
            }
            
            //calling function pointer via interface
            //casting int* to char*
            getKey((char*)&c);
         }
         else
         {
            //ESCAPE
	        if(c != 27)
	        {	       
                if(c == KEY_DOWN || c == KEY_UP || c == KEY_LEFT || c == KEY_RIGHT || c == KEY_BACKSPACE)
                {
                    //do nothing for all arrow key
                    //BACKSPACE
                    if(c == KEY_BACKSPACE && countCMD >= 1)
                    {
                        wmove(stdscr, y-1,countCMD);
                        countCMD--;
                        delch();
                        cmdArray[countCMD] = '\0';
                    }
                }
                else
                {
                    //add into command array
		            strcat(cmdArray,(char*)&c);
				
				    //print the character into the screen by using count
				    addch((char)cmdArray[countCMD]);
				    countCMD++;
                }

				//ENTER
	            if(c == 13)    
			    {
					executeCMD(cmdArray);
					exitCMD(cmdArray);
					
			    }
		    }
		    else
		    {
				//mode when it suppose to do
				wmove(stdscr, y-1,0);
				exitCMD(cmdArray);
		    }	
	    }
    }
        //attrset(COLOR_PAIR(num % 8));
	    //num++;
	    
	    //sleep 
        //nanosleep(&delay, &rem);

    finish(0);  /* we're done */
}

//execute all command in command line if the command is correctly
static void executeCMD(char * cmdArray)
{
    //reset map index
    mapIndex = 0;
    
	//q
	if (cmdArray[0] == 'q' && strlen(cmdArray) == 2)
	{
		finish(0);
	}
	
///////////////////////////////////////////////////////////////////////////////////////////
	
	//w
	else if(cmdArray[0] == 'w' && strlen(cmdArray) == 2)
	{
	    //call method
	    processExecuteCMD("", 1);
	}
	
///////////////////////////////////////////////////////////////////////////////////////////
	
	//w filename
	else if(cmdArray[0] == 'w' && cmdArray[1] == ' ')
	{
	    char fileName[50];
	    
	    for(int i = 0; i < 50; i++)
	    {
	        fileName[i] = '\0';
	    }
	    
	    int i = 2;
	    
	    while(cmdArray[i] != '\0')
	    {
	        fileName[i-2] = cmdArray[i];
	        i++;
	    }
	    
	    fileName[i-3] = '\0';
	    
	    if(fileName[i-4] == 'c' && fileName[i-5] == 'a' && fileName[i-6] == 'p' && fileName[i-7] == '.')
	    {
            //write map to array
            processExecuteCMD(fileName, 2);
	    }
	    else
	    {
	        addstr("The format of write should be filename.pac!");
	    }
	}
	
///////////////////////////////////////////////////////////////////////////////////////////

	//wq
	else if(cmdArray[0] == 'w' && cmdArray[1] == 'q' && strlen(cmdArray) == 3)
	{
	    //call method
	    processExecuteCMD("", 1);
	    finish(0);
	}
	
///////////////////////////////////////////////////////////////////////////////////////////
	
	//wq filename
	else if(cmdArray[0] == 'w' && cmdArray[1] == 'q')
	{
	    char fileName[100];
	    
	    for(int i = 0; i < 100; i++)
	    {
	        fileName[i] = '\0';
	    }
	    
	    int i = 3;
	    
	    while(cmdArray[i] != '\0')
	    {
	        fileName[i-3] = cmdArray[i];
	        i++;
	    }
	    
	    fileName[i-4] = '\0';
	    
	    if(fileName[i-5] == 'c' && fileName[i-6] == 'a' && fileName[i-7] == 'p' && fileName[i-8] == '.')
	    {
            //write map to array
            processExecuteCMD(fileName, 2);
            finish(0);
	    }
	    else
	    {
	        addstr("The format of write & quit should be filename.pac!");
        }
	}
	
///////////////////////////////////////////////////////////////////////////////////////////

	//r filename
	else if(cmdArray[0] == 'r' && cmdArray[1] == ' ')
	{
		char fileName[100];
	    
	    for(int i = 0; i < 100; i++)
	    {
	        fileName[i] = '\0';
	    }
	    
	    int i = 2;
	    
	    while(cmdArray[i] != '\0')
	    {
	        fileName[i-2] = cmdArray[i];
	        i++;
	    }
	    
	    fileName[i-3] = '\0';
	    
	    //read map with name then return filename, rows & cols and map
	    getFileInfo = readFile(fileName);
	    
	    if(getFileInfo == NULL)
	    {
	        addstr("Dont match any file name!");
	    }
	    else
	    {
	        //copy map to edit array
	        processExecuteCMD("", 3);
	    }
	}
	
///////////////////////////////////////////////////////////////////////////////////////////
	
	//n filename x y
	else if(cmdArray[0] == 'n' && cmdArray[1] == ' ')
	{
	    char fileName[100];
	    
	    char rows[3] = {0,0,'\0'};
	    char cols[3] = {0,0,'\0'};
	    
	    int fileNameLength = 0;
	    int rowsLength = 0;
	    int colsLength = 0;
	    
	    for(int i = 0; i < 100; i++)
	    {
	        fileName[i] = '\0';
	    }
	
	    int i = 2;
	    
	    //after fileName
	    while(cmdArray[i] != ' ')
	    {
	        fileName[i-2] = cmdArray[i];
	        fileNameLength++;
	        i++;
	    }
	    
	    i++;
	    
	    //after x
	    while(cmdArray[i] != ' ')
	    {
	        rows[i- 3 - fileNameLength] = cmdArray[i];
	        rowsLength++;
	        i++;
	    }
	    
	    i++;
	    
	    //after y
	    while(cmdArray[i] != '\0')
	    {
	        cols[i - 4 - fileNameLength - rowsLength] = cmdArray[i];
	        colsLength++;
	        i++;
	    }
	    
	    int x = atoi(rows);
	    int y = atoi(cols);
	    
	    if(x > 0 && y > 0 && x <= 65 && y <= 35)
	    {
	        //write map to array
            writeMap(editArray,fileName,atoi(rows),atoi(cols),true);
            addstr("Successfully create new map!");
	    }
	    else
	    {
	        addstr("ERROR! Rows & Cols should between 0 and 35/65");
	    }
	}
	else
	{
	    addstr("Dont match any command!");
	}
}

//Clear comand line,command array,count and exit command mode
static void exitCMD(char * cmdArray)
{
	clrtobot();
    clearCMD(cmdArray);
	wmove(stdscr, 0,0);
	commandMod = false;					
	countCMD = 0;
}

//clear command array
static void clearCMD(char * cmdArray)
{
    int i = 0;
    while(cmdArray[i] != '\0') {
        cmdArray[i] = '\0';
        i++;
	}	
}

//exit program
static void finish(int sig)
{
    keypad(stdscr, false);
    nodelay(stdscr, false);
    nocbreak();
    echo();
    endwin();
    exit(sig);
}

static void processExecuteCMD(char * inputFileName, int mode)
{
    char fileName[100];
    char map[2300];
    
    int fileNameLength = 0;
    int rowsLength = 0;
    int colsLength = 0;
    int mapLength = 0;
    
    for(int i = 0; i < 100; i++)
    {
        fileName[i] = '\0';
    }
    
    for(int i = 0; i < 2300; i++)
    {
        map[i] = '\0';
    }
    
    //clear global row & col
    for(int i = 0; i < 2; i++)
    {
        rows[i] = 0;
        cols[i] = 0;
    }

    int i = 0;
    
    //after fileName
    while(getFileInfo[i] != ' ')
    {
        fileName[i] = getFileInfo[i];
        fileNameLength++;
        i++;
    }
    
    i++;
    
    //after rows
    while(getFileInfo[i] != ' ')
    {
        rows[i - 1 - fileNameLength] = getFileInfo[i];
        rowsLength++;
        i++;
    }
    
    i++;
    
    //after cols
    while(getFileInfo[i] != ' ')
    {
        cols[i - 2 - fileNameLength - rowsLength] = getFileInfo[i];
        colsLength++;
        i++;
    }
    
    i++;
    
    //after map
    while(getFileInfo[i] != '\0')
    {
        map[i - 3 - fileNameLength - rowsLength - colsLength] = getFileInfo[i];
        mapLength++;
        i++;
    }
    
    map[i - 3 - fileNameLength - rowsLength - colsLength] = '\0';
    
    //assign row & col each time read file
    col = atoi(cols);
    row = atoi(rows);
    
    //w & wq
    if(mode == 1)       
    {
        writeMap(editArray,fileName,atoi(rows),atoi(cols),false);
    }
    //w filename & wq filename
    else if(mode == 2)  
    {
        writeMap(editArray,inputFileName,atoi(rows),atoi(cols),false);
    }
    //copy map to edit array
    else if(mode == 3)  
    {
        //clear edit array
        clearCMD(editArray);
        //copy map to edit array
        strcpy(editArray,map);
        
        checkBlankMap(false);
    }
}

//check blank map to draw the border
static void checkBlankMap(bool checkBlank)
{
    for(int i = 0; i < row*col; i++)
    {
        if(editArray[i] == ' ')
        {
            checkBlank = false;
        }
        else
        {
            checkBlank = true;
            break;
        }
    }
    
    if(checkBlank == false)
    {
        int x = 0;
        int y = 0;

        col = atoi(cols);
        row = atoi(rows);

        getmaxyx(stdscr,y,x);
        startX = (x - col) / 2;
        startY = 6; 
        drawBorder(startX, startY, col, row);
    }
}

static void drawBorder(int startX, int startY, int col, int row)
{
	//check the start coordinate x and y to draw the border
    for(int i = startX - 1; i < startX + col + 1 ; i++)
    {
        mvaddch(startY - 1,i,'*');
        mvaddch(startY + row,i,'*');
    }
	
    for(int i = 0; i < row + 1; i++)
    {
        mvaddch(startY + i, startX -1 ,'*');
        mvaddch(startY + i, startX + col,'*');
    }
}
