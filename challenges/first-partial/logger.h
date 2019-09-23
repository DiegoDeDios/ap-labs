#ifndef sys/types.h
#define sys/types.h
struct File{
    int fileFD;
    off_t fileSize;
};

struct Analyzer{
    char* lineAddress;
    int currentOffset;
    ssize_t lineSize;
};

struct Package{
    char* packageName; 
    char* installDate; 
    char* lastUpdate; 
    char* removalDate;
    char isInstalled, isUpdated;
    int timesUpdated;
    struct Package* nextPkg; // Packages will be the nodes of the linked list in the hashtable
};

struct PkgNode{ //This will be the nodes of the linked list in the hash table
    struct Package* pkg;
    struct Package* next;
};

struct Report{
    struct Package* PkgTable[2048]; //This will work as the hashtable
    int installedPkgs, removedPkgs, upgradedPkgs, currentPkgs; 
};

unsigned int countLines(char*, off_t);
int analizeLog(char*, char*);
void getLineSize(struct Analyzer*,char*);
char* getLineBuffer(struct Analyzer*, char*);
int packageStatus(char*);
int getNameSize(char*,int);
char* getPackageName(char*,int);
char* getPackageDate(char*);


//File handling
struct File* createFile(char*);
struct Analyzer* createAnalyzer();

//Hash table
struct Report* createReport();
void insertPkg(struct Report*, char*, char*);
struct Package* searchPkg(struct Report*, char*);
int hashCode(char*, ssize_t);
void getReport(struct Report*, char*);

//Handling Packages
struct Package* createPackage(char*, char*); // Name, Installation Date
void updatePackage(struct Package*, char*, struct Report*); // Package name, update date
void removePackage(struct Package*, char*, struct Report*); // Package name, removal date;
void printPackage(struct Package*, FILE*); //Package name, file stream
#endif