//-----------------------------------------------------------------//
// written by Ganesh Hankare                                       //
// Initital version
// Date: 30th June 2023
//
// This code is responsible to display the list of files/Dirctories//
// subdirectories in a subfile.
// User can filter the data by providing pattern and regex with given//
// two seperate fields on the screen.
//-----------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <recio.h>
#include <dirent.h>
#include <string.h>

#define LEN 55
#define NUM_RECS 100
#define SUBFILENAME "*LIBL/SUBFDSPD02"
#define ARR_LIMIT 1000

typedef struct{
   char name[LEN];
}pf_t;
typedef struct{
   char searchfor[15];
   char pattern[6];
}fs_t;
typedef union
{
   pf_t fmt1;
   fs_t fmt2;
}formats;

#define RECLEN sizeof(pf_t)
char file_list[55][ARR_LIMIT];
int index = 0;

void substring(char [], char[], int, int);
void init_subfile(fs_t*,_RFILE *);
int main(void)
{
   _RFILE *pf;
   _RFILE *subf;
   //hd_t header;
   fs_t first;
   formats buf;
   _SYSindara indicator_area;

   if ((subf = _Ropen(SUBFILENAME, "ar+")) == NULL)
   {
      printf("can't open file %s\n", SUBFILENAME);
      exit(2);
   }

   // list down the all the files list to the array
   listFiles(".");

   _Rformat(subf, "FIRST");
   _Rwrite(subf, "", 0);
   _Rreadn(subf, &first, sizeof(first), __DFT);

   // Initialize the subfile with data from the array
   init_subfile(&first, subf);

   // display subfile format
   _Rformat(subf, "SFLCTL");
   _Rwrite(subf, "", 0);
   _Rreadn(subf, "", 0, __DFT);

   // Close the the subfile.
   _Rclose(subf);

   return 0;
}


void init_subfile(fs_t *first,_RFILE *subf)
{
   _RIOFB_T *fb;
   int i;
   int rrn;
   int is_find = 0;
   int position;
   char patterntext[5];
   char searchtext[15];
   char file_ext[5];
   pf_t record;

   _Rformat(subf, "SFL");
   removeTrailingSpaces(first->pattern,patterntext);
   removeTrailingSpaces(first->searchfor,searchtext);
   rrn = 0;

  for (i = 1; i <= NUM_RECS; i++)
  {
     if (i == index)
     {
        break;
     }
     is_find = 0;
     position = find(file_list[i], patterntext);
     substring(file_list[i], file_ext, position+1, 3);

     if (isFindInText(file_list[i], searchtext) == 1)
     {
        is_find = 1;
     }
     if (is_find == 1)
     {
        if (patterntext[0] == '*')
        {
           is_find = 0;
           if (position >= 1)
           {
              if (isFindInText(file_list[i], file_ext) == 1)
              {
                 is_find = 1;
              }
           }
        }
     }

     if (is_find == 1)
     {
        rrn += 1;
        strcpy(record.name,file_list[i]);
        fb = _Rwrited(subf, &record, RECLEN, rrn);
        if (fb->num_bytes != RECLEN)
        {
           printf("error occurred during write\n");
           exit(3);
        }
     }
  }
  if (rrn == 0)
  {
     strcpy(record.name,"No File Found");
     fb = _Rwrited(subf, &record, RECLEN, 1);
     if (fb->num_bytes != RECLEN)
     {
        printf("error occurred during write\n");
        exit(3);
     }
  }
}

// List the files/directories from IFS for given path
int listFiles(char *file_path)
{
    DIR *dr;
    struct dirent *en;
    char child_path[1000];
    char* file_name;
    //int index;

    //index = 0;
    dr = opendir(file_path);
    if (dr)
    {
       while ((en = readdir(dr)) != NULL)
       {
          // don't print defult directories
          if (strcmp(en->d_name,".") != 0 && strcmp(en->d_name,"..") != 0)
          {
             file_name = en->d_name;
             //strcpy(file_list[index++], file_name);
             index += 1;
             if (index > ARR_LIMIT)
             {
                break;
             }
             strcpy(file_list[index], en->d_name);

             //create a subfolder path
             if (isFindInText(file_list[index], ".") == 0)
             {
                strcpy(child_path, file_path);
                strcat(child_path, "/");
                strcat(child_path, en->d_name);

                listFiles(child_path);
             }
          }
       }
       closedir(dr);
    }
    return 0;
}

// search a part of string in base string to determine the text is
// present in the string
int isFindInText(char* text, char* search)
{
 char *ptr = strstr(text, search);

 if (ptr != NULL) // It indicate the search is find in the text
 {
     return 1;
 }
 else
 {
     return 0;
 }

 return 0;
}

// remove the trailing blanks
int removeTrailingSpaces(char *str1,char str2[])
{
  int index;

  for (index = 0; index <= 15; index++)
  {
     if (str1[index] != ' ')
     {
        str2[index] = str1[index];
     }
     else
     {
        str2[index] = '\0';
        break;
     }
  }
  return 0;
}


// This finds the provided string into the base string and return its position.
int find(char text[], char pattern[]) {
  int c, d, e, text_length, pattern_length, position = -1;

  text_length    = strlen(text);
  pattern_length = strlen(pattern)-1;

  if (pattern_length > text_length) {
    return -1;
  }

  for (c = 0; c <= text_length - pattern_length; c++) {
    position = e = c;

    for (d = 1; d < pattern_length; d++) {
      if (pattern[d] == text[e]) {
        e++;
      }
      else {
        break;
      }
    }
    if (d == pattern_length) {
      return position;
    }
  }

  return -1;
}

// copied from web as it is
void substring(char s[], char sub[], int p, int l) {
   int c = 0;

   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}
