#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <stdbool.h>
#include <string.h>
#include "IO.h"
#include "interface.h"

void writeFile(char * finalArray, char * fileName, int rows, int cols, bool createMap)
{
    //declare array to contain full path of file
    char fullName[50];
    
    //first part of file's path 
    char path[] = "levels/";
    
    //clear fullName array
    for(int i = 0; i < strlen(fullName); i++)
    {
        fullName[i] = '\0';
    }
    
    //copy first file's part to fullName 
    strcpy(fullName,path);
    
    //add file name from user input to first part 
    strcat(fullName,fileName);

    //write mode with full path
    FILE * f = fopen(fullName, "w");
    
    //if the path of file is not found
    //then return NULL
    if(f == NULL)
    {
        addstr("Error write!\n");
        endwin();
        exit(1);
    }

    /* authors of the level */
    const char * authors = "Trinh Luan <s3372775@rmit.edu.vn>, Cao Phi Hung <s3372748@rmit.edu.vn>";
    fprintf(f, "%s\n", authors);
    
    /* title of the level */
    fprintf(f, "%s\n", fileName);
    
    /* number of rows */
    fprintf(f, "%d\n", rows);
    
    /* number of rows */
    fprintf(f, "%d\n", cols);
    
    //take 1D finalArray then write like 2D array
    if(createMap != true)
    {
        //write normal map to .pac file
        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < cols; j++) 
            {
                fprintf(f, "%c", finalArray[i*cols+j]);
            }
            fprintf(f, "\n");
        }
    }
    //create new blank map
    else
    {
        //delare blankArray
        char blankArray[2300];
        
        //clear blank array
        for(int i = 0; i < 2300; i++)
        {
            blankArray[i] = '\0';
        }
        
        //assign all elements to blankArray
        for(int i = 0; i < rows*cols; i++)
        {
            blankArray[i] = ' ';
        }
        
        //write blank space to .pac file
        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < cols; j++) 
            {
                fprintf(f, "%c", blankArray[i*cols+j]);
            }
            fprintf(f, "\n");
        }
    }
    
    fprintf(f, "\n");

    fclose(f);
}

void writeMap(char * editArray,char * fileName,int rows, int cols, bool createMap)
{
    //declare array on heap
    char * finalArray = malloc(sizeof(char)*strlen(editArray));
    
    //get char from editArray then put into finalArray
    for (int i = 0; i < strlen(editArray); i++) 
    {
        finalArray[i] = editArray[i];
    }
    
    //write to .pac file
    writeFile(finalArray,fileName,rows,cols, createMap);
    
    //free array on heap
    free(finalArray);
}

char * readFile(char * fileName)
{
    //declare char array
    char fullName[50];
    
    //array contains first part of path
    char path[] = "levels/";
    
    //clear fullName array
    for(int i = 0; i < strlen(fullName); i++)
    {
        fullName[i] = '\0';
    }
    
    //copy 1st part into fullName
    strcpy(fullName,path);
    
    //add file name from user input into 1st part
    strcat(fullName,fileName);

    //read file with full path
    FILE * f = fopen(fullName,"r");
    
    //declare buf array contains everything from .pac file
    char buf[2500];
    
    //declare map array contains only map
    char map[2300];
    
    //clear buf array
    for(int i = 0; i < 2500; i++)
    {
        buf[i] = '\0';
    }
    
    //clear map array
    for(int i = 0; i < 2300; i++)
    {
        map[i] = '\0';
    }
    
    //return NULL of file's path is incorrect
    if (!f)
    {
        return NULL;
    }
	
    //erase all screen
    erase();
	
	//declare char array for rows and cols
	char rows[3] = {0,0,'\0'};
	char cols[3] = {0,0,'\0'};
	
	//delcare int row and col
	int row = 0;
	int col = 0;

	//authors
	fgets(buf,sizeof(buf),f);
	addstr("Authors: ");
	
	//add authors to screen
	addstr(buf);
	
	//title of the levels
	fgets(buf,sizeof(buf),f);
	addstr("Title: ");	
	
	//add title to screen
	addstr(buf);

	//rows
	fgets(buf,sizeof(buf),f);
	addstr("Rows: ");
	
	//add rows to screen	
	addstr(buf);
	
	//add rows
    for(int i = 0; i < 3; i++)
    {
        if(buf[i] != '\n')
        {
            //assign to rows array
            rows[i] = buf[i];
        }
        //convert to int
		row = atoi(rows);
    }
	
	//cols
	fgets(buf,sizeof(buf),f);
	addstr("Cols: ");
	
	//add cols to screen
	addstr(buf);
	
	//add cols
	for(int i = 0; i < 3; i++)
    {
        if(buf[i] != '\n')
        {
            //assign to cols array
            cols[i] = buf[i];
        }
        //convert to int
		col = atoi(cols);
    }
    
    //declare array on heap - declare on stack cause warning
    char * getFileInfo = malloc(sizeof(char)*2500);
    
    //clear getFileInfo array
    for(int i = 0; i < 2500; i++)
    {
        getFileInfo[i] = '\0';
    }
    
    //copy file name to getFileInfo array
    strcpy(getFileInfo,fileName);
    strcat(getFileInfo," ");
    
    //add rows to getFileInfo array
    strcat(getFileInfo,rows);
    strcat(getFileInfo," ");
    
    //add cols to getFileInfo array
    strcat(getFileInfo,cols);
    
    int i = 0;
    //read map 
    //buf[i] in this while loop read on top to bottom from left to right, NOT line by line
    //so cannot assign map[i] = buf[i] same as previous rows and cols
    while(fgets(buf,sizeof(buf),f))
    {
        if(buf[i] != '\0')
        {
            strcat(map,buf);
        }
        i++;
    }
    
    //convert line by line to 1 string
    for(int i = 0; map[i] != '\0'; i++)
    {
        if(map[i] == '\n')
        {
            for(int j = i; j < sizeof(map); j++)
            {
                map[j] = map[j+1];
            }
        }
    }
    
    strcat(getFileInfo," ");
    
    //add map to getFileInfo array
    strcat(getFileInfo,map);
    
    //draw map & display on the screen
    drawMap(fullName,col,row);
    
    //return array contain filename, rows and cols and map
    return getFileInfo;
}

void drawMap(char * fullName, int col, int row)
{
    //read file with above path
    FILE * f = fopen(fullName,"r");
    
    //declare char array contains authors, title, rows, cols
    char buf[100];
    
    //declare char c
    char c;
    
    //clear buf array
    for(int i = 0; i < 100; i++)
    {
        buf[i] = '\0';
    }
    
    //exit
    if (!f)
    {
        endwin();
	    exit(1);
    }
    
    //authors
	fgets(buf,sizeof(buf),f);
	//title of the levels
	fgets(buf,sizeof(buf),f);
	//rows
	fgets(buf,sizeof(buf),f);
	//cols
	fgets(buf,sizeof(buf),f);
	
	int maxX = 0;
	int maxY = 0;
	getmaxyx(stdscr,maxY,maxX);    
	int startX = (maxX - col)/2;	
	wmove(stdscr, 6, startX);  
	int mapY = 7;  
	
	//read map  
	do
    {
        //get each char from map
        c = fgetc(f);
             
		//if see \n
        if(c == '\n')
        {   
			wmove(stdscr, mapY,startX);
			mapY++; 
        }
       
        //func pointer
        getKey((char*)&c);
        
    }while(c != EOF);
    
    fclose(f);
}
