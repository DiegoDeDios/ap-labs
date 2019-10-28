// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {
	var username string
	var host string

	flag.StringVar(&username, "user", "placeholder", "username")
	flag.StringVar(&host, "server", "localhost:9000", "server")

	flag.Parse()

	if username == "placeholder" {
		log.Fatal("Register a username to enter\n")
		os.Exit(-1);
	}

	conn, err := net.Dial("tcp", host)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Fprintf(conn, username+"\n")

	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("Connection closed from server")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}