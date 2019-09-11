package main

import "fmt"

func Pic(dx, dy int) [][]uint8 {
	picture:= make([][]uint8,dy);
	for i:= 0; i < dy; i++ {
		picture[i] = make([]uint8,dx);
			}
	for i:= 0; i < dy; i++{
		for j:= 0; j < dx; j++{
				picture[i][j] = uint8(i*j/2);
				}
			}
	return picture;
}

func main() {
	fmt.Printf("%v\n",Pic(4,5));
}

