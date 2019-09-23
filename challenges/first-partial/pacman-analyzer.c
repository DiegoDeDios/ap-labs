#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include "logger.h"
#define REPORT_FILE "packages_report.txt"

int main(int argc, char** argv)
{

    if (argc < 2) {
	printf("Please specify a file to analyze\n");
	return 1;
    }

    return analizeLog(argv[1], REPORT_FILE);;
}

//----------------File Handling-----------------------

struct File* createFile(char* fileDir)
{   //File descriptor assigning
    struct File* newFile = (struct File*) malloc(sizeof(struct File)); 
    newFile->fileFD = open(fileDir, O_RDONLY); //The return value of open() is a file descriptor

    //Getting File size in bytes
    off_t curOffset = (newFile->fileFD, (size_t)0, SEEK_CUR); //lseek() repositions the file offset of the open file description
    newFile->fileSize = lseek(newFile->fileFD, (size_t)0, SEEK_END); //Find the EOF and count bytes

    lseek(newFile->fileFD, curOffset, SEEK_SET); //Make sure to rewind to original offset of file

    return newFile;
}

struct Analyzer* createAnalyzer(){
    struct Analyzer* newAnalyzer = (struct Analyzer*)malloc(sizeof(struct Analyzer));
    newAnalyzer->currentOffset = 0;
    return newAnalyzer;
}

int analizeLog(char* logFile, char* report)
{
    printf("Generating Report from: [%s] log file\n", logFile);

    struct File* myLog = createFile(logFile);
    struct Analyzer* logAnalyzer = createAnalyzer();
    struct Report* reportLog = createReport();

    char* fileBuffer = (char*)malloc(sizeof(char)*myLog->fileSize);
    logAnalyzer->lineAddress = fileBuffer;

    read(myLog->fileFD,fileBuffer,myLog->fileSize);

    char* pkgName;
    char* pkgDate;
    struct Package* updatePtr;

    int i = 0;
    unsigned int fileLines = countLines(fileBuffer,myLog->fileSize);

    while(i < fileLines){
    getLineSize(logAnalyzer, logAnalyzer->lineAddress);
    char* lineBuf = getLineBuffer(logAnalyzer, fileBuffer);
    switch(packageStatus(lineBuf)){
        case 1:
            pkgName = getPackageName(lineBuf,36);
            pkgDate = getPackageDate(lineBuf);
            insertPkg(reportLog, pkgName, pkgDate);
            break;
        case 2:
            updatePtr = searchPkg(reportLog,getPackageName(lineBuf,35));
            pkgDate = getPackageDate(lineBuf);
            updatePackage(updatePtr,pkgDate, reportLog);
            break; 
        case 3:
            updatePtr = searchPkg(reportLog,getPackageName(lineBuf,34));
            pkgDate = getPackageDate(lineBuf);
            removePackage(updatePtr,pkgDate, reportLog);
            break; 
    }

    i++;
    }
    getReport(reportLog,report);

    //And finally take out the trash
    free(fileBuffer);
    free(myLog);
    free(logAnalyzer);
    free(reportLog);

    printf("Report is generated at: [%s]\n", report);
    return close(myLog->fileFD); //Returns success :D
}

void getLineSize(struct Analyzer* analyzer,char* buffer){
    ssize_t lineSize = 0;
    do{
        lineSize++;
        buffer+=1;
    }
    while(*buffer!= '\n');
    analyzer->lineAddress = buffer + 1;
    analyzer->lineSize = lineSize;
}

unsigned int countLines(char* fileBuffer, off_t filesSize){
    int i = 0;
    unsigned int lineCounter = 0;
    while(i < filesSize){
        if(fileBuffer[i] == '\n'){
            lineCounter++;
        }
        i++;
    }
    return lineCounter;
}

char* getLineBuffer(struct Analyzer* analyzer, char* fileBuffer){
    char* lineBuffer = (char*)malloc(sizeof(char)*analyzer->lineSize);
    for(int i = 0; i < analyzer->lineSize; i++){
        lineBuffer[i] = fileBuffer[analyzer->currentOffset++];
    }
    analyzer->currentOffset++;
    return lineBuffer;
}

int getNameSize(char* lineBuffer, int offset){
    int nameSize = 0;
    while(lineBuffer[offset]!=' '){
        nameSize++;
        offset++;
    }
    return nameSize;
}

char* getPackageName(char* lineBuffer, int offset){
    int size = getNameSize(lineBuffer,offset);
    char* name = (char*)malloc(sizeof(char)*size);
    char i = offset;
    char j = 0;
    while(lineBuffer[i]!=' '){
        name[j++] = lineBuffer[i];
        i++;
    }
    return name;
}
char* getPackageDate(char* lineBuffer){
    char* date = (char*)malloc(sizeof(char)*16);
    for(int i = 0; i < 18;i++){
        date[i] = lineBuffer[i];
    }
    return date;
}

int packageStatus(char* buffer){
    if(strstr(buffer,"[ALPM] installed")!= NULL){
        return 1;
    }
    if(strstr(buffer,"[ALPM] upgraded")!= NULL){
        return 2;
    }
    if(strstr(buffer,"[ALPM] removed")!= NULL){
        return 3;
    }
    else{
        return -1;
    }
}


//--------------Package Handling-----------------------

struct Package* createPackage(char* name, char* date){
    struct Package* newPkg = (struct Package*)malloc(sizeof(struct Package));
    newPkg->packageName = name;
    newPkg->installDate = date;
    newPkg->isInstalled = 1;
    newPkg->isUpdated = 0;
    newPkg->timesUpdated = 0;
    return newPkg;
} 
void updatePackage(struct Package* pkgName, char* date, struct Report* fileReport){
    fileReport->upgradedPkgs += pkgName->isUpdated ? 1 : 0;
    pkgName->isUpdated = 1;
    pkgName->lastUpdate = date;
    pkgName->timesUpdated+=1;
} 
void removePackage(struct Package* pkgName, char* date, struct Report* fileReport){
    pkgName->removalDate=date;
    pkgName->isInstalled = 0;
    fileReport->removedPkgs++;
} 
void printPackage(struct Package* pkg, FILE* fp){
    if(pkg->lastUpdate == NULL){
        pkg->lastUpdate = "-";
    }
    if(pkg->removalDate == NULL){
        pkg->removalDate = "-";
    }
    fprintf(fp,"- Package Name: %s\nInstall date: %s\nLast Update date: %s\nHow many updates: %d\nRemoval Date: %s\n", pkg->packageName, pkg->installDate,pkg->lastUpdate, pkg->timesUpdated,pkg->removalDate);
} 

//-----------HashTable/Report Handling-------------

int hashCode(char* name, ssize_t size){
    return (long int)*name % size;
}
struct Report* createReport(){
    struct Report* newReport = (struct Report*)malloc(sizeof(struct Report));
    newReport->currentPkgs = 0;
    newReport->installedPkgs = 0;
    newReport->removedPkgs = 0;
    newReport->upgradedPkgs = 0;
    return newReport;
}
void insertPkg(struct Report* report, char* name, char* date){
    struct Package* newPkg = createPackage(name,date);
    int hashIndex = hashCode(newPkg->packageName,2048);
    if(report->PkgTable[hashIndex] == NULL){
        report->PkgTable[hashIndex] = newPkg;
    }
    else{
        struct Package* i = report->PkgTable[hashIndex];
        while(i!=NULL){
            if(i->nextPkg == NULL){
                i->nextPkg = newPkg;
                break;
            }
            i = i->nextPkg;
        }
    }
    report->installedPkgs++;
}
struct Package* searchPkg(struct Report* report, char* name){
    int hashIndex = hashCode(name,2048);
    struct Package* i = report->PkgTable[hashIndex];
    while(i!=NULL){
        if(!strcmp(name,i->packageName)){
            return i;
        }
        i = i->nextPkg;
    }
    return 0;
}

void getReport(struct Report* report, char* logFile){
    FILE * fp;
    fp = fopen (logFile,"w");
    report->currentPkgs = (report->installedPkgs) - (report->removedPkgs);
    fprintf(fp,"Pacman Packages Report\n----------------------\n- Installed packages : %d\n- Removed packages   : %d\n- Upgraded packages  : %d\n- Current installed  : %d\n", report->installedPkgs, report->removedPkgs, report->upgradedPkgs, report->currentPkgs);
    fprintf(fp,"-------------------------\n");
    for(int i = 0; i < 2048; i++){
        if(report->PkgTable[i]!=NULL){
            struct Package* temp = report->PkgTable[i];
            while(temp!=NULL){
                printPackage(temp,fp);
                temp = temp->nextPkg;
            }
        }
    }
    fclose (fp);

}