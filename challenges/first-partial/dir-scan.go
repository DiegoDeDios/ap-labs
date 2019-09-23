package main

import (
	"fmt"
	"os"
	"path/filepath"
)

type Directory struct{
	name string
	dirs int
	symbolinks int
	devices int
	sockets int
	misc int
} 


// scanDir stands for the directory scanning implementation
func scanDir(myDir string) *Directory {
	directoryLog:= Directory{myDir, 0, 0, 0, 0, 0}
	var dirTraversal = func(directoryPath string, info os.FileInfo, err error) error {
		// first thing to do, check error. and decide what to do about it
		if err != nil {
			fmt.Printf("error %v at a path 「%q」\n", err, directoryPath)
			return err
		}
		// find out if it's a dir or file, if file, print info
		if info.IsDir() {
			directoryLog.dirs++;
		} 
		if info.Mode() & os.ModeSymlink !=0{
			directoryLog.symbolinks++;
		}
		if info.Mode() & os.ModeDevice !=0{
			directoryLog.devices++;
		}
		if info.Mode() & os.ModeSocket != 0{
			directoryLog.sockets++;
		}else {
			directoryLog.misc++;
		}
		return nil
	}
	err := filepath.Walk(myDir, dirTraversal)
	if err != nil {
		fmt.Printf("Error during directory traversal :c\n", myDir, err)
	}
	return &directoryLog;
}


func main() {
	if len(os.Args) < 2 {
		fmt.Println("To execute this program please specify a directory to analyze")
		os.Exit(1)
	} 

	scanResults:= scanDir(os.Args[1])
	fmt.Println("+-------------------------+------+");
	fmt.Println("| Path \t\t\t  |",scanResults.name, "|");
	fmt.Println("+-------------------------+------+");
	fmt.Println("| Directories \t\t  |",scanResults.dirs,"|");
	fmt.Println("| Symlinks \t\t  |",scanResults.symbolinks,"|");
	fmt.Println("| Devices \t\t  |",scanResults.devices,"|");
	fmt.Println("| Sockets \t\t  |",scanResults.sockets,"|");
	fmt.Println("| Others \t\t  |",scanResults.misc,"|");
	fmt.Println("+-------------------------+------+");
}
