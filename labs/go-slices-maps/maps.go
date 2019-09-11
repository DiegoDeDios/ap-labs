package main

import (
	"strings"
	"fmt"
)

func WordCount(s string) map[string]int {
	m:= make(map[string]int);
	for _, word := range strings.Fields(s){
		m[word] += 1;
		}
	return m;
}

func main() {
	fmt.Printf("El mapa es: %v\n",WordCount("Hola mundo esto es un lab"));
}

