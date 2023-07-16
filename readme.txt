Overview:
This utility displays the directory and files on the screen for the user default IFS path.
It provides a facility to filter the list, using “Pattern” and “Search” fields from the screen.
The “Pattern” is to filter the files according to their file types (extension) thus “Search” is to simply search by the contents.

Follow the installation option and steps mentioned below.

Display type:
It is a subfile, user can use scroll up/scroll down to see next/previous list.

Functionality:
User can fill the data to the fields or can search for all the directories/files available on the path by keeping fields “Pattern” and “Search” blank.
Refer manual document provided with this package.


Source/Object details:
Member      Type        Text                        
SUBFDSPD02  DSPF        Subfile display test program
SUBFDSPR02  C           Subfile display test program 


Installation options:
1. Download the source files from git and upload to IBMi.
2. Download the SAVF file provided in the email and install using below steps.


Installation option 2:

Steps to upload the SAVF to IBMi system.
FTP {ip address}
enter user name 
enter password
BIN
QUOTE SITE NAMEFMT 1
PUT FLSEARCH.SAVF /QSYS.LIB/QGPL.LIB/FLSEARCH.SAVF
QUIT


Steps to install and create objects and call untility program:
1. Create a library
	CRTLIB LIB(FLSEARCH) TYPE(*TEST) TEXT('File Search Utility v1.0')
2. Restore source files from save files
	RSTLIB SAVLIB(FLSEARCH) DEV(*SAVF) SAVF(QGPL/FLSEARCH) RSTLIB(FLSEARCH)
3. Go to libary 
	WRKMBRPDM FILE(FLSEARCH/QSRC)
4. Add work libary
	ADDLIBLE LIB(FLSEARCH)  
5. Create display file object (compile display)
		CRTDSPF FILE(FLSEARCH/SUBFDSPD02) SRCFILE(FLSEARCH/QSRC) SRCMBR(SUBFDSPD02)
6. Create program object (Compile program)
		CRTBNDC PGM(FLSEARCH/SUBFDSPR02) SRCFILE(FLSEARCH/QSRC) SRCMBR(SUBFDSPR02) DBGVIEW(*SOURCE)	
7. CALL PGM(SUBFDSPR02)	
 
