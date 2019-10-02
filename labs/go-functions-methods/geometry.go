package main

import (
	"fmt"
	"math"
	"os"
	"math/rand"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}


// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	fmt.Printf("\t- ")
	for i := range path {
		if i > 0 {
			fmt.Printf(" %.4f + ", path[i-1].Distance(path[i]))
			sum += path[i-1].Distance(path[i])
		}
	}
	fmt.Printf("%.4f", path[len(path) - 1].Distance(path[0]))
	sum += path[len(path) - 1].Distance(path[0])
	fmt.Print(" = ")
	return sum
}


func onSegment(p, q, r Point) bool {
	if q.X() <= math.Max(q.X(), r.X()) && q.X() >= math.Min(q.X(), r.X()) &&
		q.Y() <= math.Max(q.Y(), r.Y()) && q.Y() >= math.Min(q.Y(), r.Y()) {
			return true;
	}
	return false;
}
/* From : https://www.geeksforgeeks.org/orientation-3-ordered-points/
// To find orientation of ordered triplet (p1, p2, p3). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
*/
func getOrientation(p, q, r Point) int {
	val := ( (q.Y() - p.Y()) * (r.X() - p.X()) - (q.X() - p.X()) * (r.Y() - q.Y()) )
	if val == 0 {
		return 0
	}
	if val > 0 {
		return 1
	} else {
		return 2
	}
}


/*
1. General Case:
– (p1, q1, p2) and (p1, q1, q2) have different orientations and
– (p2, q2, p1) and (p2, q2, q1) have different orientations.
2- Special CasE:
– (p1, q1, p2), (p1, q1, q2), (p2, q2, p1), and (p2, q2, q1) are all collinear and
– the x-projections of (p1, q1) and (p2, q2) intersect
– the y-projections of (p1, q1) and (p2, q2) intersect
*/
func areIntersecting(p1, q1, p2, q2 Point) bool {
	l1 := getOrientation(p1, q1, p2)
	l2 := getOrientation(p1, q1, q2)
	l3 := getOrientation(p2, q2, p1)
	l4 := getOrientation(p2, q2, q1)
	if l1 != l2 && l3 != l4 {
		return true
	}
	if l1 == 0 && onSegment(p1, p2, q1) {
		return true
	}
	if l2 == 0 && onSegment(p1, q2, q1) {
		return true
	}
	if l3 == 0 && onSegment(p2, p1, q2) {
		return true
	}
	if l4 == 0 && onSegment(p2, q1, q2) {
		return true
	}
	return false
}

func genVertices (Paths Path, sides int) []Point {
	for i := 0; i < sides; i++ {
    Paths[i].x = (genRange(-100, 100))
    Paths[i].y = (genRange(-100, 100))
    fmt.Printf("- (%.4f , %.4f)\n" ,Paths[i].X(), Paths[i].Y())
  }
	return Paths
}

func genRange(min, max int ) float64 {
	seed := rand.NewSource(time.Now().UnixNano())
	rand := rand.New(seed)
	return rand.Float64()*float64((max - min) + min)
}

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Please input the number of vertices\n")
		return 
	}
	arg , _:=strconv.Atoi(os.Args[1])
	if arg < 3 {
		fmt.Printf("A closed figure must have more than 2 vertices\n")
		return 
	}
    fmt.Printf("- Generating a [%v] sided figure\n", arg)
	fmt.Print("- Figure's vertices\n")
	Paths := make(Path, arg) //Creating slice
	Paths = genVertices(Paths, arg)
	for areIntersecting(Paths[0], Paths[1], Paths[2], Paths[3]) {
		Paths = genVertices(Paths, arg)
	}
	fmt.Printf("- Figure's Perimeter:\n")
	fmt.Printf("%.4f\n", Paths.Distance())
}