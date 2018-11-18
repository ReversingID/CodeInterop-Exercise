/**
Memanggil kode Go dari NodeJs

Membuat shared library (dll / so)

Compile:
	$ go build -o called -buildmode=c-shared called.go
*/

package main 

import "C"

import (
	"fmt"
	"math"
	"sort"
)

//export Add
func Add(a, b int) int { return a + b }

//export Cosine
func Cosine(x float64) float64 { return math.Cos(x) }

//export Sort
func Sort(vals []int) { sort.Ints(vals) }

//export Hello
func Hello(name string) {
	fmt.Println("Hello, {}!", name)
} 

func main() { }